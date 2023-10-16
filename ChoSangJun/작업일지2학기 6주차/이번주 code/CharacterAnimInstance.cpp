// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAnimInstance.h"

UCharacterAnimInstance::UCharacterAnimInstance()
{
	m_fCurrentPawnSpeed = 0.f;
	m_bPawnRun = false;
	m_bPawnHandInBall = false;

}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

}
