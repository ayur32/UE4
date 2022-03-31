// Fill out your copyright notice in the Description page of Project Settings.


#include "HOPECharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "RifleProjectile.h"
#include "HOPEAnimInstance.h"
#include "SlashProjectile.h"
#include "HOPEUserWidget.h"
#include "HOPEGameModeBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"

// Sets default values
AHOPECharacter::AHOPECharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = true;

	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	SpringArm->SetupAttachment(RootComponent);
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Camera->SetupAttachment(SpringArm);
	Camera->FieldOfView = 90.f;

	SpringArm->TargetArmLength = 60.f;

	SpringArm->SetRelativeLocation(FVector(75.f, -1.53f, 79.15f));
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -88.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FP(TEXT("SkeletalMesh'/Game/FPPCode-39/Character/Mesh/SK_Mannequin_Arms.SK_Mannequin_Arms'"));
	if (FP.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(FP.Object);
	}

	//총기+스코프 소켓에 스폰
	FName WeaponSocket(TEXT("hand_r_socket"));
	if (GetMesh()->DoesSocketExist(WeaponSocket))
	{
		Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
		Weapon->SetupAttachment(GetMesh());
		Scope = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SCOPE"));
		Scope->SetupAttachment(Weapon);

		static ConstructorHelpers::FObjectFinder<USkeletalMesh> RF(TEXT("SkeletalMesh'/Game/Gun/Gun.Gun'"));
		if (RF.Succeeded())
		{
			Weapon->SetSkeletalMesh(RF.Object);
		}
		static ConstructorHelpers::FObjectFinder<UStaticMesh> SC(TEXT("StaticMesh'/Game/Gun/Scope.Scope'"));
		if (SC.Succeeded())
		{
			Scope->SetStaticMesh(SC.Object);
		}

		Weapon->SetupAttachment(GetMesh(), WeaponSocket);

		Weapon->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
		Scope->SetRelativeLocationAndRotation(FVector(0.f, -1.6f, 0.f), FRotator(0.f, 180.f, 0.f));
	}
	
	//소드 소켓에 스폰
	FName KATANASocket(TEXT("hand_r_socketKTN"));
	if (GetMesh()->DoesSocketExist(KATANASocket))
	{
		Katana = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KATANA"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh> KTN(TEXT("StaticMesh'/Game/FPP_Katana/Katana/SM_Katana.SM_Katana'"));
		if (KTN.Succeeded())
		{
			Katana->SetStaticMesh(KTN.Object);
		}
		Katana->SetupAttachment(GetMesh(), KATANASocket);
	}

	Katana->SetOwnerNoSee(true);

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->GravityScale = 1.7f;

	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	Weapon->SetCollisionProfileName(TEXT("NoCollision"));
	Scope->SetCollisionProfileName(TEXT("NoCollision"));
	Katana->SetCollisionProfileName(TEXT("NoCollision"));

	//HUD 초기 투명도 설정
	AHOPEGameModeBase* GameMode = Cast<AHOPEGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		UHOPEUserWidget* MyHUD = Cast<UHOPEUserWidget>(GameMode->CurrentWidget);
		if (MyHUD)
		{
			MyHUD->KatanaLongTime->SetRenderOpacity(0.f);
			MyHUD->KatanaLongTimeText->SetRenderOpacity(0.f);
			MyHUD->KatanaCooltimeText->SetRenderOpacity(0.f);
			MyHUD->DashCooltimeText->SetRenderOpacity(0.f);

			MyHUD->FireAmmoIcon->SetRenderOpacity(0.f);
			MyHUD->LightningAmmoIcon->SetRenderOpacity(0.f);
			MyHUD->AmmoCooltimeText->SetRenderOpacity(0.f);

			MyHUD->SkillTooltip->SetRenderOpacity(0.f);
		}
	}
}

// Called when the game starts or when spawned
void AHOPECharacter::BeginPlay()
{
	Super::BeginPlay();

	//1초마다 자동회복 함수 콜
	GetWorldTimerManager().SetTimer(HPAutoHealTimerHandle, this, &AHOPECharacter::AutoHeal, 1.f, true, 0.f);
}

