// Fill out your copyright notice in the Description page of Project Settings.


#include "HOPEAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

AHOPEAIController::AHOPEAIController()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("BehaviorTree'/Game/Blueprints/AI/BT_BullMech.BT_BullMech'"));
	if (BT.Succeeded())
	{
		BehaviorTree = BT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BD(TEXT("BlackboardData'/Game/Blueprints/AI/BB_BullMech.BB_BullMech'"));
	if (BD.Succeeded())
	{
		BlackboardData = BD.Object;
	}
}

void AHOPEAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UseBlackboard(BlackboardData, Blackboard))
	{
		if (RunBehaviorTree(BehaviorTree))
		{

		}
	}
}

void AHOPEAIController::OnUnPossess()
{
	Super::OnUnPossess();
	
}