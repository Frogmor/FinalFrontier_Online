// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "arme_a_feu.generated.h"

UCLASS()
class FINQLFRONTIERII_API Aarme_a_feu : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Aarme_a_feu();
	//Aarme_a_feu(FString nNom, FString nType, int32 nCharge);

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "InfoArme")
	int32 ChargeurMax;
	UPROPERTY(EditAnywhere, Category = "InfoArme")
	FString Nom;
	UPROPERTY(EditAnywhere, Category = "InfoArme")
	FString TypeTire;

	UPROPERTY(EditAnywhere, Category = "InfoArme")
	int32 MunitionAct;
	int32 Chargeur;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "ChangeWeapon")
	void SetMunitionAct(int32 nMuni);
	UFUNCTION(BlueprintCallable, Category = "ChangeWeapon")
	void SetNom(FString nNom);
	UFUNCTION(BlueprintCallable, Category = "ChangeWeapon")
	void SetTypeTire(FString nType);
	UFUNCTION(BlueprintCallable, Category = "ChangeWeapon")
	void SetChargeurMax(int32 nCharge);

	UFUNCTION(BlueprintCallable, Category = "ChangeWeapon")
	void Recharger();
	UFUNCTION(BlueprintCallable, Category = "ChangeWeapon")
	void tirer();

	UFUNCTION(BlueprintCallable, Category = "ChangeWeapon")
	FString GetNom();
	UFUNCTION(BlueprintCallable, Category = "ChangeWeapon")
	FString GetTypeTire(); 
	UFUNCTION(BlueprintCallable, Category = "ChangeWeapon")
	int32 GetChargeurMax();
	UFUNCTION(BlueprintCallable, Category = "ChangeWeapon")
	int32 GetMunitionAct();
	UFUNCTION(BlueprintCallable, Category = "ChangeWeapon")
	 int32 GetChargeur();

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USphereComponent* SphereTrigger;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* CollisionMesh = nullptr;
	
};
