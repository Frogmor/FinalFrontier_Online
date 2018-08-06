// Fill out your copyright notice in the Description page of Project Settings.

#include "FinqlFrontierII.h"
#include "MenuWidget.h"


void UMenuWidget::setInterface(IMenuInterface* nMenuInterface) {
	InterfaceOfMenu = nMenuInterface;
}

void UMenuWidget::setup()
{

	this->AddToViewport();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeUIOnly InputMode;
	
	//InputMode.SetWidgetToFocus(this->TakeWidget());
	//InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	//PlayerController->SetInputMode(InputMode);

	PlayerController->bShowMouseCursor = true;
}

void UMenuWidget::OnLevelRemovedFromWorld(ULevel * InLevel, UWorld * InWorld)
{
	TearDown();

}

void UMenuWidget::TearDown()
{
	this->RemoveFromViewport();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeGameOnly InputMode;

	PlayerController->bShowMouseCursor = false;
	PlayerController->SetInputMode(InputMode);

}


