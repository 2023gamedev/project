// Fill out your copyright notice in the Description page of Project Settings.


#include "ProZombie/RunningZombie.h"
#include "ProZombie/RunningZombieAIController.h"
#include "ProZombie/ZombieAnimInstance.h"

ARunningZombie::ARunningZombie()
{

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.f), FRotator(0.f, -90.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("/Game/ZombieAsset/RunningZombie/RunningZombie.RunningZombie"));
	if (SK_MANNEQUIN.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_MANNEQUIN.Object);
	}


	static ConstructorHelpers::FClassFinder<UAnimInstance> ZOMBIE_ANIM(TEXT("/Game/ZombieAsset/Animation/RunningZombieAnimation/BP_RunningZombieAnimBlueprint.BP_RunningZombieAnimBlueprint_C"));
	if (ZOMBIE_ANIM.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(ZOMBIE_ANIM.Class);
	}


	SetHP(20);
	SetStartHP(20);
	SetSpeed(4);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	SetSTR(FMath::RandRange(4, 8)); 
	SetSpecialAbility(true);
	SetZombieName("RunningZombie");

}

void ARunningZombie::BeginPlay()
{
	Super::BeginPlay();
}

void ARunningZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto CharactorAnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr != CharactorAnimInstance) {
		CharactorAnimInstance->SetCurrentPawnSpeed(GetVelocity().Size());
	}
}


void ARunningZombie::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}


