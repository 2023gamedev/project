// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/HPBar.h"


UHPBar::UHPBar(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UHPBar::NativeConstruct()
{
	Super::NativeConstruct();

	if (Character != nullptr) {
		m_fCurrentHealth = Character->GetHP();
		m_fMaxHealth = Character->GetMaxHP();
	}
}

void UHPBar::UpdateHealthBar(float currenthealth, float MaxHealth)
{
	m_fMaxHealth = MaxHealth;
	if (m_fMaxHealth <= 0.f) {
		return;
	}

	m_fCurrentHealth = currenthealth;
	m_fCurrentHPRatio = FMath::Clamp(m_fCurrentHealth / m_fMaxHealth, 0.0f, 1.0f);

}

