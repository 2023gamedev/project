// Fill out your copyright notice in the Description page of Project Settings.


#include "ProZombie/BaseZombie.h"
#include "Engine/DamageEvents.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ProZombie/ZombieAIController.h"
#include "ProZombie/RunningZombieAIController.h"
#include "ProZombie/ShoutingZombieAIController.h"
#include "ProZombie/ZombieAnimInstance.h"

// Sets default values
ABaseZombie::ABaseZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.f, 0.0f);
	GetCapsuleComponent()->SetCollisionProfileName("Zombie");

	
	ZombieId = 0;
	
	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceConsiderationRadius = 400.f;
}

// Called when the game starts or when spawned
void ABaseZombie::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto CharacterAnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr != CharacterAnimInstance) {
		CharacterAnimInstance->SetCurrentPawnSpeed(GetVelocity().Size());
	}


}

void ABaseZombie::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 변경 필요 각각의 animinstance가 필요할 것 같다.
	auto AnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());

	AnimInstance->OnMontageEnded.AddDynamic(this, &ABaseZombie::AttackMontageEnded);
	AnimInstance->OnMontageEnded.AddDynamic(this, &ABaseZombie::ShoutingMontageEnded);
	AnimInstance->OnMontageEnded.AddDynamic(this, &ABaseZombie::BeAttackedMontageEnded);

	AnimInstance->OnAttackStartCheck.AddLambda([this]() -> void {
		AttackCheck();
		});

}

void ABaseZombie::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

float ABaseZombie::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("HP %f"), GetHP()));
	SetHP(GetHP() - Damage);
	BeAttacked();

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("HP %f"), GetHP()));
	return Damage;
}

void ABaseZombie::SetNormalDeadWithAnim()
{
	m_bIsNormalDead = true;
	auto CharacterAnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr != CharacterAnimInstance) {
		CharacterAnimInstance->SetIsNormalDead(m_bIsNormalDead);
	}
	GetCapsuleComponent()->SetCollisionProfileName("NoCollision");

	StartResurrectionTimer();

	StopAITree();
}

void ABaseZombie::StopAITree()
{
	if (GetZombieName() == "NormalZombie") {
		AZombieAIController* NormalZombieAIController = Cast<AZombieAIController>(GetController());

		NormalZombieAIController->StopAI();

	}
	else if (GetZombieName() == "RunningZombie") {
		ARunningZombieAIController* RunningZombieAIController = Cast<ARunningZombieAIController>(GetController());

		RunningZombieAIController->StopAI();
	}
	else if (GetZombieName() == "ShoutingZombie") {
		AShoutingZombieAIController* ShoutingZombieAIController = Cast<AShoutingZombieAIController>(GetController());

		ShoutingZombieAIController->StopAI();
	}

}

void ABaseZombie::StartAITree()
{
	if (GetZombieName() == "NormalZombie") {
		AZombieAIController* NormalZombieAIController = Cast<AZombieAIController>(GetController());

		NormalZombieAIController->StartAI();

	}
	else if (GetZombieName() == "RunningZombie") {
		ARunningZombieAIController* RunningZombieAIController = Cast<ARunningZombieAIController>(GetController());

		RunningZombieAIController->StartAI();
	}
	else if (GetZombieName() == "ShoutingZombie") {
		AShoutingZombieAIController* ShoutingZombieAIController = Cast<AShoutingZombieAIController>(GetController());

		ShoutingZombieAIController->StartAI();
	}
}

void ABaseZombie::SetCuttingDeadWithAnim()
{
	m_bIsCuttingDead = true;
	auto CharacterAnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr != CharacterAnimInstance) {
		CharacterAnimInstance->SetIsCuttingDead(m_bIsCuttingDead);
	}
	GetCapsuleComponent()->SetCollisionProfileName("NoCollision");

	StartResurrectionTimer();

	StopAITree();
}

