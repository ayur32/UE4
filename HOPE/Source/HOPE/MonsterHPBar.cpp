// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterHPBar.h"
#include "Components/ProgressBar.h"

//���� HPBar ������Ʈ
void UMonsterHPBar::UpdateHP(float CurrentHP, float MaxHP)
{
	HPBar->SetPercent(CurrentHP / MaxHP);
}
