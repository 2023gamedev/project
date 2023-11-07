// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalZombieCharacter.h"
#include "ZombieAIController.h"
#include "ZombieAnimInstance.h"

ANormalZombieCharacter::ANormalZombieCharacter()
{
	AIControllerClass = AZombieAIController::StaticClass();
	
	//이 부분이 적용이 안되서 그냥 블루프린트에서 위치지정
	//GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.f), FRotator(0.f, -90.f, 0.f));
	

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("/Game/Characters/Mannequin_UE4/Meshes/SK_Mannequin.SK_Mannequin"));
	if (SK_MANNEQUIN.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_MANNEQUIN.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> ZOMBIE_ANIM(TEXT("/Game/ZombieAnimBlueprint.ZombieAnimBlueprint_C"));

	if (ZOMBIE_ANIM.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(ZOMBIE_ANIM.Class);
	}

}

void ANormalZombieCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ANormalZombieCharacter::Tick(float DeltaTime)
{
	auto CharacterAnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr != CharacterAnimInstance) {
		CharacterAnimInstance->SetCurrentPawnSpeed(GetVelocity().Size());
	}
}
