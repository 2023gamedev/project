// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ZombieAnimInstance.generated.h"

/**
 * 
 */
// ZombieAnimInstance 

DECLARE_MULTICAST_DELEGATE(FOnAttackStartCheckDelegate)
//DECLARE_MULTICAST_DELEGATE(FOnAttackEndCheckDelegate)

UCLASS()
class PROJECT_API UZombieAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UZombieAnimInstance();

	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void SetCurrentPawnSpeed(float speed) { m_fCurrentPawnSpeed = speed; }
	void SetIsNormalDead(bool dead) { m_bIsNormalDead = dead; }
	void SetIsCuttingDead(bool dead) { m_bIsCuttingDead = dead; }
	void SetIsStanding(bool stand) { m_bIsStanding = stand; }

	void PlayAttackMontage();
	void PlayShoutingMontage();
	void PlayBeAttackedMontage();

	FOnAttackStartCheckDelegate OnAttackStartCheck;
	//FOnAttackEndCheckDelegate OnAttackEndCheck;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllodwPrivateAccess = true))
	float m_fCurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllodwPrivateAccess = true))
	bool m_bIsNormalDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllodwPrivateAccess = true))
	bool m_bIsCuttingDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllodwPrivateAccess = true))
	bool m_bIsStanding;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category= Attack, Meta=(AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* ShoutingMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* BeAttackedMontage;


private:
	UFUNCTION()
	void AnimNotify_AttackStart();

	//UFUNCTION()
	//void AnimNotify_AttackEnd();
};
