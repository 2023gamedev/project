// Fill out your copyright notice in the Description page of Project Settings.


#include "FireFighterCharacter.h"

AFireFighterCharacter::AFireFighterCharacter()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn"));

	if (SK_MANNEQUIN.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_MANNEQUIN.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> CHARACTER_ANIM(TEXT("/Game/ChAnimBlueprint.ChAnimBlueprint_C"));

	if (CHARACTER_ANIM.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(CHARACTER_ANIM.Class);
	}

	SetHP(60);
	SetSpeed(5);
	SetSTR(7);
	SetSpecialEffect(true);
	SetCharacterName("FireFighterCharacter");
}

void AFireFighterCharacter::BeginPlay()
{
}

void AFireFighterCharacter::Tick(float DeltaTime)
{
}