// Called every frame
void AHOPECharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	AnimInstance = Cast<UHOPEAnimInstance>(GetMesh()->GetAnimInstance());

	//AnimInstance의 상태 boolean과 연동
	if (AnimInstance)
	{
		AnimInstance->IsAiming = IsAiming;
		AnimInstance->IsAttacking = IsAttacking;
		AnimInstance->IsRunning = IsRunning;
		AnimInstance->IsOnDash = OnDash;
		AnimInstance->IsReloading = IsReloading;
		AnimInstance->IsOnKATANA = IsOnKatana;
	}
	
	//캐릭터 상태에 따른 속도 설정
	if (OnDash)
	{
		GetCharacterMovement()->MaxWalkSpeed = 50000.f;
	}
	else
	{

		if (IsAiming)
		{
			GetCharacterMovement()->MaxWalkSpeed = 300.f;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = 700.f;
		}

		if (IsRunning || IsOnKatana)
		{
			GetCharacterMovement()->MaxWalkSpeed = 1100.f;
		}

		if (IsOnKatana && IsRunning)
		{
			GetCharacterMovement()->MaxWalkSpeed = 1500.f;
			GetCharacterMovement()->JumpZVelocity = 800.f;
		}
	}
}

// Called to bind functionality to input
void AHOPECharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AHOPECharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AHOPECharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AHOPECharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AHOPECharacter::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Dash", EInputEvent::IE_Pressed, this, &AHOPECharacter::Dash);
	PlayerInputComponent->BindAction("Shoot", EInputEvent::IE_Pressed, this, &AHOPECharacter::Shoot);
	PlayerInputComponent->BindAction("Shoot", EInputEvent::IE_Released, this, &AHOPECharacter::ShootEnd);
	PlayerInputComponent->BindAction("Aiming", EInputEvent::IE_Pressed, this, &AHOPECharacter::Aiming);
	PlayerInputComponent->BindAction("Aiming", EInputEvent::IE_Released, this, &AHOPECharacter::AimingEnd);
	PlayerInputComponent->BindAction("Aiming", EInputEvent::IE_Pressed, this, &AHOPECharacter::Defense);
	PlayerInputComponent->BindAction("Aiming", EInputEvent::IE_Released, this, &AHOPECharacter::DefenseEnd);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Pressed, this, &AHOPECharacter::Run);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Released, this, &AHOPECharacter::RunEnd);
	PlayerInputComponent->BindAction("Reload", EInputEvent::IE_Pressed, this, &AHOPECharacter::Reload);
	PlayerInputComponent->BindAction("ChangeWeapon", EInputEvent::IE_Pressed, this, &AHOPECharacter::OnKatana);
	PlayerInputComponent->BindAction("ChangeWeapon", EInputEvent::IE_Pressed, this, &AHOPECharacter::KatanaSlash);
	PlayerInputComponent->BindAction("ChangeAmmo", EInputEvent::IE_Pressed, this, &AHOPECharacter::ChangeAmmo);
	PlayerInputComponent->BindAction("SkillTooltip", EInputEvent::IE_Pressed, this, &AHOPECharacter::OnSkillTooltip);
	PlayerInputComponent->BindAction("SkillTooltip", EInputEvent::IE_Released, this, &AHOPECharacter::OffSkillTooltip);
}

void AHOPECharacter::MoveForward(float Value)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AHOPECharacter::MoveRight(float Value)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void AHOPECharacter::Dash()
{
	if (CountDashCooltime > 0)
		return;

	if (GetMovementComponent()->IsFalling())
		return;

	UE_LOG(LogTemp, Log, TEXT("DASH"))

	OnDash = true;
	GetCharacterMovement()->MaxAcceleration = 50000.f;

	//0.2초 후에 OnDash를 fasle로 돌려놓음
	FTimerHandle DashTimerHandle;
	float LongTime = 0.2f;
	GetWorldTimerManager().SetTimer(DashTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			
			OnDash = false;
			GetCharacterMovement()->MaxAcceleration = 2048.f;

			GetWorldTimerManager().ClearTimer(DashTimerHandle);

			UE_LOG(LogTemp, Log, TEXT("DASHEND"))

		}), LongTime, false);

	GetWorldTimerManager().SetTimer(DashCooltimeTimerHandle, this, &AHOPECharacter::UpdateDashCooltime, 0.01f, true, 0.f);
}

