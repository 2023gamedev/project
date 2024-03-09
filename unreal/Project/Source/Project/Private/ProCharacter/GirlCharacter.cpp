// Fill out your copyright notice in the Description page of Project Settings.


#include "ProCharacter/GirlCharacter.h"

AGirlCharacter::AGirlCharacter()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("/Game/SKM_QuinnNew.SKM_QuinnNew"));

	if (SK_MANNEQUIN.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_MANNEQUIN.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> CHARACTER_ANIM(TEXT("/Game/ChAnimBlueprint.ChAnimBlueprint_C"));

	if (CHARACTER_ANIM.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(CHARACTER_ANIM.Class);
	}

	SetHP(40);
	SetSpeed(6);
	SetSTR(3);
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