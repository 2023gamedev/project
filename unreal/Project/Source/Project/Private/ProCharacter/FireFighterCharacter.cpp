// Fill out your copyright notice in the Description page of Project Settings.


#include "ProCharacter/FireFighterCharacter.h"
#include "ProCharacter/PlayerCharacterAnimInstance.h"
#include "ProCharacter/PlayerCharacterController.h"

AFireFighterCharacter::AFireFighterCharacter()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("/Game/CharacterAsset/FireFighter/FireFighter_UE.FireFighter_UE"));
	
	if (SK_MANNEQUIN.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_MANNEQUIN.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> CHARACTER_ANIM(TEXT("/Game/CharacterAsset/Animation/BP_EmployeeAnimBlueprint.BP_EmployeeAnimBlueprint_C"));

	if (CHARACTER_ANIM.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(CHARACTER_ANIM.Class);
	}

	SetHP(60);
	SetMaxHP(60);
	SetBasicSpeed(5);
	SetSTR(7);
	SetStamina(100);
	SetSpecialEffect(true);
	SetCharacterName("FireFighterCharacter");
}

void AFireFighterCharacter::BeginPlay()
{
	Super::BeginPlay();
	SmokeTimer();
}

void AFireFighterCharacter::SmokeTimer()
{
	GetWorld()->GetTimerManager().SetTimer(SmokeHandle, this, &AFireFighterCharacter::LimitSmoking, 120.0f, false);
}

void AFireFighterCharacter::LimitSmoking()
{
	LimitSmokingIcon();
	SetSTR(GetSTR() - 2);
	SetBasicSpeed(GetBasicSpeed() - 1);
	GetWorld()->GetTimerManager().SetTimer(DyingHandle, this, &AFireFighterCharacter::NoSmokeIsDying, 2.0f, true, 1.0f);

}

void AFireFighterCharacter::NoSmokeIsDying()
{
	SetHP(GetHP() - 1);
	if (GetHP() <= 0) {
		SetDead(true);
		auto CharacterAnimInstance = Cast<UPlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());
		if (nullptr != CharacterAnimInstance) {
			CharacterAnimInstance->SetCurrentPawnSpeed(GetVelocity().Size());
			CharacterAnimInstance->SetIsDead(IsDead());
		}
		APlayerCharacterController* controller = Cast<APlayerCharacterController>(this->GetController());
		if (controller != nullptr) {
			DisableInput(controller); // 이래도 움직임 좀비 죽었을때 회전이랑 캐릭터 움직이는 거 수정해야 할듯
		}
	}
}

void AFireFighterCharacter::Smoking()
{
	SmokingIcon();
	GetWorld()->GetTimerManager().ClearTimer(SmokeHandle);
	GetWorld()->GetTimerManager().ClearTimer(DyingHandle);
	if (GetSTR() < 7) {
		SetSTR(GetSTR() + 2);
	}
	if (GetBasicSpeed() < 5) {
		SetBasicSpeed(GetBasicSpeed() + 1);
	}
	SmokeTimer();
}

void AFireFighterCharacter::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
}