void ABaseZombie::Attack()
{
	if (m_bIsAttacking) {
		return;
	}
	auto AnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());


	AnimInstance->PlayAttackMontage();
	
	m_bIsAttacking = true;


	if (!IsDie()) {
		StopAITree();
	}

}

void ABaseZombie::AttackMontageEnded(UAnimMontage* Montage, bool interrup)
{
	m_bIsAttacking = false;

	m_DAttackEnd.Broadcast();

	if (!IsDie()) {
		StartAITree();
	}
}

void ABaseZombie::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * m_fAttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(m_fAttackRadius),
		Params
		);


	// debug 용(충돌 범위 확인 용)
	//FVector TraceVec = GetActorForwardVector() * m_fAttackRange;
	//FVector Center = GetActorLocation() + TraceVec * 0.5f;
	//float HalfHeight = m_fAttackRange * 0.5f + 50.f;
	//FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	//FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	//float DebugLifeTime = m_fAttackRadius;


	//DrawDebugCapsule(GetWorld(),
	//	Center,
	//	HalfHeight,
	//	m_fAttackRadius,
	//	CapsuleRot,
	//	DrawColor,
	//	false,
	//	DebugLifeTime);

	if (bResult) {

		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Hit Actor")));
		FDamageEvent DamageEvent;
		HitResult.GetActor()->TakeDamage(GetSTR(), DamageEvent, GetController(), this);
	}
}

void ABaseZombie::Shouting()
{
	if (m_bIsShouting) {
		return;
	}
	auto AnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());


	AnimInstance->PlayShoutingMontage();


	UWorld* World = GetWorld();
	FVector Center = GetActorLocation();
	float DetectRadius = 2000.f;

	if (nullptr == World) return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	if (bResult) {

		for (const FOverlapResult& OverlapResult : OverlapResults)
		{
			// ABaseZombie인지 확인
			ABaseZombie* OverlappedZombie = Cast<ABaseZombie>(OverlapResult.GetActor());
			if (OverlappedZombie)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("HEAR"));
				OverlappedZombie->UpdateLastKnownPositionByFootSound(GetActorLocation());
			}

		}
	}

	m_bIsShouting = true;

	
	UE_LOG(LogTemp, Log, TEXT("Character is Die :: %s"), IsShouted() ? TEXT("true") : TEXT("false"));
	UE_LOG(LogTemp, Error, TEXT("PLAYSHOUTINHGGGGGGGGGGGGGGGGGGGGGGGGGGG"));
}

void ABaseZombie::ShoutingMontageEnded(UAnimMontage* Montage, bool interrup)
{
	m_bIsShouting = false;
	SetShouted(true);
	UE_LOG(LogTemp, Error, TEXT("bIsShouted true"));
	m_DShoutingEnd.Broadcast();

}

void ABaseZombie::BeAttacked()
{
	if (m_bBeAttacked) {
		return;
	}
	auto AnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());


	AnimInstance->PlayBeAttackedMontage();

	GetCharacterMovement()->MaxWalkSpeed = 1.f;
	m_bBeAttacked = true;
}

void ABaseZombie::BeAttackedMontageEnded(UAnimMontage* Montage, bool interrup)
{
	m_bBeAttacked = false;
	GetCharacterMovement()->MaxWalkSpeed = GetSpeed() * 100.f;
}



void ABaseZombie::SetZombieId(uint32 NewZombieId)
{
	ZombieId = NewZombieId;
}

uint32 ABaseZombie::GetZombieId() const
{
	return ZombieId;
}

void ABaseZombie::UpdateZombieData(FVector Location)
{
	NewLocation = Location;
}

void ABaseZombie::StartResurrectionTimer()
{

	if (m_bIsNormalDead) {
		GetWorld()->GetTimerManager().SetTimer(ResurrectionHandle, this, &ABaseZombie::ResurrectionTimerElapsed, 30.0f, false);
	}
	else if (m_bIsCuttingDead) {
		GetWorld()->GetTimerManager().SetTimer(ResurrectionHandle, this, &ABaseZombie::ResurrectionTimerElapsed, 60.0f, false);
	}

}


