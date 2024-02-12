// Fill out your copyright notice in the Description page of Project Settings.


#include "IdolCharacter.h"

AIdolCharacter::AIdolCharacter()
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

	SetHP(50);
	SetSpeed(5);
	SetSTR(5);
	SetSpecialEffect(true);
	SetCharacterName("IdolCharacter");
}

void AIdolCharacter::BeginPlay()
{
}

void AIdolCharacter::Tick(float DeltaTime)
{
}