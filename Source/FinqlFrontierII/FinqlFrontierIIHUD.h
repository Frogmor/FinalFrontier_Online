// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once 
#include "GameFramework/HUD.h"
#include "FinqlFrontierIIHUD.generated.h"

UCLASS()
class AFinqlFrontierIIHUD : public AHUD
{
	GENERATED_BODY()

public:
	AFinqlFrontierIIHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;
};

