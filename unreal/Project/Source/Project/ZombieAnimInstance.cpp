// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieAnimInstance.h"

UZombieAnimInstance::UZombieAnimInstance()
{
	m_fCurrentPawnSpeed = 0.f;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/SK_Mannequin_Montage.SK_Mannequin_Montage"));
	if (ATTACK_MONTAGE.Succeeded()) {
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SHOUTING_MONTAGE(TEXT("/Game/SK_Mannequin_Montage_Shouting.SK_Mannequin_Montage_Shouting"));
	if (SHOUTING_MONTAGE.Succeeded()) {
		ShoutingMontage = SHOUTING_MONTAGE.Object;
	}
}

void UZombieAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UZombieAnimInstance::PlayAttackMontage()
{
	if (!Montage_IsPlaying(AttackMontage)) {
		Montage_Play(AttackMontage, 1.f);
	}
}

void UZombieAnimInstance::PlayShoutingMontage()
{
	if (!Montage_IsPlaying(ShoutingMontage)) {
		Montage_Play(ShoutingMontage, 1.f);
	}
}
