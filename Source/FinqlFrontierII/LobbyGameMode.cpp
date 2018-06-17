// Fill out your copyright notice in the Description page of Project Settings.

#include "FinqlFrontierII.h"
#include "LobbyGameMode.h"

void ALobbyGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++numberOfPlayers;

	if (numberOfPlayers >= 4) {
		UWorld* World = GetWorld();
		if (!ensure(World != nullptr)) return;

		bUseSeamlessTravel = true;

		World->ServerTravel(
			"/Game/FirstPersonCPP/Maps/FirstPersonExampleMap?listen");
	}
}

void ALobbyGameMode::Logout(AController * Exiting)
{
	Super::Logout(Exiting);
	--numberOfPlayers;
}
