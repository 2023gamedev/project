// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseUI.h"
#include "HPBar.generated.h"

/**
 * 
 */
// HPBar


UCLASS()
class PROJECT_API UHPBar : public UBaseUI
{
	GENERATED_BODY()

public:
    UHPBar(const FObjectInitializer& ObjectInitializer);

    virtual void NativeConstruct() override;


    UFUNCTION(BlueprintCallable, Category = "HealthBar")
    void UpdateHealthBar(float CurrentHealth, float MaxHealth);

    UPROPERTY(BlueprintReadWrite, Category = "HPBar")
    float m_fCurrentHPRatio = 0.f;

    // 현재 체력
    UPROPERTY(BlueprintReadWrite, Category = "HPBar")
    float m_fCurrentHealth = 0.f;

    // 최대 체력
    UPROPERTY(BlueprintReadWrite, Category = "HPBar")
    float m_fMaxHealth = 0.f;
};
