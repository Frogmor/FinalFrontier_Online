// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FinqlFrontierIIGameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FINQLFRONTIERII_API ALobbyGameMode : public AFinqlFrontierIIGameMode
{
	GENERATED_BODY()


public:

	void PostLogin(APlayerController* NewPlayer) override;

	void Logout(AController * Exiting) override;

private:
	uint32 numberOfPlayers = 0;
	
};