void AHOPECharacter::Fire()
{
	AmmoCount--;

	UpdateAmmo();

	if (AmmoCount == 0)
		IsAttacking = false;
	
	if (ProjectileClass)
	{
		//SpringArm 위치에 총알을 스폰. 발사 방향은 CameraRotation.
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);
		
		FVector SpawnLocation = SpringArm->GetComponentLocation();

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();
			
			//상태에 따라 3종류의 총알을 선택하여 스폰
			if (NormalAmmo)
			{
				ARifleProjectile* Projectile = World->SpawnActor<ARifleProjectile>(ProjectileClass, SpawnLocation, CameraRotation, SpawnParams);
				if (Projectile)
				{
					FVector LaunchDirection = CameraRotation.Vector();
					Projectile->FireInDirection(LaunchDirection);
				}
			}
			if (FireAmmo)
			{
				ARifleProjectile* Projectile = World->SpawnActor<ARifleProjectile>(FireProjectileClass, SpawnLocation, CameraRotation, SpawnParams);
				if (Projectile)
				{
					FVector LaunchDirection = CameraRotation.Vector();
					Projectile->FireInDirection(LaunchDirection);
				}
			}
			if (LightningAmmo)
			{
				ARifleProjectile* Projectile = World->SpawnActor<ARifleProjectile>(LightningProjectileClass, SpawnLocation, CameraRotation, SpawnParams);
				if (Projectile)
				{
					FVector LaunchDirection = CameraRotation.Vector();
					Projectile->FireInDirection(LaunchDirection);
				}
			}
		}
	}
	
	//조준 상태에 따른 반동.
	if (IsAiming)
	{
		AddControllerYawInput(FMath::RandRange(-0.1f, 0.1f)); //좌우반동
	}
	else
	{
		AddControllerPitchInput(FMath::RandRange(-0.6f, -0.2f)); //상하반동
		AddControllerYawInput(FMath::RandRange(-0.3f, 0.3f)); //좌우반동
	}
}

void AHOPECharacter::Shoot()
{
	if (IsReloading)
		return;

	if (IsOnKatana) //소드 모드에서의 공격
	{
		if (IsAttacking)
			return;

		IsAttacking = true;
		DefenseEnd();

		AnimInstance->PlayKatanaAttackMontage();
		
		AnimInstance->JumpToSection(AttackIndex); //공격 모션 Section 구분 및 IndexCount
		AttackIndex = (AttackIndex + 1) % 4;
	}
	else //일반 사격 공격
	{
		if (AmmoCount <= 0)
			return;

		IsAttacking = true;
		IsRunning = false; //달리기 중 공격했을 때 달리는 속도가 유지되는 현상을 방지

		AnimInstance->PlayAimMontage();
	}
}

void AHOPECharacter::ShootEnd()
{
	if (IsOnKatana)
		return;

	IsAttacking = false;	
}

void AHOPECharacter::Aiming()
{
	if (IsReloading) //장전 중 조준 불가능. 단 탄창 장착모션 후에는 노티파이에 의해 IsReloading이 false가 되므로 조준 가능
		return;

	if (IsOnKatana) //소드 모드 중 조준 불가능.
		return;

	IsAiming = true;
	IsRunning = false;

	AnimInstance->PlayStopReloadMontage(); //탄창 장착 후 장전모션 중이라면 장전을 스킵하고 빠르게 조준 가능
	
	UpdateCrosshair(0.f); //크로스헤어 투명화
}


void AHOPECharacter::AimingEnd()
{
	IsAiming = false;

	UpdateCrosshair(1.f); //크로스헤어 가시화
}

