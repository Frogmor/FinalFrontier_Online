// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MenuSystem/MenuWidget.h"
#include "MenuInterface.h"
#include "MainMenu.generated.h"

USTRUCT()
struct FServerData
{
	GENERATED_BODY()

		FString Name;
		uint16 CurrentPlayers;
		uint16 MaxPlayers;
		FString HostUsername;

};

/**
 * 
 */
UCLASS()
class FINQLFRONTIERII_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()
	
public:
	UMainMenu(const FObjectInitializer & ObjectInitializer);

	void SetServerList(TArray<FServerData> ServerNames);

	void SelectIndex(uint32 Index);

protected:
	virtual bool Initialize();

private:
	TSubclassOf<class UUserWidget> ServerRowClass;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinBackButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostBackButton;	

	UPROPERTY(meta = (BindWidget))
	class UButton* ConnectButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;
	

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ServerList;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* HostMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UCheckBox* IsLAN;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ServerHostName;
	

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenHostMenu();

	UFUNCTION()
	void BackToMainMenu();

	UFUNCTION()
	void CoonectToIP();

	UFUNCTION()
		void QuitGame();

	TOptional<uint32> SelectedIndex;

	void UpdateChildren();
};