void ABaseZombie::ResurrectionTimerElapsed()
{

	m_bIsCuttingDead = false;
	m_bIsNormalDead = false;


	m_bIsStanding = true;
	auto CharacterAnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr != CharacterAnimInstance) {
		CharacterAnimInstance->SetIsStanding(m_bIsStanding);
	}
	StartWatiingTimer();
}

void ABaseZombie::StartWatiingTimer()
{
	GetWorld()->GetTimerManager().SetTimer(WattingHandle, this, &ABaseZombie::WaittingTimerElapsed, 5.f, false);
}

void ABaseZombie::WaittingTimerElapsed()
{
	m_bIsStanding = false;
	auto CharacterAnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr != CharacterAnimInstance) {
		CharacterAnimInstance->SetIsNormalDead(m_bIsNormalDead);
		CharacterAnimInstance->SetIsCuttingDead(m_bIsCuttingDead);
		CharacterAnimInstance->SetIsStanding(m_bIsStanding);
	}
	GetCharacterMovement()->MaxWalkSpeed = GetSpeed() * 100.f;
	GetCapsuleComponent()->SetCollisionProfileName("Zombie");
	SetHP(GetStartHP());

	SetDie(true);
	StartAITree();
}

void ABaseZombie::StartAttackedStunHandle()
{
	StopAITree();
	GetWorld()->GetTimerManager().SetTimer(AttakcedStunHandle, this, &ABaseZombie::AttackedStunTimerElapsed, 2.0f, false);
}

void ABaseZombie::AttackedStunTimerElapsed()
{
	StartAITree();
}

void ABaseZombie::UpdateLastKnownPositionByFootSound(FVector playerlocation)
{
	if (GetController()) {
		if (GetZombieName() == "NormalZombie") {
			AZombieAIController* AIZombieController = Cast<AZombieAIController>(GetController());
			if (AIZombieController) {
				AIZombieController->UpdateLastKnownPositionByFootSound(playerlocation);
			}
		}
		else if (GetZombieName() == "RunningZombie") {
			ARunningZombieAIController* AIRunningZombieController = Cast<ARunningZombieAIController>(GetController());
			if (AIRunningZombieController) {
				AIRunningZombieController->UpdateLastKnownPositionByFootSound(playerlocation);
			}
		}
		else if (GetZombieName() == "ShoutingZombie") {
			AShoutingZombieAIController* AIShoutingZombieController = Cast<AShoutingZombieAIController>(GetController());
			if (AIShoutingZombieController) {
				AIShoutingZombieController->UpdateLastKnownPositionByFootSound(playerlocation);
			}
		}
	}


}

void ABaseZombie::UpdateLastKnownPositionByShoutingSound(FVector playerlocation)
{
	if (GetController()) {
		if (GetZombieName() == "NormalZombie") {
			AZombieAIController* AIZombieController = Cast<AZombieAIController>(GetController());
			if (AIZombieController) {
				AIZombieController->UpdateLastKnownPositionByFootSound(playerlocation);
			}
		}
		else if (GetZombieName() == "RunningZombie") {
			ARunningZombieAIController* AIRunningZombieController = Cast<ARunningZombieAIController>(GetController());
			if (AIRunningZombieController) {
				AIRunningZombieController->UpdateLastKnownPositionByFootSound(playerlocation);
			}
		}
		else if (GetZombieName() == "ShoutingZombie") {
			AShoutingZombieAIController* AIShoutingZombieController = Cast<AShoutingZombieAIController>(GetController());
			if (AIShoutingZombieController) {
				AIShoutingZombieController->UpdateLastKnownPositionByFootSound(playerlocation);
			}
		}
	}
}





