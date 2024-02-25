// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseUI.h"
#include "StaminaBar.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UStaminaBar : public UBaseUI
{
	GENERATED_BODY()

public:
    UStaminaBar(const FObjectInitializer& ObjectInitializer);

    UFUNCTION(BlueprintCallable, Category = "HealthBar")
    void UpdateStaminaBar(float currenstamina);

    UPROPERTY(BlueprintReadOnly, Category = "HPBar")
    float m_fCurrentStamina;

    UPROPERTY(BlueprintReadOnly, Category = "HPBar")
    float m_fMaxStamina;
};
