// Fill out your copyright notice in the Description page of Project Settings.


#include "ProCharacter/PlayerCharacterAnimInstance.h"
#include "GameFramework/Character.h"
#include "ProCharacter/BaseCharacter.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"


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
	m_bIsAttacking = false;
	m_bIsPickUp = false;
	m_fPitch = 0.f;

	// 위에 FObjectFinder는 기존것 밑에는 새롭게 애니메이션한 부분
	//static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/CharacterAsset/Animation/BP_AMEmployee.BP_AMEmployee"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/CharacterAsset/JAnimation/BP_AMEmployeeJ.BP_AMEmployeeJ"));
	if (ATTACK_MONTAGE.Succeeded()) {
		AttackMontage = ATTACK_MONTAGE.Object;
	}
	
	//static ConstructorHelpers::FObjectFinder<UAnimMontage> PICKUP_MONTAGE(TEXT("/Game/CharacterAsset/Animation/BP_AMEmployeePickup.BP_AMEmployeePickup"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PICKUP_MONTAGE(TEXT("/Game/CharacterAsset/JAnimation/BP_AMEmployeePickupJ.BP_AMEmployeePickupJ"));
	if (PICKUP_MONTAGE.Succeeded()) {
		PickUpMontage = PICKUP_MONTAGE.Object;
	}

	//static ConstructorHelpers::FObjectFinder<UAnimMontage> HEALING_MONTAGE(TEXT("/Game/CharacterAsset/Animation/BP_AMEmployeeHealing.BP_AMEmployeeHealing"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> HEALING_MONTAGE(TEXT("/Game/CharacterAsset/JAnimation/BP_AMEmployeeHealingJ.BP_AMEmployeeHealingJ"));
	if (HEALING_MONTAGE.Succeeded()) {
		HealingMontage = HEALING_MONTAGE.Object;
	}

	//static ConstructorHelpers::FObjectFinder<UAnimMontage> BH_MONTAGE(TEXT("/Game/CharacterAsset/Animation/BP_AMEmployeeBleedHealing.BP_AMEmployeeBleedHealing"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> BH_MONTAGE(TEXT("/Game/CharacterAsset/JAnimation/BP_AMEmployeeBleedHealingJ.BP_AMEmployeeBleedHealingJ"));
	if (BH_MONTAGE.Succeeded()) {
		BleedHealingMontage = BH_MONTAGE.Object;
	}

	//static ConstructorHelpers::FObjectFinder<UAnimMontage> OK_MONTAGE(TEXT("/Game/CharacterAsset/Animation/BP_AMEmployeeOpenKey.BP_AMEmployeeOpenKey"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> OK_MONTAGE(TEXT("/Game/CharacterAsset/JAnimation/BP_AMEmployeeOpenKeyJ.BP_AMEmployeeOpenKeyJ"));
	if (OK_MONTAGE.Succeeded()) {
		OpenKeyMontage = OK_MONTAGE.Object;
	}

	//static ConstructorHelpers::FObjectFinder<UAnimMontage> JUMP_MONTAGE(TEXT("/Game/CharacterAsset/Animation/Other_Jump.Other_Jump"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> JUMP_MONTAGE(TEXT("/Game/CharacterAsset/JAnimation/Other_Jump.Other_Jump"));
	if (JUMP_MONTAGE.Succeeded()) {
		JumpMontage = JUMP_MONTAGE.Object;
	}

}

void UPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (OwnerCharacter) {
		m_bIsInAir = OwnerCharacter->GetMovementComponent()->IsFalling();
		m_bIsHandInWeapon = OwnerCharacter->IsNWHandIn();
		
		UpdateFootstepSound();
	}
	else {
		auto pawn = TryGetPawnOwner();

		if (::IsValid(pawn)) {
			OwnerCharacter = Cast<ABaseCharacter>(pawn);
		}
	}
}

void UPlayerCharacterAnimInstance::PickUpStart()
{
	m_bIsPickUp = true;
}
void UPlayerCharacterAnimInstance::AttackStart()
{
	m_bIsAttacking = true;
}


void UPlayerCharacterAnimInstance::PickUpEnd()
{
	m_bIsPickUp = false;
}
void UPlayerCharacterAnimInstance::AttackEnd()
{
	m_bIsAttacking = false;
}

void UPlayerCharacterAnimInstance::PlayAttackMontage()
{
	if (!Montage_IsPlaying(AttackMontage)) {
		//m_bIsAttacking = true;
		Montage_Play(AttackMontage, 1.f);
	}
}

