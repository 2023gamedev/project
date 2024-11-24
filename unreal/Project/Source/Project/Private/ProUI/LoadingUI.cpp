// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/LoadingUI.h"

ULoadingUI::ULoadingUI(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void ULoadingUI::NativeConstruct()
{
	m_fCurrentLoading = 0.f;
	m_fMaxLoading = 100.f;
}

void ULoadingUI::UpdateLoadingBar(float currentloading, float maxloading)
{
	if (maxloading <= 0.f) {
		return;
	}
	// 최대 로딩 값 업데이트
	m_fMaxLoading = maxloading;

	m_fCurrentLoading = currentloading;
	m_fCurrentLoadingRatio = FMath::Clamp(m_fCurrentLoading / m_fMaxLoading, 0.0f, 1.0f);

}
