// Fill out your copyright notice in the Description page of Project Settings.


#include "ShoutingZombie.h"
#include "ZombieAIController.h"
#include "ZombieAnimInstance.h"

AShoutingZombie::AShoutingZombie()
{
	//	AIControllerClass = AZombieAIController::StaticClass();

	//	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.f), FRotator(0.f, -90.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn"));
	if (SK_MANNEQUIN.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_MANNEQUIN.Object);
	}

	SetHP(30);
	SetSpeed(3);
	SetSTR(4); // 수정 필요 4 ~ 8
	SetSpecialAbility(true);
}

void AShoutingZombie::BeginPlay()
{
}

void AShoutingZombie::Tick(float DeltaTime)
{
}