void AHOPECharacter::Run()
{
	if (IsAttacking&&!IsOnKatana) //사격 중에는 달리기가 불가능하나 소드 모드에선 공격하면서 달리기 가능
		return;

	if (IsReloading) //장전 중 달리기 불가능. 단 탄창 장착모션 후에는 노티파이에 의해 IsReloading이 false가 되므로 달리기 가능
		return;

	IsRunning = true;
	IsAiming = false; //조준 중 달렸을 때 카메라 줌아웃이 안되는 현상을 방지
	UpdateCrosshair(1.f); //조준 중 달렸을 때 크로스헤어가 생성되지 않는 현상을 방지

	AnimInstance->PlayStopReloadMontage(); //탄창 장착 후 장전모션 중이라면 장전을 스킵하고 빠르게 달리기 가능
}

void AHOPECharacter::RunEnd()
{
	IsRunning = false;;
}

void AHOPECharacter::Reload()
{
	if (AmmoCount == MaxAmmoCount)
		return;

	if (IsOnKatana)
		return;

	if (IsReloading)
		return;

	IsReloading = true;
	IsRunning = false;
	IsAttacking = false;
	IsAiming = false; //달리기, 공격, 조준 중에 장전 가능

	AnimInstance->PlayReloadMontage();
}


float AHOPECharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (ActualDamage > 0.f)
	{
		CurrentHP -= ActualDamage;
		UpdateHP(); //데미지를 입을때마다 체력 동기화

		if (CurrentHP <= 0.f) //죽음처리
		{
			CurrentHP = 0.f;
			Death();
		}
	}
	return ActualDamage;
}

void AHOPECharacter::Death()
{
	if (!IsDeath)
	{
		IsDeath = true;
		UpdateCrosshair(0.f);

		UE_LOG(LogTemp, Log, TEXT("DEATH"))
	}
}

//총알 카운트 HUD Text와 동기화
void AHOPECharacter::UpdateAmmo()
{
	AHOPEGameModeBase* GameMode = Cast<AHOPEGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		UHOPEUserWidget* MyHUD = Cast<UHOPEUserWidget>(GameMode->CurrentWidget);
		if (MyHUD)
		{
			const FString AmmoStr = FString::Printf(TEXT("%01d/%01d"), AmmoCount, MaxAmmoCount);
			MyHUD->AmmoText->SetText(FText::FromString(AmmoStr));
		}
	}
}

//캐릭터 체력 HUD HPBar와 동기화
void AHOPECharacter::UpdateHP()
{
	AHOPEGameModeBase* GameMode = Cast<AHOPEGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		UHOPEUserWidget* MyHUD = Cast<UHOPEUserWidget>(GameMode->CurrentWidget);
		if (MyHUD)
		{
			MyHUD->HPBar->SetPercent(CurrentHP / MaxHP);
		}
	}
}

//크로스헤어 투명도 설정 함수
void AHOPECharacter::UpdateCrosshair(float Transparency)
{
	AHOPEGameModeBase* GameMode = Cast<AHOPEGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		UHOPEUserWidget* MyHUD = Cast<UHOPEUserWidget>(GameMode->CurrentWidget);
		if (MyHUD)
		{
			MyHUD->CrossHair->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, Transparency));
		}
	}
}

//소드 모드 지속시간 HUD ProgressBar, Text 동기화
void AHOPECharacter::UpdateKatanaLongTime()
{
	KatanaRemainingTime -= 0.01f; //소드 모드시 0.01s 마다 호출됨

	AHOPEGameModeBase* GameMode = Cast<AHOPEGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		UHOPEUserWidget* MyHUD = Cast<UHOPEUserWidget>(GameMode->CurrentWidget);
		if (MyHUD)
		{
			MyHUD->KatanaLongTime->SetPercent( KatanaRemainingTime / KatanaLongTime );
			const FString KatanaStr = FString::Printf(TEXT("Remain : %.2f\nPrees Button E for Skill Attack"), KatanaRemainingTime);
			MyHUD->KatanaLongTimeText->SetText(FText::FromString(KatanaStr));
		}
	}

	if (KatanaRemainingTime <= 0.f)
	{
		GetWorldTimerManager().ClearTimer(KatanaLongTimerHandle);
	}
}

