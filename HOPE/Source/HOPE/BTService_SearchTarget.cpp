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
	float SearchRadius = 1000.f; //���� ����

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
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), Character); //ĳ���� ���� �� �������� Target Object�� ����

				DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Green, false, 0.2f); //Ÿ�� ���� ���� -> �ʷ� ��

				auto Monster = Cast<ABullMechMonster>(OwnerComp.GetAIOwner()->GetPawn());
				Monster->Targeting(); //�̵��ӵ� �����Լ� ȣ��

				return;
			}
		}

		DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Red, false, 0.2f); //Ÿ�� ���� ���� -> ���� ��
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), nullptr);

		DrawDebugSphere(World, Center, SearchRadius, 16, FColor::Red, false, 0.2f); //Ÿ�� ���� ���� -> ���� ��
	}
}