// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FinqlFrontierII.h"
#include "FinqlFrontierIICharacter.h"
#include "FinqlFrontierIIProjectile.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"
#include "Grenade_Explo.h"
#include "arme_a_feu.h"
//#include "arme_a_feu.h"
#include "Engine.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//class Aarme_a_feu* ArmeUn = NewObject<Aarme_a_feu>();

//////////////////////////////////////////////////////////////////////////
// AFinqlFrontierIICharacter

void AFinqlFrontierIICharacter::AddThrowingPower()
{
	if (this->ThrowingPower < 5000) {
		int32 i = 1;
		UE_LOG(LogTemp, Warning, TEXT("vitesse 2 : %d, %d"), ThrowingPower, i);
		i = i + 89;
		this->ThrowingPower = this->ThrowingPower + 333;
		UE_LOG(LogTemp, Warning, TEXT("vitesse 3 : %d, %d"), ThrowingPower, i);
	}
}

AFinqlFrontierIICharacter::AFinqlFrontierIICharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	PlayerVariable.HealthPoint = 1.0f;

	ActHealth = PlayerVariable.HealthPoint / PlayerVariable.MaxHealthPoint;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->Hand = EControllerHand::Right;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);

	// Create a gun and attach it to the right-hand VR controller.
	// Create a gun mesh component
	VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	VR_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	VR_Gun->bCastDynamicShadow = false;
	VR_Gun->CastShadow = false;
	VR_Gun->SetupAttachment(R_MotionController);
	VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	VR_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VR_MuzzleLocation"));
	VR_MuzzleLocation->SetupAttachment(VR_Gun);
	VR_MuzzleLocation->SetRelativeLocation(FVector(0.000004, 53.999992, 10.000000));
	VR_MuzzleLocation->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));		// Counteract the rotation of the VR gun model.
}

void AFinqlFrontierIICharacter::setThrowingPower(int32 nThrowinPower)
{
	this->ThrowingPower = nThrowinPower;
}

int32 AFinqlFrontierIICharacter::getThrowingPower()
{
	return this->ThrowingPower;
}

void AFinqlFrontierIICharacter::TakeDamage(float damage)
{
	PlayerVariable.HealthPoint = PlayerVariable.HealthPoint - (damage/PlayerVariable.MaxHealthPoint);
}

void AFinqlFrontierIICharacter::InvertWeapon()
{
	TSubclassOf<class Aarme_a_feu> TempArme;
	FString temp = "";
	
	if (Arme2 != nullptr) {
		temp = Arme1->GetName();
		TempArme = Arme1;
		Arme1 = Arme2;
		Arme2 = TempArme;
		UE_LOG(LogTemp, Warning, TEXT("Arme : %s"), *temp);
	}
}

void AFinqlFrontierIICharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	ThrowingPower = 0;
	PlayerVariable.GrenadeExplo = 2;

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	if (bUsingMotionControllers)
	{
		VR_Gun->SetHiddenInGame(false, true);
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		VR_Gun->SetHiddenInGame(true, true);
		Mesh1P->SetHiddenInGame(false, true);
	}
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		const FRotator SpawnRotation = GetControlRotation();
		// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
		const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);
		World->SpawnActor<Aarme_a_feu>(Arme1, SpawnLocation, SpawnRotation);
		World->SpawnActor<Aarme_a_feu>(Arme2, SpawnLocation, SpawnRotation);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFinqlFrontierIICharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AFinqlFrontierIICharacter::TouchStarted);
	
	/*if (EnableTouchscreenMovement(PlayerInputComponent) == false)
	{
		PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFinqlFrontierIICharacter::OnFire);
		PlayerInputComponent->BindAction("Fire", IE_Released, this, &AFinqlFrontierIICharacter::OnStopFire);
	}*/

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AFinqlFrontierIICharacter::OnResetVR);
	PlayerInputComponent->BindAction("Grenade", IE_Pressed, this, &AFinqlFrontierIICharacter::OnGrenadeThrowing);
	PlayerInputComponent->BindAction("Grenade", IE_Released, this, &AFinqlFrontierIICharacter::OnGrenade);
	PlayerInputComponent->BindAction("PickUpWeapon", IE_Pressed, this, &AFinqlFrontierIICharacter::OnPickUpWeapon);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFinqlFrontierIICharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFinqlFrontierIICharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFinqlFrontierIICharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFinqlFrontierIICharacter::LookUpAtRate);
}

void AFinqlFrontierIICharacter::CreatePellet()
{
	UWorld* const World = GetWorld();
	if (World != NULL)
	{
		if (bUsingMotionControllers)
		{
			const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
			const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
			World->SpawnActor<AFinqlFrontierIIProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
		}
		else
		{
			const FRotator SpawnRotation = GetControlRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			World->SpawnActor<AFinqlFrontierIIProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}
	//}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}




void AFinqlFrontierIICharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AFinqlFrontierIICharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AFinqlFrontierIICharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
//		OnFire();
	}
	TouchItem.bIsPressed = false;
}

//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void AFinqlFrontierIICharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}

void AFinqlFrontierIICharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFinqlFrontierIICharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AFinqlFrontierIICharacter::OnDash()
{
	FVector Velo,c = FVector(0.0f, 0.0f, 0.0f);
	float a, b = 0.0;
	a = GetInputAxisValue("MoveForward") * 3000;
	b = GetInputAxisValue("MoveRight") * 3000;
	c = ((GetActorForwardVector()*a) + (GetActorRightVector()*b));
	Velo = (c + (GetVelocity() * 3));
	LaunchCharacter(Velo, false, false);	

}

void AFinqlFrontierIICharacter::OnGrenade()
{
	GetWorldTimerManager().ClearTimer(MyHandle);
	const FRotator SpawnRotation = GetControlRotation();
	const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
		UWorld* const World = GetWorld();
		AGrenade_Explo* defaultGrenade = NewObject<AGrenade_Explo>();
		
		if (World != NULL && PlayerVariable.GrenadeExplo>0)
		{				
			
			auto Projectile = World->SpawnActor<AGrenade_Explo>(MyGrenade_Explo_BluePrint, SpawnLocation, SpawnRotation, ActorSpawnParams);
			Projectile->LaunchProjectile(ThrowingPower);
			PlayerVariable.GrenadeExplo--;
		}
		ThrowingPower = 0;
}

void AFinqlFrontierIICharacter::OnGrenadeThrowing()
{
	GetWorldTimerManager().SetTimer(MyHandle, this, &AFinqlFrontierIICharacter::AddThrowingPower, 0.25f, true);
}

void AFinqlFrontierIICharacter::OnPickUpWeapon()
{

}

void AFinqlFrontierIICharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFinqlFrontierIICharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool AFinqlFrontierIICharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	bool bResult = false;
	if (FPlatformMisc::GetUseVirtualJoysticks() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		bResult = true;
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AFinqlFrontierIICharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AFinqlFrontierIICharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AFinqlFrontierIICharacter::TouchUpdate);
	}
	return bResult;
}
