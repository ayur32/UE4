// Fill out your copyright notice in the Description page of Project Settings.


#include "HOPEWeapon.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AHOPEWeapon::AHOPEWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	Scope = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SCOPE"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> RF(TEXT("SkeletalMesh'/Game/Gun/Gun.Gun'"));
	if (RF.Succeeded())
	{
		Weapon->SetSkeletalMesh(RF.Object);
	}
	Weapon->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	Scope->SetupAttachment(Weapon);
	Scope->SetRelativeLocationAndRotation(FVector(0.f, -1.6f, 0.f), FRotator(0.f, 180.f, 0.f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SC(TEXT("StaticMesh'/Game/Gun/Scope.Scope'"));
	if (SC.Succeeded())
	{
		Scope->SetStaticMesh(SC.Object);
	}

	Weapon->SetCollisionProfileName(TEXT("NoCollision"));
	Scope->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void AHOPEWeapon::BeginPlay()
{
	Super::BeginPlay();
}


