// Fill out your copyright notice in the Description page of Project Settings.


#include "ProZombie/ShoutingZombie.h"
#include "ProZombie/ShoutingZombieAIController.h"
#include "ProZombie/ZombieAnimInstance.h"

AShoutingZombie::AShoutingZombie()
{

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.f), FRotator(0.f, -90.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("/Game/ZombieAsset/ShoutingZombie/ShoutingZombie.ShoutingZombie"));
	if (SK_MANNEQUIN.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_MANNEQUIN.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> ZOMBIE_ANIM(TEXT("/Game/ZombieAsset/Animation/ShoutingAnimation/BP_ShoutingZombieAnimBlueprint.BP_ShoutingZombieAnimBlueprint_C"));
	if (ZOMBIE_ANIM.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(ZOMBIE_ANIM.Class);
	}


	SetHP(30);
	SetStartHP(30);
	SetSpeed(3);
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	SetSTR(FMath::RandRange(4, 8)); // 수정 필요 4 ~ 8
	SetSpecialAbility(true);
	SetZombieName("ShoutingZombie");
}

void AShoutingZombie::BeginPlay()
{
	Super::BeginPlay();
}

void AShoutingZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto CharactorAnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr != CharactorAnimInstance) {
		CharactorAnimInstance->SetCurrentPawnSpeed(GetVelocity().Size());
	}
}


void AShoutingZombie::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}