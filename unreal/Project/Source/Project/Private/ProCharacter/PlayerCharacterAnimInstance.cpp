// Fill out your copyright notice in the Description page of Project Settings.


#include "ProCharacter/PlayerCharacterAnimInstance.h"
#include "GameFramework/Character.h"
#include "ProCharacter/BaseCharacter.h"
#include "GameFramework/PawnMovementComponent.h"

#define default_healing_anim_playtime 4.57f
#define pb_anim_4_sec 4.f
#define pb_anim_3_5_sec 3.5f
#define pb_anim_3_sec 3.f

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

	static ConstructorHelpers::FObjectFinder<UAnimMontage> JUMP_MONTAGE(TEXT("/Game/CharacterAsset/Animation/Other_Jump.Other_Jump"));
	if (JUMP_MONTAGE.Succeeded()) {
		JumpMontage = JUMP_MONTAGE.Object;
	}

}

void UPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto pawn = TryGetPawnOwner();

	if (::IsValid(pawn)) {
		auto character = Cast<ABaseCharacter>(pawn);
		if (character) {
			m_bIsInAir = character->GetMovementComponent()->IsFalling();
			m_bIsHandInWeapon = character->IsNWHandIn();
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

void UPlayerCharacterAnimInstance::PlayHealingMontage(float PlaySpeed)
{
	if (!Montage_IsPlaying(HealingMontage)) {
		Montage_Play(HealingMontage, PlaySpeed);
	}
}

void UPlayerCharacterAnimInstance::PlayBleedHealingMontage(float PlaySpeed)
{
	if (!Montage_IsPlaying(BleedHealingMontage)) {
		Montage_Play(BleedHealingMontage, PlaySpeed);
	}
}
void UPlayerCharacterAnimInstance::PlayJumpMontage()
{
	if (!Montage_IsPlaying(JumpMontage)) {
		if (Montage_Play(JumpMontage, 1.f))
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("JumpMontage 재생 시작"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("JumpMontage 재생 실패"));
		}
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

UAnimMontage* UPlayerCharacterAnimInstance:: GetAttackMontage()
{
	if (AttackMontage) {
		return AttackMontage;
	}
	return nullptr;
}

UAnimMontage* UPlayerCharacterAnimInstance::GetPickupMontage()
{
	if (PickUpMontage) {
		return PickUpMontage;
	}
	return nullptr;
}

UAnimMontage* UPlayerCharacterAnimInstance::GetHealingMontage()
{
	if (HealingMontage) {
		return HealingMontage;
	}

	return nullptr;
}

UAnimMontage* UPlayerCharacterAnimInstance::GetBleedingMontage()
{
	if (BleedHealingMontage) {
		return BleedHealingMontage;
	}
	return nullptr;
}

