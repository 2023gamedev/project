// Fill out your copyright notice in the Description page of Project Settings.


#include "ProCharacter/PlayerCharacterAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"


UPlayerCharacterAnimInstance::UPlayerCharacterAnimInstance()
{
	m_fCurrentPawnSpeed = 0.f;
	m_bPawnRun = false;
	m_bIsInAir = false;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/CharacterAsset/Animation/BP_AMEmployee.BP_AMEmployee"));
	if (ATTACK_MONTAGE.Succeeded()) {
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}

void UPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto pawn = TryGetPawnOwner();

	if (::IsValid(pawn)) {
		auto character = Cast<ACharacter>(pawn);
		if (character) {
			m_bIsInAir = character->GetMovementComponent()->IsFalling();
		}

	}


}

void UPlayerCharacterAnimInstance::PlayAttackMontage()
{
	if (!Montage_IsPlaying(AttackMontage)) {
		Montage_Play(AttackMontage, 1.f);
	}
}

void UPlayerCharacterAnimInstance::AnimNotify_AttackStart()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("AnimNotify_AttackStart"));
	OnAttackStartCheck.Broadcast();
}

void UPlayerCharacterAnimInstance::AnimNotify_AttackEnd()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("AnimNotify_AttackEnd"));
	OnAttackEndCheck.Broadcast();
}
