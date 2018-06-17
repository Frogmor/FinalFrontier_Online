// Fill out your copyright notice in the Description page of Project Settings.

#include "FinqlFrontierII.h"
#include "Grenade_Explo.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AGrenade_Explo::AGrenade_Explo()
{
	OnDestroyed.AddDynamic(this, &AGrenade_Explo::WhenDestroyed);

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Collision Mesh"));
	SetRootComponent(CollisionMesh);
	CollisionMesh->SetNotifyRigidBodyCollision(true);
	CollisionMesh->SetVisibility(true);

	//LaunchBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Launch Blast"));
	//LaunchBlast->AttachTo(RootComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile Movement"));
	ProjectileMovement->bAutoActivate = false;
	ProjectileMovement->MaxSpeed = 1500.0f;
	ProjectileMovement->InitialSpeed = 0.0f;

	ImpactBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Impact Blast"));
	ImpactBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ImpactBlast->bAutoActivate = false;

	ExplosionForce = CreateDefaultSubobject<URadialForceComponent>(FName("Explosion Force"));
	ExplosionForce->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

}

// Called when the game starts or when spawned
void AGrenade_Explo::BeginPlay()
{
	Super::BeginPlay();
}

void AGrenade_Explo::WhenDestroyed(AActor * Act)
{
	ExplosionForce->FireImpulse();
	
}

// Called every frame
void AGrenade_Explo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AGrenade_Explo::LaunchProjectile(float Speed)
{
	FVector temp = FVector(1, 0, 0);
	ProjectileMovement->SetVelocityInLocalSpace(temp * Speed);
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Activate();
}

void AGrenade_Explo::setnSpeed(int32 newSpeed)
{
	this->nSpeed = newSpeed;
}

int32 AGrenade_Explo::getnSpeed()
{
	return int32();
}
