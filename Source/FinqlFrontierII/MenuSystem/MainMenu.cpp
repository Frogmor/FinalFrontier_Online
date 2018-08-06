// Fill out your copyright notice in the Description page of Project Settings.

#include "FinqlFrontierII.h"
#include "MainMenu.h"
#include "FinqlFrontierIIGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/CheckBox.h"
#include "ServerRow.h"

UMainMenu::UMainMenu(const FObjectInitializer & ObjectInitializer) {
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/UI/MenuSystem/WPB_Server_Row"));
	if (!ensure(ServerRowBPClass.Class != nullptr)) return;

	ServerRowClass = ServerRowBPClass.Class;
}

void UMainMenu::SetServerList(TArray<FServerData> ServerNames)
{
	UWorld* world = this->GetWorld();
	if (!ensure(world != nullptr)) return;

	ServerList->ClearChildren();
	uint32 i = 0;
	for (const FServerData& serverData : ServerNames)
	{
		UServerRow* Row = CreateWidget<UServerRow>(world, ServerRowClass);
		if (!ensure(Row != nullptr))return;

		Row->ServerName->SetText(FText::FromString(serverData.Name));
		Row->HostUser->SetText(FText::FromString(serverData.HostUsername));
		FString Franction = FString::Printf(TEXT("%d/%d "), serverData.CurrentPlayers, serverData.MaxPlayers);
		Row->ConnectionFraction->SetText(FText::FromString(Franction));
		Row->Setup(this, i);
		++i;

		ServerList->AddChild(Row);
	}
	
}

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}

void UMainMenu::addPlayer()
{
	NbPlayers++;
}

int32 UMainMenu::getNbPlayers()
{
	return NbPlayers;
}

int32 UMainMenu::getActiveSwitcher()
{
	return MenuSwitcher->GetActiveWidgetIndex();
}

bool UMainMenu::Initialize() {
	bool Success = Super::Initialize();
	if (!Success) return false; 

	NbPlayers = -1;

	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(JoinButton != nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(HostMenuButton != nullptr)) return false;
	HostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenHostMenu);

	if (!ensure(LocalMenuButton != nullptr)) return false;
	LocalMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenLocalMenu);

	if (!ensure(JoinBackButton != nullptr)) return false;
	JoinBackButton->OnClicked.AddDynamic(this, &UMainMenu::BackToMainMenu);

	if (!ensure(HostBackButton != nullptr)) return false;
	HostBackButton->OnClicked.AddDynamic(this, &UMainMenu::BackToMainMenu);

	if (!ensure(ConnectButton != nullptr)) return false;
	ConnectButton->OnClicked.AddDynamic(this, &UMainMenu::CoonectToIP);

	if (!ensure(LocalLaunchButton != nullptr)) return false;
	LocalLaunchButton->OnClicked.AddDynamic(this, &UMainMenu::JoinLocalGame);
	

	if (!ensure(QuitButton != nullptr)) return false;
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);	

	return true;
}

void UMainMenu::HostServer()
{
	if (InterfaceOfMenu != nullptr) {
		InterfaceOfMenu->Host(IsLAN->IsChecked(), ServerHostName->Text.ToString());
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;

	MenuSwitcher->SetActiveWidget(JoinMenu);

	if (InterfaceOfMenu != nullptr) {
		InterfaceOfMenu->RefreshServerList();
	}
}

void UMainMenu::OpenHostMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(HostMenu != nullptr)) return;

	MenuSwitcher->SetActiveWidget(HostMenu);
}

void UMainMenu::OpenLocalMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(LocalMenu != nullptr)) return;

	MenuSwitcher->SetActiveWidget(LocalMenu);
}

void UMainMenu::BackToMainMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;

	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::CoonectToIP()
{
	if (SelectedIndex.IsSet() && InterfaceOfMenu != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected index %d"), SelectedIndex.GetValue())
		InterfaceOfMenu->Join(SelectedIndex.GetValue());
	}
	else UE_LOG(LogTemp, Warning, TEXT("Selected index not set"));

}

void UMainMenu::JoinLocalGame()
{
	UE_LOG(LogTemp, Warning, TEXT("nbplayers : %d"), NbPlayers);
	UWorld* world = GetWorld();
	if (!ensure(world != nullptr)) return;

	if (NbPlayers > -1)
		UGameplayStatics::OpenLevel(world, "TestLevel");
	else UE_LOG(LogTemp, Warning, TEXT("Il faut au moins un joueur pour rejoindre la partie"));
}

void UMainMenu::QuitGame()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ConsoleCommand("Quit", false);
}

void UMainMenu::UpdateChildren()
{
	for (int32 i = 0; i < ServerList->GetChildrenCount(); ++i) {
		auto* Row = Cast<UServerRow>(ServerList->GetChildAt(i));
		if (Row != nullptr) {
			Row->Selected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);
		}
	}
}
