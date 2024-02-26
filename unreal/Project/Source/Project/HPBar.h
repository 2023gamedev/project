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

    // ���� ü��
    UPROPERTY(BlueprintReadOnly, Category = "HPBar")
    float m_fCurrentHealth;

    // �ִ� ü��
    UPROPERTY(BlueprintReadOnly, Category = "HPBar")
    float m_fMaxHealth;
};
