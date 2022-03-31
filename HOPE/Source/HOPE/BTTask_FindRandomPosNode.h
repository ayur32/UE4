// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindRandomPosNode.generated.h"

/**
 * 
 */
UCLASS()
class HOPE_API UBTTask_FindRandomPosNode : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FindRandomPosNode();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};