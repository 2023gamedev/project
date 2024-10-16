// Fill out your copyright notice in the Description page of Project Settings.


#include "ProCharacter/GirlCharacter.h"

AGirlCharacter::AGirlCharacter()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("/Game/CharacterAsset/Girl/GirlCharacter_UE.GirlCharacter_UE"));

	if (SK_MANNEQUIN.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_MANNEQUIN.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> CHARACTER_ANIM(TEXT("/Game/CharacterAsset/Animation/BP_EmployeeAnimBlueprint.BP_EmployeeAnimBlueprint_C"));

	if (CHARACTER_ANIM.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(CHARACTER_ANIM.Class);
	}

	SetHP(40);
	SetMaxHP(40);
	SetBasicSpeed(6);
	SetSTR(3);
	SetStamina(100);
	SetSpecialEffect(false);
	SetCharacterName("GirlCharacter");
}

void AGirlCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AGirlCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}