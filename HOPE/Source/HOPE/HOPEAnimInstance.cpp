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
	if (!CanKatanaAttack) //�ҵ� ��� ��ų ������ �ҵ� ��� �Ϲ� ���� �Ұ�
		return;

	if (!Montage_IsPlaying(KatanaAttack))
	{
		Montage_Play(KatanaAttack, 1.f);
	}
}

void UHOPEAnimInstance::PlaySlashMontage()
{
	if (!CanKatanaAttack) //�ҵ� ��� ��ų ������ �ҵ� ��� ��ų �ߵ� �Ұ�
		return;

	if (!Montage_IsPlaying(Slash) && !Montage_IsPlaying(InKatana))
	{
		Montage_Play(Slash, 1.f);

		CanKatanaAttack = false; //��ų �ߺ� ���� �ߵ� ����
		Char = Cast<AHOPECharacter>(TryGetPawnOwner());
		Char->DefenseEnd();
		Char->SeeKatanaLongTime(0.f); //�ҵ� ��� ���ӽð� ProgressBar ����ȭ
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
		CanKatanaAttack = true; //��ų ���� ���� ����
	}
}

void UHOPEAnimInstance::PlayEndKatanaMontage()
{
	if (!Montage_IsPlaying(EndKatana)&&!Montage_IsPlaying(Slash))
	{
		Montage_Play(EndKatana, 1.f);
		CanKatanaAttack = false;

		Char = Cast<AHOPECharacter>(TryGetPawnOwner());
		Char->SeeKatanaLongTime(0.f); //�ҵ� ��� ���ӽð� ProgressBar ����ȭ
	}
}

void UHOPEAnimInstance::JumpToSection(int32 SectionIndex)
{
	FName Name = GetAttackMontageName(SectionIndex); //Section Index �ѹ� ����
	Montage_JumpToSection(Name, KatanaAttack); //�ҵ� ��� �Ϲ� ���� Montage Section ����
}

FName UHOPEAnimInstance::GetAttackMontageName(int32 SectionIndex)
{
	return FName(FString::Printf(TEXT("Attack%d"), SectionIndex));
}

//��� Montage �ݵ��� ���� Notify ��ġ
void UHOPEAnimInstance::AnimNotify_AimFire()
{
	Char = Cast<AHOPECharacter>(TryGetPawnOwner());
	Char -> Fire(); //�Ѿ� ����
}

//���� Montage���� źâ�� �ѿ� �����Ǵ� ������ Notify ��ġ
void UHOPEAnimInstance::AnimNotify_CompleteReload()
{
	Char = Cast<AHOPECharacter>(TryGetPawnOwner());
	Char->IsReloading = false;
	Char->AmmoCount = Char->MaxAmmoCount;
	Char->UpdateAmmo(); //AmmoText ������Ʈ
}

//�ҵ� ��� �Ϲݰ����� ������ ������ Notify ��ġ. �ҵ� ��� �Ϲݰ��� �ߺ����� ���� ����
void UHOPEAnimInstance::AnimNotify_EndAttack()
{
	Char = Cast<AHOPECharacter>(TryGetPawnOwner());
	Char->IsAttacking = false;
}

//���� ���� �߰� ������ ���� Notify ��ġ
void UHOPEAnimInstance::AnimNotify_Slash()
{
	Char = Cast<AHOPECharacter>(TryGetPawnOwner());
	Char->SpawnSlash(); //�˱� ����
}

void UHOPEAnimInstance::AnimNotify_BlockOn()
{
	Char = Cast<AHOPECharacter>(TryGetPawnOwner());
	Char->BlockOn();
}

//�ҵ� ��� ��ų �ߵ� �� EndKatanaMontage ����
void UHOPEAnimInstance::AnimNotify_EndKatana()
{
	PlayEndKatanaMontage();
}

//EndKatanaMontage �� ���� ���� ����
void UHOPEAnimInstance::AnimNotify_ChangeWeapon()
{
	Char = Cast<AHOPECharacter>(TryGetPawnOwner());
	Char->OnRifle();
}