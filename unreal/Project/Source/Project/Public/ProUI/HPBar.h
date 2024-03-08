// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseUI.h"
#include "HPBar.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UHPBar : public UBaseUI
{
	GENERATED_BODY()

public:
    UHPBar(const FObjectInitializer& ObjectInitializer);

    UFUNCTION(BlueprintCallable, Category = "HealthBar")
    void UpdateHealthBar(float CurrentHealth);

    // 현재 체력
    UPROPERTY(BlueprintReadOnly, Category = "HPBar")
    float m_fCurrentHealth;

    // 최대 체력
    UPROPERTY(BlueprintReadOnly, Category = "HPBar")
    float m_fMaxHealth;
};
