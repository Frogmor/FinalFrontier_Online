// Fill out your copyright notice in the Description page of Project Settings.

#include "FinqlFrontierII.h"
#include "FinalFrontierGameInstance.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"

#include "UObject/ConstructorHelpers.h"
#include "MenuSystem/MainMenu.h"
//#include "MenuSystem/MenuWidget.h"

#include "OnlineSessionSettings.h"

const static FName SESSION_NAME = TEXT("Game");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");


UFinalFrontierGameInstance::UFinalFrontierGameInstance(const FObjectInitializer & FobjInit) {
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/UI/MenuSystem/WBP_MainMenu"));

	if (!ensure(MenuBPClass.Class != nullptr))return;

	MenuClass = MenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/UI/MenuSystem/WBP_PauseMenu"));

	if (!ensure(InGameMenuBPClass.Class != nullptr))return;

	InGameMenuClass = InGameMenuBPClass.Class;

}

void UFinalFrontierGameInstance::Init()
{
	IOnlineSubsystem *SubSys = IOnlineSubsystem::Get();
	if (SubSys != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Found Subsystem %s"), *SubSys->Subs().ToString());
		
		SessionInterface = SubSys->GetSessionInterface();
		if (SessionInterface.IsValid()) {
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UFinalFrontierGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UFinalFrontierGameInstance::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UFinalFrontierGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UFinalFrontierGameInstance::OnJoinSessionComplete);

		}
	}
	else UE_LOG(LogTemp, Warning, TEXT("Found no Subsystem "));
}

void UFinalFrontierGameInstance::LoadMenuWidget()
{
	if (!ensure(MenuClass != nullptr))return;

	Menu = CreateWidget<UMainMenu>(this, MenuClass);

	if (!ensure(Menu != nullptr))return;

	Menu->setup();
	Menu->setInterface(this);
}

void UFinalFrontierGameInstance::InGameLoadMenu()
{
	if (!ensure(InGameMenuClass != nullptr))return;

	UMenuWidget* GameMenu = CreateWidget<UMenuWidget>(this, InGameMenuClass);

	if (!ensure(GameMenu != nullptr))return;
	GameMenu->setup();
	GameMenu->setInterface(this);
}

void UFinalFrontierGameInstance::Host(bool lan, FString ServerName)
{

	DesiredServerName = ServerName;
	if (SessionInterface.IsValid()) {
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession != nullptr) {
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else {
			CreateSession(lan);
		}
	}
	
}

void UFinalFrontierGameInstance::Join(uint32 index)
{
	if (!SessionInterface.IsValid()) return;
	if (!SessionSearch.IsValid()) return;

	//if (Menu != nullptr) {
	//	Menu->SetServerList({"Test1","Test2"});
	//}

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[index]);


}

void UFinalFrontierGameInstance::LoadMainMenu()
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;
	
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;
	PlayerController->ClientTravel("/Game/UI/MenuSystem/MainMenu",
		ETravelType::TRAVEL_Absolute);
}

void UFinalFrontierGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	if (SessionSearch.IsValid()) {

		SessionSearch->MaxSearchResults = 2000000;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		UE_LOG(LogTemp, Warning, TEXT("Starting Find Session"));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UFinalFrontierGameInstance::setNumberOfPlayers(int32 nb)
{
	numberOfPlayers = nb;
}

int32 UFinalFrontierGameInstance::getNumberOfPlayers(int32 nb)
{
	return numberOfPlayers;
}

void UFinalFrontierGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success) {
		UE_LOG(LogTemp, Warning, TEXT("Could not create session"));
		return;
	}
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel(
		"/Game/FirstPersonCPP/Maps/Lobby?listen");
}

void UFinalFrontierGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	//if (Success) {
	//	CreateSession();
	//}
}

void UFinalFrontierGameInstance::CreateSession(bool lan)
{
	if (SessionInterface.IsValid()) {
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsLANMatch = lan;
		SessionSettings.NumPublicConnections = 4;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

void UFinalFrontierGameInstance::OnFindSessionComplete(bool Sucess)
{
	if (Sucess && SessionSearch.IsValid() && Menu != nullptr) {
		UEngine* Engine = GetEngine();
		if (!ensure(Engine != nullptr)) return;

		Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Finished find session"));
		UE_LOG(LogTemp, Warning, TEXT("Finished find session"));

		TArray<FServerData> ServerNames;
		
		for (const FOnlineSessionSearchResult &SearchResult : SessionSearch->SearchResults) {
			Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("Joining %s"), *SearchResult.GetSessionIdStr()));
			UE_LOG(LogTemp, Warning, TEXT("Found Session Names %s"), *SearchResult.GetSessionIdStr());
			FServerData Data;
			//Data.Name = SearchResult.GetSessionIdStr();
			Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
			Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
			Data.HostUsername = SearchResult.Session.OwningUserName;

			FString ServerName;
			if (SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName))
			{
				Data.Name = ServerName;
			}
			else
			{
				Data.Name = "Could not find name.";
			}

			ServerNames.Add(Data);
		}

		Menu->SetServerList(ServerNames);
	}
}

void UFinalFrontierGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid()) return;

	FString address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, address)) {
		UE_LOG(LogTemp, Warning, TEXT("Couldn't get connect string"));
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 5, FColor::Green,
		FString::Printf(TEXT("Joining %s"), *address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel(address,
			ETravelType::TRAVEL_Absolute);
}
