// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "HOPECharacter.generated.h"

UCLASS()
class HOPE_API AHOPECharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHOPECharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void MoveForward(float Value);
	
	UFUNCTION()
	void MoveRight(float Value);

	UFUNCTION(BlueprintCallable)
	void Dash();

	UFUNCTION()
	void Fire();

	UFUNCTION(BlueprintCallable)
	void Shoot();

	UFUNCTION()
	void ShootEnd();

	UFUNCTION()
	void Aiming();

	UFUNCTION()
	void AimingEnd();

	UFUNCTION()
	void Run();

	UFUNCTION()
	void RunEnd();

	UFUNCTION(BlueprintCallable)
	void Reload();

	UFUNCTION(BlueprintCallable)
	void Death();

	UFUNCTION()
	void UpdateAmmo();

	UFUNCTION()
	void UpdateHP();

	UFUNCTION()
	void UpdateCrosshair(float Transparency);

	UFUNCTION()
	void UpdateKatanaLongTime();

	UFUNCTION()
	void SeeKatanaLongTime(float Transparency);

	UFUNCTION()
	void UpdateKatanaCooltime();

	UFUNCTION()
	void UpdateDashCooltime();

	UFUNCTION()
	void UpdateAmmoIcon(float Normal, float Fire, float Lightning);

	UFUNCTION()
	void UpdateAmmoCooltime();

	UFUNCTION()
	void OnKatana();

	UFUNCTION()
	void OnRifle();

	UFUNCTION()
	void KatanaSlash();

	UFUNCTION()
	void Defense();

	UFUNCTION()
	void DefenseEnd();

	UFUNCTION()
	void BlockOn();

	UFUNCTION()
	void SpawnSlash();

	UFUNCTION(BlueprintCallable)
	void ChangeAmmo();

	UFUNCTION()
	void AutoHeal();

	UFUNCTION()
	void OnSkillTooltip();

	UFUNCTION()
	void OffSkillTooltip();

public:
	UPROPERTY(BlueprintReadOnly)
	int32 AmmoCount = 40;
	int32 MaxAmmoCount = 40;

	float MaxHP = 1000.f;
	float CurrentHP = 1000.f;
	float AutoHealAmount= 5.f;

	float KatanaRemainingTime = 10.f;
	float KatanaLongTime = 10.f;

	float KatanaCooltime = 20.f;
	float DashCooltime = 2.f;
	float AmmoCooltime = 3.f;

	float CountKatanaCooltime = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float CountDashCooltime = 0.f;

	UPROPERTY(BlueprintReadOnly)
	float CountAmmoCooltime = 0.f;

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	UPROPERTY()
	class UHOPEAnimInstance* AnimInstance;

	UPROPERTY()
	class ARifleProjectile* RifleProjectile;

	UPROPERTY()
	int32 AttackIndex = 0;

	UPROPERTY()
	FTimerHandle HPAutoHealTimerHandle;

	UPROPERTY()
	FTimerHandle KatanaLongTimerHandle;

	UPROPERTY()
	FTimerHandle WaitTimerHandle;

	UPROPERTY()
	FTimerHandle KatanaCooltimeTimerHandle;

	UPROPERTY()
	FTimerHandle DashCooltimeTimerHandle;

	UPROPERTY()
	FTimerHandle AmmoCooltimeTimerHandle;

public:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Gameplay, Meta = (AllowPrivateAccess = true))
	bool IsAiming = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Gameplay, Meta = (AllowPrivateAccess = true))
	bool IsAttacking = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Gameplay, Meta = (AllowPrivateAccess = true))
	bool IsRunning = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Gameplay, Meta = (AllowPrivateAccess = true))
	bool IsReloading = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Gameplay, Meta = (AllowPrivateAccess = true))
	bool IsDeath = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Gameplay, Meta = (AllowPrivateAccess = true))
	bool IsOnKatana = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Gameplay, Meta = (AllowPrivateAccess = true))
	bool OnDash = false;

	UPROPERTY()
	bool NormalAmmo = true;

	UPROPERTY()
	bool FireAmmo = false;

	UPROPERTY()
	bool LightningAmmo = false;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ARifleProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ARifleProjectile> FireProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ARifleProjectile> LightningProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ASlashProjectile> SlashProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CAM", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CAM", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	UStaticMeshComponent* Katana;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	USkeletalMeshComponent* Weapon;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	UStaticMeshComponent* Scope;
};