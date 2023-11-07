// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ZombieAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class REALPROJECT_API UZombieAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UZombieAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void SetCurrentPawnSpeed(float speed) { m_fCurrentPawnSpeed = speed; }

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		float m_fCurrentPawnSpeed;
};
