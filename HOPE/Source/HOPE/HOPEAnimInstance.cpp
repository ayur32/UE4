// Fill out your copyright notice in the Description page of Project Settings.


#include "HOPEAnimInstance.h"
#include "HOPECharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"


UHOPEAnimInstance::UHOPEAnimInstance()
{
	CurrentPawnSpeed = 0.f;
	IsInAir = false;
	IsAiming = false;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AIM_FIRE(TEXT("AnimMontage'/Game/FPPCode-39/Character/Arms/Arms_Animations/Anim_Montages/Arms_Aim_Fire_Montage.Arms_Aim_Fire_Montage'"));
	if (AIM_FIRE.Succeeded())
	{
		AimFire = AIM_FIRE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> RL(TEXT("AnimMontage'/Game/FPPCode-39/Character/Arms/Arms_Animations/Anim_Montages/Arms_Reload_Montage.Arms_Reload_Montage'"));
	if (RL.Succeeded())
	{
		Reload = RL.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> KA(TEXT("AnimMontage'/Game/FPPCode-39/Character/Arms/Arms_Animations/Anim_Montages/KatanaAttack_Montage.KatanaAttack_Montage'"));
	if (KA.Succeeded())
	{
		KatanaAttack = KA.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SL(TEXT("AnimMontage'/Game/FPPCode-39/Character/Arms/Arms_Animations/Anim_Montages/Slash_Montage.Slash_Montage'"));
	if (SL.Succeeded())
	{
		Slash = SL.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DF(TEXT("AnimMontage'/Game/FPPCode-39/Character/Arms/Arms_Animations/Anim_Montages/Defense_Montage.Defense_Montage'"));
	if (DF.Succeeded())
	{
		Defense = DF.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> IK(TEXT("AnimMontage'/Game/FPPCode-39/Character/Arms/Arms_Animations/Anim_Montages/InKatana_Montage.InKatana_Montage'"));
	if (IK.Succeeded())
	{
		InKatana = IK.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> EK(TEXT("AnimMontage'/Game/FPPCode-39/Character/Arms/Arms_Animations/Anim_Montages/EndKatana_Montage.EndKatana_Montage'"));
	if (EK.Succeeded())
	{
		EndKatana = EK.Object;
	}
}

void UHOPEAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
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

void UHOPEAnimInstance::PlayAimMontage()
{
	if (!Montage_IsPlaying(AimFire))
	{
		Montage_Play(AimFire, 1.f);
	}
}
void UHOPEAnimInstance::PlayReloadMontage()
{
	if (!Montage_IsPlaying(Reload))
	{
		Char->UpdateCrosshair(1.f);
		Montage_Play(Reload, 1.f);
	}
}

void UHOPEAnimInstance::PlayStopReloadMontage()
{
	if (Montage_IsPlaying(Reload))
	{
		Montage_Stop(0.1f, Reload);
	}
}

void UHOPEAnimInstance::PlayKatanaAttackMontage()
{
	if (!CanKatanaAttack) //소드 모드 스킬 시전중 소드 모드 일반 공격 불가
		return;

	if (!Montage_IsPlaying(KatanaAttack))
	{
		Montage_Play(KatanaAttack, 1.f);
	}
}

void UHOPEAnimInstance::PlaySlashMontage()
{
	if (!CanKatanaAttack) //소드 모드 스킬 시전중 소드 모드 스킬 발동 불가
		return;

	if (!Montage_IsPlaying(Slash) && !Montage_IsPlaying(InKatana))
	{
		Montage_Play(Slash, 1.f);

		CanKatanaAttack = false; //스킬 중복 실행 발동 방지
		Char = Cast<AHOPECharacter>(TryGetPawnOwner());
		Char->DefenseEnd();
		Char->SeeKatanaLongTime(0.f); //소드 모드 지속시간 ProgressBar 투명화
	}
}

void UHOPEAnimInstance::PlayDefenseMontage()
{
	if (!CanKatanaAttack)
		return;

	if (!Montage_IsPlaying(Defense))
	{
		Montage_Play(Defense, 1.f);
	}
}

void UHOPEAnimInstance::PlayStopDefenseMontage()
{
	if (Montage_IsPlaying(Defense))
	{
		Montage_Stop(0.1f, Defense);
	}
}

void UHOPEAnimInstance::PlayInKatanaMontage()
{
	if (!Montage_IsPlaying(InKatana))
	{
		Montage_Play(InKatana, 1.f);
		CanKatanaAttack = true; //스킬 공격 실행 가능
	}
}

void UHOPEAnimInstance::PlayEndKatanaMontage()
{
	if (!Montage_IsPlaying(EndKatana)&&!Montage_IsPlaying(Slash))
	{
		Montage_Play(EndKatana, 1.f);
		CanKatanaAttack = false;

		Char = Cast<AHOPECharacter>(TryGetPawnOwner());
		Char->SeeKatanaLongTime(0.f); //소드 모드 지속시간 ProgressBar 투명화
	}
}

void UHOPEAnimInstance::JumpToSection(int32 SectionIndex)
{
	FName Name = GetAttackMontageName(SectionIndex); //Section Index 넘버 설정
	Montage_JumpToSection(Name, KatanaAttack); //소드 모드 일반 공격 Montage Section 점프
}

FName UHOPEAnimInstance::GetAttackMontageName(int32 SectionIndex)
{
	return FName(FString::Printf(TEXT("Attack%d"), SectionIndex));
}

//사격 Montage 반동에 맞춰 Notify 배치
void UHOPEAnimInstance::AnimNotify_AimFire()
{
	Char = Cast<AHOPECharacter>(TryGetPawnOwner());
	Char -> Fire(); //총알 스폰
}

//장전 Montage에서 탄창이 총에 장착되는 순간에 Notify 배치
void UHOPEAnimInstance::AnimNotify_CompleteReload()
{
	Char = Cast<AHOPECharacter>(TryGetPawnOwner());
	Char->IsReloading = false;
	Char->AmmoCount = Char->MaxAmmoCount;
	Char->UpdateAmmo(); //AmmoText 업데이트
}

//소드 모드 일반공격이 끝나는 시점에 Notify 배치. 소드 모드 일반공격 중복실행 방지 역할
void UHOPEAnimInstance::AnimNotify_EndAttack()
{
	Char = Cast<AHOPECharacter>(TryGetPawnOwner());
	Char->IsAttacking = false;
}

//베기 동작 중간 지점에 맞춰 Notify 배치
void UHOPEAnimInstance::AnimNotify_Slash()
{
	Char = Cast<AHOPECharacter>(TryGetPawnOwner());
	Char->SpawnSlash(); //검귀 스폰
}

void UHOPEAnimInstance::AnimNotify_BlockOn()
{
	Char = Cast<AHOPECharacter>(TryGetPawnOwner());
	Char->BlockOn();
}

//소드 모드 스킬 발동 후 EndKatanaMontage 실행
void UHOPEAnimInstance::AnimNotify_EndKatana()
{
	PlayEndKatanaMontage();
}

//EndKatanaMontage 후 소총 모드로 변경
void UHOPEAnimInstance::AnimNotify_ChangeWeapon()
{
	Char = Cast<AHOPECharacter>(TryGetPawnOwner());
	Char->OnRifle();
}