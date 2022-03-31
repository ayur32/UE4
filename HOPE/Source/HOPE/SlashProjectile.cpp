// Fill out your copyright notice in the Description page of Project Settings.


#include "SlashProjectile.h"
#include "Engine/EngineTypes.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASlashProjectile::ASlashProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InitialLifeSpan = 1.5f;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(1.0f);
	RootComponent = CollisionComponent;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 10000.0f;
	ProjectileMovementComponent->MaxSpeed = 10000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;

	Effect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect"));
	Effect->SetupAttachment(CollisionComponent);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> SL(TEXT("NiagaraSystem'/Game/AdvancedNiagaraVFX/Particles/Slash/NS_Slash6.NS_Slash6'"));
	if (SL.Succeeded())
	{
		Effect->SetAsset(SL.Object);
	}

	Collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	Collision->SetupAttachment(Effect);
	Collision->SetRelativeRotation(FRotator(90.f, 0.f, 90.f));
	Collision->InitCapsuleSize(1.f, 250.f);
	Collision->SetCollisionProfileName(TEXT("SlashProjectile"));
	CollisionComponent->SetCollisionProfileName(TEXT("SlashProjectile"));
}

// Called when the game starts or when spawned
void ASlashProjectile::BeginPlay()
{
	Super::BeginPlay();	

	Effect->SetWorldRotation(FRotator(0.f, 0.f, FMath::RandRange(-90.f, 90.f))); //검기의 기울기 랜덤화
}

// Called every frame
void ASlashProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASlashProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

