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
<<<<<<< HEAD

	SmokeTimer();
=======
	if (GetPlayerId() == 99) {
		SmokeTimer();
	}
>>>>>>> 789e68b506dd6df26509e94b9716c109328cdd78
}

void AFireFighterCharacter::SmokeTimer()
{
	GetWorld()->GetTimerManager().SetTimer(SmokeHandle, this, &AFireFighterCharacter::LimitSmoking, 120.0f, false);	// 2분후 타이머 실행 (담배를 안 피울 시 체력 금단현상 시작)
}

void AFireFighterCharacter::LimitSmoking()
{
	if (GetPlayerId() == 99) {	// 다른 클라가 소방관이면 해당 타이머 작동 시키지 X
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("SmokeTimer ON -> My Client is a FireFighter (PlayerId: 99)")));
		//UE_LOG(LogTemp, Log, TEXT("SmokeTimer ON -> My Client is a FireFighter (PlayerId: 99)"));

		LimitSmokingIcon();
		SetSTR(GetSTR() - 2);
		SetBasicSpeed(GetBasicSpeed() - 1);
		GetWorld()->GetTimerManager().SetTimer(DyingHandle, this, &AFireFighterCharacter::NoSmokeIsDying, 2.0f, true, 1.0f);	// 2초 간격으로 체력 빠짐
	}
	else {
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("SmokeTimer OFF -> Other Client is the FireFighter")));
		//UE_LOG(LogTemp, Log, TEXT("SmokeTimer OFF -> Other Client is the FireFighter"));

		GetWorld()->GetTimerManager().ClearTimer(SmokeHandle);
	}
}

void AFireFighterCharacter::NoSmokeIsDying()
{
	if (GetPlayerId() == 99) {	// 다른 클라가 소방관이면 해당 타이머 작동 시키지 X
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("NoSmokeIsDyingTimer ON -> My Client is a FireFighter (PlayerId: 99)")));
		//UE_LOG(LogTemp, Log, TEXT("NoSmokeIsDyingTimer ON -> My Client is a FireFighter (PlayerId: 99)"));

		SetHP(GetHP() - 1);
		if (GetHP() <= 0 && !IsDeadPlay()) {
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("NoSmokeIsDying -> PlayDead!")));
			//UE_LOG(LogTemp, Log, TEXT("NoSmokeIsDying -> PlayDead!"));
			SetDeadPlay(true);

			if (IsDeadPlay() && !IsDead()) {
				PlayDead();
			}
		}
		else {
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("NoSmokeIsDyingTimer OFF -> Other Client is the FireFighter")));
			//UE_LOG(LogTemp, Log, TEXT("NoSmokeIsDyingTimer OFF -> Other Client is the FireFighter"));

			GetWorld()->GetTimerManager().ClearTimer(DyingHandle);
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
