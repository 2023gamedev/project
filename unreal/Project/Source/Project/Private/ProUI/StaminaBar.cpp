// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/StaminaBar.h"

UStaminaBar::UStaminaBar(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UStaminaBar::NativeConstruct()
{
	Super::NativeConstruct();

	if (Character != nullptr) {
		m_fCurrentStamina = Character->GetStamina();
		m_fMaxStamina = Character->GetMaxStamina();
	}
}

void UStaminaBar::UpdateStaminaBar(float CurrentStamina, float MaxStamina)
{
	m_fMaxStamina = MaxStamina;
	if (m_fMaxStamina <= 0.f) {
		return;
	}

	m_fCurrentStamina = CurrentStamina;
	m_fCurrentStaminaRatio = FMath::Clamp(m_fCurrentStamina / m_fMaxStamina, 0.0f, 1.0f);
}
