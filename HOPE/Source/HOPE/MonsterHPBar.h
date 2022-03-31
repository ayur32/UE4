// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MonsterHPBar.generated.h"

/**
 * 
 */
UCLASS()
class HOPE_API UMonsterHPBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void UpdateHP(float CurrentHP, float MaxHP);

private:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HPBar;
};
