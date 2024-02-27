// Fill out your copyright notice in the Description page of Project Settings.


#include "HPBar.h"

UHPBar::UHPBar(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	if (Character != nullptr) {
		m_fCurrentHealth = Character->GetHP();
		m_fMaxHealth = Character->GetHP();
	}

}

void UHPBar::UpdateHealthBar(float currenthealth)
{
	m_fCurrentHealth = currenthealth;
}
