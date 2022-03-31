// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "HOPEAIController.generated.h"

/**
 * 
 */
UCLASS()
class HOPE_API AHOPEAIController : public AAIController
{
	GENERATED_BODY()

public:
	AHOPEAIController();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	UPROPERTY()
	class UBehaviorTree* BehaviorTree;

	UPROPERTY()
	class UBlackboardData* BlackboardData;
};
