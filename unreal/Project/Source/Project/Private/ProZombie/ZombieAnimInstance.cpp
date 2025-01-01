// Fill out your copyright notice in the Description page of Project Settings.


#include "ProZombie/ZombieAnimInstance.h"

UZombieAnimInstance::UZombieAnimInstance()
{
	m_fCurrentPawnSpeed = 0.f;
	m_bIsNormalDead = false;
	m_bIsCuttingDead = false;
	m_bIsStanding = false;

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
		SetCurrentPawnSpeed(0);	// 애니메이션 잠시 정지 (idle 상태) => 해당 애니메이션 끝나고 바로 적용시킬려고 (+ endplay때 부르는 것보다 여기가 더 확실함)
	}
}

void UZombieAnimInstance::PlayShoutingMontage()
{
	if (!Montage_IsPlaying(ShoutingMontage)) {
		Montage_Play(ShoutingMontage, 1.f);
		SetCurrentPawnSpeed(0);	// 애니메이션 잠시 정지 (idle 상태) => 해당 애니메이션 끝나고 바로 적용시킬려고
	}
}

void UZombieAnimInstance::PlayBeAttackedMontage()
{
	if (!Montage_IsPlaying(BeAttackedMontage)) {
		Montage_Play(BeAttackedMontage, 1.f);
		SetCurrentPawnSpeed(0);	// 애니메이션 잠시 정지 (idle 상태) => 해당 애니메이션 끝나고 바로 적용시킬려고
	}
}

void UZombieAnimInstance::AnimNotify_AttackStart()
{
	OnAttackStartCheck.Broadcast();
}

//void UZombieAnimInstance::AnimNotify_AttackEnd()
//{
//	OnAttackEndCheck.Broadcast();
//}
