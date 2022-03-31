// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SearchTarget.h"
#include "HOPEAIController.h"
#include "HOPECharacter.h"
#include "BullMechMonster.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_SearchTarget::UBTService_SearchTarget()
{
	NodeName = TEXT("SearchTarget");
	Interval = 1.f;
}



void UBTService_SearchTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr)
		return;

	UWorld* World = CurrentPawn->GetWorld();
	FVector Center = CurrentPawn->GetActorLocation();
	float SearchRadius = 1000.f; //감지 범위

	if (World == nullptr)
		return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams(NAME_None, false, CurrentPawn);

	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(SearchRadius),
		QueryParams);

	if (bResult)
	{
		for (auto& OverlapResult : OverlapResults)
		{
			AHOPECharacter* Character = Cast<AHOPECharacter>(OverlapResult.GetActor());
			if (Character && Character->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), Character); //캐릭터 감지 후 블랙보드의 Target Object에 저장

				DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Green, false, 0.2f); //타겟 감지 성공 -> 초록 구

				auto Monster = Cast<ABullMechMonster>(OwnerComp.GetAIOwner()->GetPawn());
				Monster->Targeting(); //이동속도 증가함수 호출

				return;
			}
		}

		DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Red, false, 0.2f); //타겟 감지 실패 -> 빨간 구
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), nullptr);

		DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Red, false, 0.2f); //타겟 감지 실패 -> 빨간 구
	}
}