//소드 모드 스킬 아이콘 및 쿨타임 Text 투명도
void AHOPECharacter::SeeKatanaLongTime(float Transparency)
{
	AHOPEGameModeBase* GameMode = Cast<AHOPEGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		UHOPEUserWidget* MyHUD = Cast<UHOPEUserWidget>(GameMode->CurrentWidget);
		if (MyHUD)
		{
			MyHUD->KatanaLongTime->SetRenderOpacity(Transparency);
			MyHUD->KatanaLongTimeText->SetRenderOpacity(Transparency);
		}
	}
}

//인게임 소드 모드 쿨타임, HUD 스킬 아이콘(ProgressBar) 및 쿨타임 Text와 동기화
void AHOPECharacter::UpdateKatanaCooltime()
{
	CountKatanaCooltime += 0.01f; //소드 모드시 0.01s 마다 호출됨

	AHOPEGameModeBase* GameMode = Cast<AHOPEGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		UHOPEUserWidget* MyHUD = Cast<UHOPEUserWidget>(GameMode->CurrentWidget);
		if (MyHUD)
		{
			MyHUD->KatanaCooltimeText->SetRenderOpacity(1.f); //쿨타임 Text 가시화
			MyHUD->KatanaSkillIcon->SetPercent( 0.1f + CountKatanaCooltime*0.8 / KatanaCooltime);
			const FString KatanaStr = FString::Printf(TEXT("%.0f"), KatanaCooltime - CountKatanaCooltime);
			MyHUD->KatanaCooltimeText->SetText(FText::FromString(KatanaStr));
		}

		if (CountKatanaCooltime >= KatanaCooltime)
		{
			GetWorldTimerManager().ClearTimer(KatanaCooltimeTimerHandle);
			CountKatanaCooltime = 0.f;
			MyHUD->KatanaCooltimeText->SetRenderOpacity(0.f); //쿨타임 초기화시 큘타임 Text 투명화
		}
	}
}

//인게임 질주스킬 쿨타임, HUD 스킬 아이콘(ProgressBar) 및 쿨타임 Text와 동기화
void AHOPECharacter::UpdateDashCooltime()
{
	CountDashCooltime += 0.01f;


	AHOPEGameModeBase* GameMode = Cast<AHOPEGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		UHOPEUserWidget* MyHUD = Cast<UHOPEUserWidget>(GameMode->CurrentWidget);
		if (MyHUD)
		{
			MyHUD->DashCooltimeText->SetRenderOpacity(1.f);
			MyHUD->DashIcon->SetPercent(0.1f + CountDashCooltime * 0.8 / DashCooltime);
			const FString DashStr = FString::Printf(TEXT("%.0f"), DashCooltime - CountDashCooltime);
			MyHUD->DashCooltimeText->SetText(FText::FromString(DashStr));
		}

		if (CountDashCooltime >= DashCooltime)
		{
			GetWorldTimerManager().ClearTimer(DashCooltimeTimerHandle);
			CountDashCooltime = 0.f;
			MyHUD->DashCooltimeText->SetRenderOpacity(0.f); //쿨타임 초기화시 Text 투명화
		}
	}
}

//탄변경 스킬 사용시 사용하는 총알의 아이콘이 보이도록 각 아이콘의 투명화 설정
void AHOPECharacter::UpdateAmmoIcon(float Normal, float Fire, float Lightning)
{
	AHOPEGameModeBase* GameMode = Cast<AHOPEGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		UHOPEUserWidget* MyHUD = Cast<UHOPEUserWidget>(GameMode->CurrentWidget);
		if (MyHUD)
		{
			MyHUD->NormalAmmoIcon->SetRenderOpacity(Normal);
			MyHUD->FireAmmoIcon->SetRenderOpacity(Fire);
			MyHUD->LightningAmmoIcon->SetRenderOpacity(Lightning);
		}
	}
}

