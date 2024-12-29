// Fill out your copyright notice in the Description page of Project Settings.


#include "ProCharacter/IdolCharacter.h"

AIdolCharacter::AIdolCharacter()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("/Game/CharacterAsset/Idol/IdolCharacter_UE.IdolCharacter_UE"));
	
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
	SetBasicSpeed(5);
	SetSTR(5);
	SetStamina(100);
	SetMaxStamina(100);
	SetSpecialEffect(true);
	SetCharacterName("IdolCharacter");
}

void AIdolCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AIdolCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}