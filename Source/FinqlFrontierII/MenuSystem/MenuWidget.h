// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINQLFRONTIERII_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void setInterface(IMenuInterface* nMenuInterface);
	void setup();
	void TearDown();
	
protected:
	virtual void OnLevelRemovedFromWorld(ULevel * InLevel, UWorld * InWorld);
	IMenuInterface* InterfaceOfMenu;
};
