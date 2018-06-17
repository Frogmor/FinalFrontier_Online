// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MenuSystem/MenuWidget.h"
#include "IngameMenu.generated.h"

/**
 * 
 */
UCLASS()
class FINQLFRONTIERII_API UIngameMenu : public UMenuWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize();


private:
	UPROPERTY(meta = (BindWidget))
		class UButton* MainMenuButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* QuitGameButton;
	UPROPERTY(meta = (BindWidget))
		class UButton* CancelButton;


	UFUNCTION()
		void BackToMainMenu();
	UFUNCTION()
		void QuitGame();
	UFUNCTION()
		void CancelPause();
	
	
};
