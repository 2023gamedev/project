// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/HealingPB.h"

UHealingPB::UHealingPB(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	m_fCurrentHealing = 0.f;
	m_fMaxHealing = 100.f;

	//SetVisibility(ESlateVisibility::Hidden);
}

void UHealingPB::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Hidden);
}

void UHealingPB::UpdateHealingPB(float currentHealing)
{
	m_fCurrentHealing = currentHealing;
	m_fCurrentHealingRatio = FMath::Clamp(m_fCurrentHealing / m_fMaxHealing, 0.0f, 1.0f);

	// 힐링 상태가 아닐때는 해당 힐링 프로그래스바 숨기기 (+ 참고로 힐링이 다끝나도 여기서 걸리는데, 그게 BaseCharacter에서 SetHealing(0.f)으로 만들어줌)
	if (m_fCurrentHealingRatio == 0.f) {
		bIsHealing = false;
	}
	else {
		bIsHealing = true;
	}

	if (bWasHealing != bIsHealing) {
		SetVisibility(bIsHealing ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		bWasHealing = bIsHealing;
	}
}