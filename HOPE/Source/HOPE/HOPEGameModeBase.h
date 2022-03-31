// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HOPEGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class HOPE_API AHOPEGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AHOPEGameModeBase();

	UPROPERTY()
	TSubclassOf<UUserWidget> HUD_Class;

	UPROPERTY()
	UUserWidget* CurrentWidget;
};
