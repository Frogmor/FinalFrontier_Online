// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MenuInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMenuInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * 
 */
class FINQLFRONTIERII_API IMenuInterface
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	virtual void Host(bool lan, FString ServerName) = 0;
	
	virtual void Join(uint32 index) = 0;

	virtual void LoadMainMenu() = 0;

	virtual void RefreshServerList() = 0;

};