//인게임 탄변경스킬 쿨타임, HUD 스킬 아이콘(ProgressBar) 및 쿨타임 Text와 동기화
void AHOPECharacter::UpdateAmmoCooltime()
{
	CountAmmoCooltime += 0.01f;


	AHOPEGameModeBase* GameMode = Cast<AHOPEGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		UHOPEUserWidget* MyHUD = Cast<UHOPEUserWidget>(GameMode->CurrentWidget);
		if (MyHUD)
		{
			MyHUD->AmmoCooltimeText->SetRenderOpacity(1.f); //쿨타임 Text 가시화
			MyHUD->NormalAmmoIcon->SetPercent(0.1f + CountAmmoCooltime * 0.8 / AmmoCooltime); //아이콘 채우는 연출
			MyHUD->FireAmmoIcon->SetPercent(0.1f + CountAmmoCooltime * 0.8 / AmmoCooltime); //아이콘 채우는 연출
			MyHUD->LightningAmmoIcon->SetPercent(0.1f + CountAmmoCooltime * 0.8 / AmmoCooltime); //아이콘 채우는 연출
			const FString AmmoStr = FString::Printf(TEXT("%.0f"), AmmoCooltime - CountAmmoCooltime);
			MyHUD->AmmoCooltimeText->SetText(FText::FromString(AmmoStr)); //쿨타임 남은시간
		}

		if (CountAmmoCooltime >= AmmoCooltime) //쿨타임 완료
		{
			GetWorldTimerManager().ClearTimer(AmmoCooltimeTimerHandle);
			CountAmmoCooltime = 0.f;
			MyHUD->AmmoCooltimeText->SetRenderOpacity(0.f); //쿨타임 초기화시 Text 투명화
		}
	}
}

void AHOPECharacter::OnRifle() //소드 모드 Off
{
	IsOnKatana = false;
	IsAttacking = false;

	GetMesh()->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 60.f;

	Weapon->SetOwnerNoSee(false); //소총 가시화
	Scope->SetOwnerNoSee(false);
	Katana->SetOwnerNoSee(true); //소드 투명화

	GetWorldTimerManager().ClearTimer(WaitTimerHandle); //소드 모드 지속시간 TimerHandle 클리어
}

void AHOPECharacter::OnKatana() //소드 모드 On
{
	if (IsOnKatana)
		return;

	if (CountKatanaCooltime > 0.f)
		return;

	IsOnKatana = true;
	IsReloading = false; //장전 중 사용 가능하므로 추가
	IsAttacking = false; //사격 중 사용 가능하므로 추가

	KatanaRemainingTime = 10.f;

	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	SpringArm->TargetArmLength = 80.f;

	AnimInstance->PlayInKatanaMontage(); //칼 꺼내는 몽타주 실행

	Weapon->SetOwnerNoSee(true);  //소총 투명화
	Scope->SetOwnerNoSee(true);
	Katana->SetOwnerNoSee(false); //소드 가시화

	SeeKatanaLongTime(1.f); //소드 모드 지속시간 상태바 가시화

	//10초(소드 모드 지속시간) 후 엔드 Montage 실행
	float WaitTime = 10.f;
	GetWorldTimerManager().SetTimer(WaitTimerHandle, FTimerDelegate::CreateLambda([&]()
		{
			if (!IsOnKatana)
				return;

			AnimInstance->PlayEndKatanaMontage(); //엔드 몽타주 실행

			GetWorldTimerManager().ClearTimer(WaitTimerHandle);
			
		}), WaitTime, false);

	//0.01초마다 실행
	GetWorldTimerManager().SetTimer(KatanaLongTimerHandle, this, &AHOPECharacter::UpdateKatanaLongTime, 0.01f, true, 0.f); //지속 시간 상태바 관리
	GetWorldTimerManager().SetTimer(KatanaCooltimeTimerHandle, this, &AHOPECharacter::UpdateKatanaCooltime, 0.01f, true, 0.f); //쿨타임 관리
}

//소드 모드 추가스킬
void AHOPECharacter::KatanaSlash()
{
	if(!IsOnKatana)
		return;

	AnimInstance->PlaySlashMontage();
}

void AHOPECharacter::Defense()
{
	if (!IsOnKatana)
		return;

	IsAttacking = false;
	AnimInstance->PlayDefenseMontage();
}

