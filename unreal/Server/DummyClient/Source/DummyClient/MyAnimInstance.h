// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DUMMYCLIENT_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMyAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void SetCurrentPawnSpeed(float speed) { m_fCurrentPawnSpeed = speed; }

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllodwPrivateAccess = true))
		float m_fCurrentPawnSpeed;
	
};
