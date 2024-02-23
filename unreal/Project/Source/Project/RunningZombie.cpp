// Fill out your copyright notice in the Description page of Project Settings.


#include "RunningZombie.h"
#include "RunningZombieAIController.h"
#include "ZombieAnimInstance.h"

ARunningZombie::ARunningZombie()
{
	//AIControllerClass = ARunningZombieAIController::StaticClass();

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.f), FRotator(0.f, -90.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn"));
	if (SK_MANNEQUIN.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_MANNEQUIN.Object);
	}


	static ConstructorHelpers::FClassFinder<UAnimInstance> ZOMBIE_ANIM(TEXT("/Game/RZombieAnimBlueprint.RZombieAnimBlueprint_C"));
	if (ZOMBIE_ANIM.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(ZOMBIE_ANIM.Class);
	}


	SetHP(20);
	SetSpeed(4);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	SetSTR(4); // 수정 필요 4 ~ 8
	SetSpecialAbility(true);
	SetZombieName("RunningZombie");

}

void ARunningZombie::BeginPlay()
{
	Super::BeginPlay();
}

void ARunningZombie::Tick(float DeltaTime)
{
	auto CharactorAnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr != CharactorAnimInstance) {
		CharactorAnimInstance->SetCurrentPawnSpeed(GetVelocity().Size());
	}
}


void ARunningZombie::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}