void UPlayerCharacterAnimInstance::PlayPickUpMontage()
{
	if (!Montage_IsPlaying(PickUpMontage)) {
		//m_bIsPickUp = true;
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
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("JumpMontage 재생 시작"));
			UE_LOG(LogTemp, Display, TEXT("PlayJumpMontage()"));
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("JumpMontage 재생 실패"));
			UE_LOG(LogTemp, Warning, TEXT("PlayJumpMontage() 재생 실패: JumpMontage 이미 재생중"));
		}
	}
}


void UPlayerCharacterAnimInstance::PlayKeyMontage(float PlaySpeed)
{
	if (!Montage_IsPlaying(OpenKeyMontage)) {
		if (Montage_Play(OpenKeyMontage, PlaySpeed))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("OpenKeyMontage 재생 시작"));
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("OpenKeyMontage 재생 실패"));
		}
	}
}

void UPlayerCharacterAnimInstance::AnimNotify_AttackStart()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("AnimNotify_AttackStart"));
	OnAttackStartCheck.Broadcast();
}

void UPlayerCharacterAnimInstance::AnimNotify_AttackEnd()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("AnimNotify_AttackEnd"));
	OnAttackEndCheck.Broadcast();
}

void UPlayerCharacterAnimInstance::AnimNotify_FootSound1()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("AnimNotify_FootSound1"));
	OnFootSoundCheck.Broadcast();
}

void UPlayerCharacterAnimInstance::AnimNotify_FootSound2()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("AnimNotify_FootSound2"));
	OnFootSoundCheck.Broadcast();
}

void UPlayerCharacterAnimInstance::PlayFootstepRunSound()
{
	if (OwnerCharacter) {
		if (!FootstepRunAudioComponent)
		{
			UE_LOG(LogTemp, Error, TEXT("FootstepRunAudioComponent is not here!"));
		}

		if (FootstepRunAudioComponent && !FootstepRunAudioComponent->IsPlaying())
		{
			FootstepRunAudioComponent->Play();
		}
	}
}

void UPlayerCharacterAnimInstance::StopFootstepRunSound()
{
	if (FootstepRunAudioComponent && FootstepRunAudioComponent->IsPlaying())
	{
		FootstepRunAudioComponent->Stop();
	}
}

void UPlayerCharacterAnimInstance::PlayFootstepWalkSound()
{
	if (OwnerCharacter) {
		if (!FootstepWalkAudioComponent)
		{
			UE_LOG(LogTemp, Error, TEXT("FootstepWalkAudioComponent is not here!"));
		}

		if (FootstepWalkAudioComponent && !FootstepWalkAudioComponent->IsPlaying())
		{
			FootstepWalkAudioComponent->Play();
		}
	}
}

void UPlayerCharacterAnimInstance::StopFootstepWalkSound()
{
	if (FootstepWalkAudioComponent && FootstepWalkAudioComponent->IsPlaying())
	{
		FootstepWalkAudioComponent->Stop();
	}
}

void UPlayerCharacterAnimInstance::UpdateFootstepSound()
{
	if (OwnerCharacter) {

		if (OwnerCharacter->GetHP() > 0) {	// 살아 있을때만
			if (OwnerCharacter->IsRun() == true) {		// 뛰는 중
				if (OwnerCharacter->GetVelocity().Size() > 0)
				{
					PlayFootstepRunSound();  // 이동 중이면 뛰기소리 재생
				}
				else
				{
					StopFootstepRunSound();  // 멈추면 뛰기소리 정지
				}

				StopFootstepWalkSound();  // 걷기소리는 정지
			}
			else {	// 걷는 중
				if (OwnerCharacter->GetVelocity().Size() > 0)
				{
					PlayFootstepWalkSound();  // 이동 중이면 걷기소리 재생
				}
				else
				{
					StopFootstepWalkSound();  // 멈추면 걷기소리 정지
				}

				StopFootstepRunSound();  // 뛰기소리는 정지
			}
		}
		else {	// 만약 죽었다면
			StopFootstepWalkSound();  // 걷기소리는 정지
			StopFootstepRunSound();  // 뛰기소리는 정지
		}
	}
}

//void UPlayerCharacterAnimInstance::AnimNotify_OnFootstep()
//{
//	PlayFootstepRunSound();
//}

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

UAnimMontage* UPlayerCharacterAnimInstance::GetOpenKeyMontage()
{
	if (OpenKeyMontage) {
		return OpenKeyMontage;
	}
	return nullptr;
}
