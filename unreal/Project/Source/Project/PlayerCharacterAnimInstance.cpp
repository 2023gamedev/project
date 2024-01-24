// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterAnimInstance.h"


UPlayerCharacterAnimInstance::UPlayerCharacterAnimInstance()
{
	m_fCurrentPawnSpeed = 0.f;
	m_bPawnRun = false;

}

void UPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

}