// Fill out your copyright notice in the Description page of Project Settings.

#include "FinqlFrontierII.h"
#include "arme_a_feu.h"


// Sets default values
Aarme_a_feu::Aarme_a_feu()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Collision Mesh"));
	SetRootComponent(CollisionMesh);

	SphereTrigger = CreateDefaultSubobject<USphereComponent>(FName("Sphere Trigger"));
	SphereTrigger->InitSphereRadius(50);
	SphereTrigger->SetupAttachment(RootComponent);
}


// Called when the game starts or when spawned
void Aarme_a_feu::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void Aarme_a_feu::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void Aarme_a_feu::SetMunitionAct(int32 nMuni)
{
	MunitionAct = MunitionAct + nMuni;
}

void Aarme_a_feu::SetNom(FString nNom)
{
	Nom = nNom;
}

void Aarme_a_feu::SetTypeTire(FString nType)
{
	TypeTire = nType;
}

void Aarme_a_feu::SetChargeurMax(int32 nCharge)
{
	ChargeurMax = nCharge;
}

void Aarme_a_feu::Recharger()
{
	UE_LOG(LogTemp, Warning, TEXT("Chargeur avant : %d"), Chargeur);
	UE_LOG(LogTemp, Warning, TEXT("MunitionAct avant : %d"), MunitionAct);
	UE_LOG(LogTemp, Warning, TEXT("ChargeurMax avant : %d"), ChargeurMax);
	int32 temp = ChargeurMax - Chargeur;
	if (MunitionAct > temp) {
		if (MunitionAct >= temp) {
			Chargeur = Chargeur + temp;
			MunitionAct = MunitionAct - temp;
		}
		else {
			Chargeur = Chargeur + MunitionAct;
			MunitionAct = 0;
		}
	}
	else {
		Chargeur = Chargeur + MunitionAct;
		MunitionAct = 0;
		UE_LOG(LogTemp, Warning, TEXT("Chargeur vide"));
	}
	UE_LOG(LogTemp, Warning, TEXT("Chargeur après %d"), Chargeur);
	UE_LOG(LogTemp, Warning, TEXT("MunitionAct après %d"), MunitionAct);
	UE_LOG(LogTemp, Warning, TEXT("ChargeurMax après %d"), ChargeurMax);

}

void Aarme_a_feu::tirer()
{
	if (Chargeur > 0) Chargeur--;
}

FString Aarme_a_feu::GetNom()
{
	return Nom;
}

FString Aarme_a_feu::GetTypeTire()
{
	return TypeTire;
}

int32 Aarme_a_feu::GetChargeurMax()
{
	return ChargeurMax;
}

int32 Aarme_a_feu::GetMunitionAct()
{
	return MunitionAct;
}

int32 Aarme_a_feu::GetChargeur()
{
	return Chargeur;
}

