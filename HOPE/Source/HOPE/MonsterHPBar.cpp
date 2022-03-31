// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterHPBar.h"
#include "Components/ProgressBar.h"

//몬스터 HPBar 업데이트
void UMonsterHPBar::UpdateHP(float CurrentHP, float MaxHP)
{
	HPBar->SetPercent(CurrentHP / MaxHP);
}
