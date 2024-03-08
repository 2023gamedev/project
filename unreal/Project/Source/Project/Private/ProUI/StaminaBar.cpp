// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/StaminaBar.h"

UStaminaBar::UStaminaBar(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	m_fCurrentStamina = 100.f;
	m_fMaxStamina	  = 100.f;
}

void UStaminaBar::UpdateStaminaBar(float currenstamina)
{
	m_fCurrentStamina = currenstamina;
}
