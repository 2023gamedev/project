// Fill out your copyright notice in the Description page of Project Settings.


#include "ProZombie/ZombieAnimInstance.h"

UZombieAnimInstance::UZombieAnimInstance()
{
	m_fCurrentPawnSpeed = 0.f;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/ZombieAsset/Animation/BP_AMAttackZombie.BP_AMAttackZombie"));
	if (ATTACK_MONTAGE.Succeeded()) {
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SHOUTING_MONTAGE(TEXT("/Game/ZombieAsset/Animation/BP_AMShouting.BP_AMShouting"));
	if (SHOUTING_MONTAGE.Succeeded()) {
		ShoutingMontage = SHOUTING_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> BEATTACKED_MONTAGE(TEXT("/Game/ZombieAsset/Animation/BP_AMBeAttackedZombie.BP_AMBeAttackedZombie"));
	if (BEATTACKED_MONTAGE.Succeeded()) {
		BeAttackedMontage = BEATTACKED_MONTAGE.Object;
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

void UZombieAnimInstance::PlayBeAttackedMontage()
{
	if (!Montage_IsPlaying(BeAttackedMontage)) {
		Montage_Play(BeAttackedMontage, 1.f);
	}
}
