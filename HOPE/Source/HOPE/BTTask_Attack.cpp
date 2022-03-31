// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "BullMechMonster.h"
#include "HOPEAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Monster = Cast<ABullMechMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (Monster == nullptr)
		return EBTNodeResult::Failed;

	Monster->Attack();
	IsAttacking = true;

	Monster->OnAttackEnd.AddLambda([this]() //박치기 공격이 끝났다는 델리게이트
	{
		IsAttacking = false;//Tick에 의해 FinishLatentTask 실행
	});

	return Result;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (IsAttacking == false)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
