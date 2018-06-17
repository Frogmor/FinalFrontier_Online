// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameModeBase.h"
#include "FinqlFrontierIIGameMode.generated.h"

UCLASS(minimalapi)
class AFinqlFrontierIIGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	static int32 Players;
public:
	AFinqlFrontierIIGameMode();


};
