// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseUI.h"
#include "StaminaBar.generated.h"

/**
 * 
 */
// StaminaBar

UCLASS()
class PROJECT_API UStaminaBar : public UBaseUI
{
	GENERATED_BODY()

public:
    UStaminaBar(const FObjectInitializer& ObjectInitializer);

    virtual void NativeConstruct() override;


    UFUNCTION(BlueprintCallable, Category = "StaminaBar")
    void UpdateStaminaBar(float currenstamina);

    UPROPERTY(BlueprintReadWrite, Category = "StaminaBar")
    float m_fCurrentStaminaRatio;
  
    UPROPERTY(BlueprintReadWrite, Category = "StaminaBar")
    float m_fCurrentStamina;

    UPROPERTY(BlueprintReadWrite, Category = "StaminaBar")
    float m_fMaxStamina;
};