void AHOPECharacter::DefenseEnd()
{
	if (!IsOnKatana)
		return;

	AnimInstance->PlayStopDefenseMontage();

	UE_LOG(LogTemp, Log, TEXT("BlockEnd"));
}

void AHOPECharacter::BlockOn()
{
	UE_LOG(LogTemp, Log, TEXT("BlockOn"));
}

//소드 모드 검귀 스폰
void AHOPECharacter::SpawnSlash()
{
	if (SlashProjectileClass)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		FVector SocketLocation = GetMesh()->GetSocketLocation("spawnslash_socket"); //칼날 위에서 스폰

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			auto Blade = GetWorld()->SpawnActor<ASlashProjectile>(SlashProjectileClass, SocketLocation, FRotator::ZeroRotator);
			if (nullptr != Blade)
			{
				FVector LaunchDirection = CameraRotation.Vector();
				Blade->FireInDirection(LaunchDirection);
			}
		}
	}
}

//탄변경 스킬
void AHOPECharacter::ChangeAmmo()
{
	if (IsReloading)
		return;

	if (IsOnKatana)
		return;

	if (CountAmmoCooltime > 0.f)
		return;

	GetWorldTimerManager().SetTimer(AmmoCooltimeTimerHandle, this, &AHOPECharacter::UpdateAmmoCooltime, 0.01f, true, 0.f);

	//Normal->Fire->Lightning->Normal 순으로 순환하며 변경됨
	//3가지 if문이 동시에 발동되어 의도한대로 작동하지 않아 TimerHandle을 이용
	if (NormalAmmo)
	{
		float WaitTime = 0.01f;
		GetWorldTimerManager().SetTimer(WaitTimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				NormalAmmo = false;
				FireAmmo = true;
				LightningAmmo = false;

				GetWorldTimerManager().ClearTimer(WaitTimerHandle);

				UpdateAmmoIcon(0.f, 1.f, 0.f); //상태에 알맞는 아이콘 표시

			}), WaitTime, false);
	}
	if (FireAmmo)
	{
		float WaitTime = 0.01f;
		GetWorldTimerManager().SetTimer(WaitTimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				NormalAmmo = false;
				FireAmmo = false;
				LightningAmmo = true;

				GetWorldTimerManager().ClearTimer(WaitTimerHandle);

				UpdateAmmoIcon(0.f, 0.f, 1.f); //상태에 알맞는 아이콘 표시

			}), WaitTime, false);
	}
	if (LightningAmmo)
	{
		float WaitTime = 0.01f;
		GetWorldTimerManager().SetTimer(WaitTimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				NormalAmmo = true;
				FireAmmo = false;
				LightningAmmo = false;

				GetWorldTimerManager().ClearTimer(WaitTimerHandle);

				UpdateAmmoIcon(1.f, 0.f, 0.f); //상태에 알맞는 아이콘 표시

			}), WaitTime, false);
	}
}

//일정간격마다 AutoHealAmount만큼 체력 회복
void AHOPECharacter::AutoHeal()
{
	if (CurrentHP < MaxHP)
	{
		CurrentHP += AutoHealAmount;
		UpdateHP();
	}

	if (CurrentHP > MaxHP) //회복이 최대 체력을 넘을 수 없음
	{
		CurrentHP = MaxHP;
		UpdateHP();
	}
}

void AHOPECharacter::OnSkillTooltip()
{
	AHOPEGameModeBase* GameMode = Cast<AHOPEGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		UHOPEUserWidget* MyHUD = Cast<UHOPEUserWidget>(GameMode->CurrentWidget);
		if (MyHUD)
		{
			MyHUD->SkillTooltip->SetRenderOpacity(1.f);
		}
	}
}

void AHOPECharacter::OffSkillTooltip()
{
	AHOPEGameModeBase* GameMode = Cast<AHOPEGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		UHOPEUserWidget* MyHUD = Cast<UHOPEUserWidget>(GameMode->CurrentWidget);
		if (MyHUD)
		{
			MyHUD->SkillTooltip->SetRenderOpacity(0.f);
		}
	}
}