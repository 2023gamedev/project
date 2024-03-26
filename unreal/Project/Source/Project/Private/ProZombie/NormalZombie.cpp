// Fill out your copyright notice in the Description page of Project Settings.


#include "ProZombie/NormalZombie.h"
#include "ProZombie/ZombieAIController.h"
#include "ProZombie/ZombieAnimInstance.h"


ANormalZombie::ANormalZombie()
{
	//AIControllerClass = AZombieAIController::StaticClass();
	
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.f), FRotator(0.f, -90.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn"));
	if (SK_MANNEQUIN.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_MANNEQUIN.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> ZOMBIE_ANIM(TEXT("/Game/ZombieAnimBlueprint.ZombieAnimBlueprint_C"));
	if (ZOMBIE_ANIM.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(ZOMBIE_ANIM.Class);
	}



	SetHP(20);
	SetSpeed(2);
	GetCharacterMovement()->MaxWalkSpeed = 200.f;
	SetSTR(FMath::RandRange(4, 8)); // 수정 필요 4 ~ 8
	SetSpecialAbility(false);
	SetZombieName("NormalZombie");
}

void ANormalZombie::BeginPlay()
{
	Super::BeginPlay();
}

void ANormalZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto CharactorAnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr != CharactorAnimInstance) {
		CharactorAnimInstance->SetCurrentPawnSpeed(GetVelocity().Size());
	}
}

void ANormalZombie::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

}


