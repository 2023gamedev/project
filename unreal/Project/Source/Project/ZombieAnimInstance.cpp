// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieAnimInstance.h"

UZombieAnimInstance::UZombieAnimInstance()
{
	m_fCurrentPawnSpeed = 0.f;
}

void UZombieAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}
