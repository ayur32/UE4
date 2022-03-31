// Fill out your copyright notice in the Description page of Project Settings.


#include "BullMechMonster.h"
#include "HOPEAIController.h"
#include "BullMechAnimInstance.h"
#include "MonsterHPBar.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABullMechMonster::ABullMechMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -88.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FP(TEXT("SkeletalMesh'/Game/BullMech/MESH/BULLMECH_SK.BULLMECH_SK'"));
	if (FP.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(FP.Object);
	}

	AIControllerClass = AHOPEAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Head = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HEAD"));
	Leg = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LEG"));
	HPBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBAR"));

	HPBar->SetupAttachment(GetMesh());
	HPBar->SetRelativeLocation(FVector(0.f, 0.f, 400.f));
	HPBar->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> HP_BAR(TEXT("WidgetBlueprint'/Game/Blueprints/BP_HPBar.BP_HPBar_C'"));
	if (HP_BAR.Succeeded())
	{
		HPBar->SetWidgetClass(HP_BAR.Class);
		HPBar->SetDrawSize(FVector2D(150.f, 50.f));
	}

	FName HeadSocket(TEXT("head_socket")); //��ġ�� ���� CapsuleComponent ����
	if (GetMesh()->DoesSocketExist(HeadSocket))
	{
		Head->SetupAttachment(GetMesh(), HeadSocket);
	}
	FName LegSocket(TEXT("leg_socket")); //���� ���� CapsuleComponent ����
	if (GetMesh()->DoesSocketExist(LegSocket))
	{
		Leg->SetupAttachment(GetMesh(), LegSocket);
	}

	GetMesh()->SetCollisionProfileName(TEXT("Pawn"));
	Head->SetCollisionProfileName(TEXT("NoCollision"));
	Leg->SetCollisionProfileName(TEXT("MonsterAttack"));
}

// Called when the game starts or when spawned
void ABullMechMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullMechMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABullMechMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


float ABullMechMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// PointDamage �ޱ�.
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
		if (0 == (PointDamageEvent->HitInfo.BoneName).Compare(FName(TEXT("Head"))))
		{
			Damage *= 5; // ���� ������ Head��, ������ 5��.
		}
	}

	CurrentHP -= Damage;
	UpdateHP(); //�ǰݽ� HPBar ������Ʈ

	UE_LOG(LogClass, Log, TEXT("%.3f"), CurrentHP);

	if (CurrentHP <= 0.f)
		IsDeath = true;

	return Damage;
}

//Target�� �������� �ӵ��� ���ӵ��� ������
void ABullMechMonster::Targeting()
{
	GetCharacterMovement()->MaxWalkSpeed = 1600.f;
	GetCharacterMovement()->MaxAcceleration = 1600.f;
}

void ABullMechMonster::Attack() //��ġ�� ����
{
	AnimInstance = Cast<UBullMechAnimInstance>(GetMesh()->GetAnimInstance());
	AnimInstance->PlayFrontAttackMontage();
	OnAttackEnd.Broadcast(); //BTTask_Attack�� ��������Ʈ
}

void ABullMechMonster::AttackStart()
{
	Head->SetCollisionProfileName(TEXT("MonsterAttack"));
}

void ABullMechMonster::AttackEnd()
{
	Head->SetCollisionProfileName(TEXT("NoCollision"));
}

void ABullMechMonster::Death()
{
	AnimInstance = Cast<UBullMechAnimInstance>(GetMesh()->GetAnimInstance());
	AnimInstance->PlayDeathMontage();
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	IsDeath = true;
}

//MonsterHPBar���� HPBar ������Ʈ �Լ� ȣ��
void ABullMechMonster::UpdateHP()
{
	auto HPWidget = Cast<UMonsterHPBar>(HPBar->GetUserWidgetObject());
	HPWidget->UpdateHP(CurrentHP, MaxHP);
}