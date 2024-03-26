// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerCharacterAnimInstance.generated.h"

/**
 * 
 */
// Player ĳ���Ϳ� �ִ��ν��Ͻ�
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

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float m_fCurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool m_bPawnRun;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool m_bIsInAir;

	// �ִϸ��̼� ��Ÿ��
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;
};
