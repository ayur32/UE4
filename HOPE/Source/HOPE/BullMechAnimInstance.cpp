// Fill out your copyright notice in the Description page of Project Settings.


#include "BullMechAnimInstance.h"
#include "BullMechMonster.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"

UBullMechAnimInstance::UBullMechAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AK(TEXT("AnimMontage'/Game/BullMech/ANIMATIONS/BullMech_Attack_Montage.BullMech_Attack_Montage'"));
	if (AK.Succeeded())
	{
		FrontAttack = AK.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DEATH(TEXT("AnimMontage'/Game/BullMech/ANIMATIONS/BullMech_Death_Montage.BullMech_Death_Montage'"));
	if (DEATH.Succeeded())
	{
		Death = DEATH.Object;
	}
}

void UBullMechAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();

		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
}

void UBullMechAnimInstance::PlayFrontAttackMontage()
{
	if (!Montage_IsPlaying(FrontAttack))
	{
		Montage_Play(FrontAttack, 1.f);
	}
}

void UBullMechAnimInstance::PlayDeathMontage()
{
	if (!Montage_IsPlaying(Death))
	{
		Montage_Play(Death, 1.f);
	}
}

//공격모션 시작과 끝의 노티파이를 통해 Collision 설정
void UBullMechAnimInstance::AnimNotify_AttackStart()
{
	Monster = Cast<ABullMechMonster>(TryGetPawnOwner());
	Monster->AttackStart();
}

//공격모션 시작과 끝의 노티파이를 통해 Collision 설정
void UBullMechAnimInstance::AnimNotify_AttackEnd()
{
	Monster = Cast<ABullMechMonster>(TryGetPawnOwner());
	Monster->AttackEnd();
}

