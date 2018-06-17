// Fill out your copyright notice in the Description page of Project Settings.

#include "FinqlFrontierII.h"
#include "IngameMenu.h"
#include "Components/Button.h"



bool UIngameMenu::Initialize() {
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(MainMenuButton != nullptr)) return false;
	MainMenuButton->OnClicked.AddDynamic(this, &UIngameMenu::BackToMainMenu);

	if (!ensure(QuitGameButton != nullptr)) return false;
	QuitGameButton->OnClicked.AddDynamic(this, &UIngameMenu::QuitGame);

	if (!ensure(CancelButton != nullptr)) return false;
	CancelButton->OnClicked.AddDynamic(this, &UIngameMenu::CancelPause);

	return true;
}

void UIngameMenu::BackToMainMenu()
{
	if (InterfaceOfMenu != nullptr) {
		InterfaceOfMenu->LoadMainMenu();
	}
}

void UIngameMenu::QuitGame()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ConsoleCommand("Quit", false);
}

void UIngameMenu::CancelPause()
{
	TearDown();
}
