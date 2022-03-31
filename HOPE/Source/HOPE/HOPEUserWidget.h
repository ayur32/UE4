// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HOPEUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class HOPE_API UHOPEUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AmmoText;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HPBar;

	UPROPERTY(meta = (BindWidget))
	class UImage* CrossHair;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* KatanaLongTime;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* KatanaLongTimeText;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* KatanaSkillIcon;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* KatanaCooltimeText;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* DashIcon;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DashCooltimeText;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* NormalAmmoIcon;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* FireAmmoIcon;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* LightningAmmoIcon;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AmmoCooltimeText;

	UPROPERTY(meta = (BindWidget))
	class UImage* SkillTooltip;
};
