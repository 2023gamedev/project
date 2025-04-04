// Fill out your copyright notice in the Description page of Project Settings.


#include "ProCharacter/GirlCharacter.h"

AGirlCharacter::AGirlCharacter()
{
	// 위에 FObjectFinder는 기존것 밑에는 새롭게 애니메이션한 부분
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("/Game/CharacterAsset/Girl/GirlCharacter_UE.GirlCharacter_UE"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("/Game/CharacterAsset/JGirl/Girl.Girl"));

	if (SK_MANNEQUIN.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_MANNEQUIN.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	//static ConstructorHelpers::FClassFinder<UAnimInstance> CHARACTER_ANIM(TEXT("/Game/CharacterAsset/Animation/BP_EmployeeAnimBlueprint.BP_EmployeeAnimBlueprint_C"));
	static ConstructorHelpers::FClassFinder<UAnimInstance> CHARACTER_ANIM(TEXT("/Game/CharacterAsset/JAnimation/BP_EmployeeAnimBlueprintJ.BP_EmployeeAnimBlueprintJ_C"));

	if (CHARACTER_ANIM.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(CHARACTER_ANIM.Class);
	}

	SetHP(40);
	SetMaxHP(40);
	SetBasicSpeed(6);
	SetSTR(3);
	SetStamina(100 + 40);	
	SetMaxStamina(100 + 40);
	SetStaminaHealing(5 + 0.5f);	// 여학생 캐릭 특성
	SetCharacterName("GirlCharacter");

	//// 캐릭터마다 크기(scale) 재설정
	//float characterScale = 1.0f;
	//SetActorScale3D(FVector(characterScale, characterScale, characterScale));
	//// 기존 캡슐 크기를 가져오기
	//float CurrentRadius = GetCapsuleComponent()->GetUnscaledCapsuleRadius();
	//float CurrentHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	////  캡슐 크기도 동일한 비율로 변경
	//GetCapsuleComponent()->SetCapsuleSize(CurrentRadius * characterScale, CurrentHalfHeight * characterScale);
}

void AGirlCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AGirlCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}