// Fill out your copyright notice in the Description page of Project Settings.


#include "ProCharacter/PlayerCharacterAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"


UPlayerCharacterAnimInstance::UPlayerCharacterAnimInstance()
{
	m_fCurrentPawnSpeed = 0.f;
	m_bPawnRun = false;
	m_bIsInAir = false;
	m_bIsDead = false;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/CharacterAsset/Animation/BP_AMEmployee.BP_AMEmployee"));
	if (ATTACK_MONTAGE.Succeeded()) {
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> PICKUP_MONTAGE(TEXT("/Game/CharacterAsset/Animation/BP_AMEmployeePickup.BP_AMEmployeePickup"));
	if (PICKUP_MONTAGE.Succeeded()) {
		PickUpMontage = PICKUP_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HEALING_MONTAGE(TEXT("/Game/CharacterAsset/Animation/BP_AMEmployeeHealing.BP_AMEmployeeHealing"));
	if (HEALING_MONTAGE.Succeeded()) {
		HealingMontage = HEALING_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> BH_MONTAGE(TEXT("/Game/CharacterAsset/Animation/BP_AMEmployeeBleedHealing.BP_AMEmployeeBleedHealing"));
	if (BH_MONTAGE.Succeeded()) {
		BleedHealingMontage = BH_MONTAGE.Object;
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

void UPlayerCharacterAnimInstance::PlayPickUpMontage()
{
	if (!Montage_IsPlaying(PickUpMontage)) {
		Montage_Play(PickUpMontage, 3.f);
	}
}

void UPlayerCharacterAnimInstance::PlayHealingMontage()
{
	if (!Montage_IsPlaying(HealingMontage)) {
		Montage_Play(HealingMontage, 3.f);
	}
}

void UPlayerCharacterAnimInstance::PlayBleedHealingMontage()
{
	if (!Montage_IsPlaying(BleedHealingMontage)) {
		Montage_Play(BleedHealingMontage, 3.f);
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

void UPlayerCharacterAnimInstance::AnimNotify_FootSound1()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("AnimNotify_FootSound1"));
	OnFootSoundCheck.Broadcast();
}

void UPlayerCharacterAnimInstance::AnimNotify_FootSound2()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("AnimNotify_FootSound2"));
	OnFootSoundCheck.Broadcast();
}


