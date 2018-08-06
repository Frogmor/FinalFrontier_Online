// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionInterface.h"

#include "MenuSystem/MenuInterface.h"
#include "FinalFrontierGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class FINQLFRONTIERII_API UFinalFrontierGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

		UFinalFrontierGameInstance(const FObjectInitializer & FobjInit);

public:

	virtual void Init() override;

	UFUNCTION(BlueprintCallable, Category = "MainMenu")
	void LoadMenuWidget();

	UFUNCTION(BlueprintCallable, Category = "MainMenu")
	void InGameLoadMenu();
		


	void Host(bool lan, FString ServerName) override;

	void Join(uint32 index) override;

	virtual void LoadMainMenu() override;

	void RefreshServerList() override;

	UFUNCTION(BlueprintCallable, Category = "MainMenu")
	void setNumberOfPlayers(int32 nb);

	UFUNCTION(BlueprintCallable, Category = "MainMenu")
	int32 getNumberOfPlayers(int32 nb);

private :

	int32 numberOfPlayers;

	FString DesiredServerName;

	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> InGameMenuClass;
	
	class UMainMenu* Menu;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void CreateSession(bool lan);
	void OnFindSessionComplete(bool Sucess);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
};
