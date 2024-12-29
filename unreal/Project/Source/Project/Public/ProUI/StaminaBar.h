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
    void UpdateStaminaBar(float CurrenStamina/*, float MaxStamina*/);

    UPROPERTY(BlueprintReadWrite, Category = "StaminaBar")
    float m_fCurrentStaminaRatio = 0.f;
  
    UPROPERTY(BlueprintReadWrite, Category = "StaminaBar")
    float m_fCurrentStamina = 0.f;

    UPROPERTY(BlueprintReadWrite, Category = "StaminaBar")
    float m_fMaxStamina = 0.f;
};
