// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/AudioComponent.h"

#include "PlayerCharacterAnimInstance.generated.h"


// 전방선언
class ABaseCharacter;


/**
 * 
 */
// Player 캐릭터에 애님인스턴스

DECLARE_MULTICAST_DELEGATE(FOnAttackStartCheckDelegate)
DECLARE_MULTICAST_DELEGATE(FOnAttackEndCheckDelegate)
DECLARE_MULTICAST_DELEGATE(FOnFootSoundCheckDelegate)


UCLASS()
class PROJECT_API UPlayerCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPlayerCharacterAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void SetCurrentPawnSpeed(float speed) { m_fCurrentPawnSpeed = speed; }
	void SetIsPawnRun(bool run) { m_bPawnRun = run; }
	void SetIsPawnAir(bool air) { m_bIsInAir = air; }
	void SetIsDead(bool dead) { m_bIsDead = dead; }
	void SetPitch(float pitch) { m_fPitch = pitch; }

	void PlayAttackMontage();
	void PlayPickUpMontage();
	void PlayKeyMontage(float PlaySpeed);
	void PlayHealingMontage(float PlaySpeed);
	void PlayBleedHealingMontage(float PlaySpeed);
	void PlayJumpMontage();

	void PickUpStart();
	void PickUpEnd();

	void AttackStart();
	void AttackEnd();

	FOnAttackStartCheckDelegate OnAttackStartCheck;
	FOnAttackEndCheckDelegate OnAttackEndCheck;
	FOnFootSoundCheckDelegate OnFootSoundCheck;

	UAnimMontage* GetAttackMontage();
	UAnimMontage* GetPickupMontage();
	UAnimMontage* GetOpenKeyMontage();
	UAnimMontage* GetHealingMontage();
	UAnimMontage* GetBleedingMontage();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FootstepRunAudio", Transient)
	UAudioComponent* FootstepRunAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FootstepRunSound", Transient)
	TObjectPtr<USoundBase> FootstepRunSoundCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FootstepWalkAudio", Transient)
	UAudioComponent* FootstepWalkAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FootstepWalkSound", Transient)
	TObjectPtr<USoundBase> FootstepWalkSoundCue;

	UFUNCTION()
	void PlayFootstepRunSound();

	UFUNCTION()
	void StopFootstepRunSound();

	UFUNCTION()
	void PlayFootstepWalkSound();

	UFUNCTION()
	void StopFootstepWalkSound();

	UFUNCTION()
	void UpdateFootstepSound();

	//UFUNCTION()
	//void AnimNotify_OnFootstep();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Pawn, Transient)
	ABaseCharacter* OwnerCharacter;

	float LastStopRunTime;
	float LastStopWalkTime;


private:
	UFUNCTION()
	void AnimNotify_AttackStart();

	UFUNCTION()
	void AnimNotify_AttackEnd();

	UFUNCTION()
	void AnimNotify_FootSound1();

	UFUNCTION()
	void AnimNotify_FootSound2();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float m_fCurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool m_bPawnRun;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool m_bIsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool m_bIsDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool m_bIsHandInWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool m_bIsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool m_bIsPickUp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float m_fPitch;

	// 애니메이션 몽타주
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	// 애니메이션 몽타주
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = PickUp, Meta = (AllowPrivateAccess = true))
	UAnimMontage* PickUpMontage;

	// 애니메이션 몽타주
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = PickUp, Meta = (AllowPrivateAccess = true))
	UAnimMontage* HealingMontage;

	// 애니메이션 몽타주
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = PickUp, Meta = (AllowPrivateAccess = true))
	UAnimMontage* BleedHealingMontage;

	// 애니메이션 몽타주
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = PickUp, Meta = (AllowPrivateAccess = true))
	UAnimMontage* OpenKeyMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = PickUp, Meta = (AllowPrivateAccess = true))
	UAnimMontage* JumpMontage;

};
