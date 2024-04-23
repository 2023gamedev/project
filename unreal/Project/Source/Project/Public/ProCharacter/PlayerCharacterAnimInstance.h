// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerCharacterAnimInstance.generated.h"

/**
 * 
 */
// Player 캐릭터에 애님인스턴스

DECLARE_MULTICAST_DELEGATE(FOnAttackStartCheckDelegate)
DECLARE_MULTICAST_DELEGATE(FOnAttackEndCheckDelegate)

UCLASS()
class PROJECT_API UPlayerCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPlayerCharacterAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void SetCurrentPawnSpeed(float speed) { m_fCurrentPawnSpeed = speed; }
	void SetIsPawnRun(bool run) { m_bPawnRun = run; }

	void PlayAttackMontage();

	FOnAttackStartCheckDelegate OnAttackStartCheck;
	FOnAttackEndCheckDelegate OnAttackEndCheck;
private:
	UFUNCTION()
	void AnimNotify_AttackStart();

	UFUNCTION()
	void AnimNotify_AttackEnd();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float m_fCurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool m_bPawnRun;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool m_bIsInAir;

	// 애니메이션 몽타주
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;
};
