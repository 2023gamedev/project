// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class REALPROJECT_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UCharacterAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void SetCurrentPawnSpeed(float speed) { m_fCurrentPawnSpeed = speed; }
	void SetIsPawnRun(bool run) { m_bPawnRun = run; }
	void SetIsPawnHandInBall(bool hinball) { m_bPawnHandInBall = hinball; }


private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float m_fCurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool m_bPawnRun;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool m_bPawnHandInBall;
};
