// Fill out your copyright notice in the Description page of Project Settings.


#include "ProCharacter/EmployeeCharacter.h"

AEmployeeCharacter::AEmployeeCharacter()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("/Game/CharacterAsset/Employee/EmployeeCharacter_UE.EmployeeCharacter_UE"));

	if (SK_MANNEQUIN.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_MANNEQUIN.Object);
	}
	
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> CHARACTER_ANIM(TEXT("/Game/CharacterAsset/Animation/BP_EmployeeAnimBlueprint.BP_EmployeeAnimBlueprint_C"));

	if (CHARACTER_ANIM.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(CHARACTER_ANIM.Class);
	}

	SetHP(50);
	SetMaxHP(50);
	SetBasicSpeed(4);
	SetSTR(4);
	SetStamina(100);
	SetSpecialEffect(false);
	SetCharacterName("EmployeeCharacter");
}

void AEmployeeCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AEmployeeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
