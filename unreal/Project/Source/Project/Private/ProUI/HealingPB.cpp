// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/HealingPB.h"

UHealingPB::UHealingPB(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	m_fCurrentHealing = 100.f;
	m_fMaxHealing = 100.f;
}

void UHealingPB::NativeConstruct()
{
	Super::NativeConstruct();
}

void UHealingPB::UpdateHealingPB(float currentHealing)
{
	m_fCurrentHealing = currentHealing;
	m_fCurrentHealingRatio = FMath::Clamp(m_fCurrentHealing / m_fMaxHealing, 0.0f, 1.0f);
}