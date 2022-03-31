// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "BullMechMonster.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEnd);

UCLASS()
class HOPE_API ABullMechMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABullMechMonster();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttackComponent)
	UCapsuleComponent* Head;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttackComponent)
	UCapsuleComponent* Leg;

	void Targeting();

	void Attack();

	void AttackStart();

	void AttackEnd();

	UFUNCTION(BlueprintCallable)
	void Death();

	void UpdateHP();

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = State, Meta = (AllowPrivateAccess = true))
	bool IsDeath = false;

	FOnAttackEnd OnAttackEnd;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MaxHP = 15000.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrentHP = 15000.f;

	bool IsTargeting = false;

	UPROPERTY()
	float AttackRange = 200.f;

	UPROPERTY()
	float AttackRadius = 50.f;
	
public:
	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* HPBar;

private:
	UPROPERTY()
	class UBullMechAnimInstance* AnimInstance;
};
