// Fill out your copyright notice in the Description page of Project Settings.


#include "ProCharacter/EmployeeCharacter.h"

AEmployeeCharacter::AEmployeeCharacter()
{
	// 위에 FObjectFinder는 기존것 밑에는 새롭게 애니메이션한 부분
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("/Game/CharacterAsset/Employee/EmployeeCharacter_UE.EmployeeCharacter_UE"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("/Game/CharacterAsset/JEmployee/Employee.Employee"));

	if (SK_MANNEQUIN.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_MANNEQUIN.Object);
	}
	
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> CHARACTER_ANIM(TEXT("/Game/CharacterAsset/Animation/BP_EmployeeAnimBlueprint.BP_EmployeeAnimBlueprint_C"));
	//static ConstructorHelpers::FClassFinder<UAnimInstance> CHARACTER_ANIM(TEXT("/Game/CharacterAsset/JAnimation/BP_EmployeeAnimBlueprintJ.BP_EmployeeAnimBlueprintJ_C"));

	if (CHARACTER_ANIM.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(CHARACTER_ANIM.Class);
	}

	SetHP(50);
	SetMaxHP(50);
	SetBasicSpeed(4);
	SetSTR(4);
	SetStamina(100 + 30);
	SetMaxStamina(100 + 30);
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
