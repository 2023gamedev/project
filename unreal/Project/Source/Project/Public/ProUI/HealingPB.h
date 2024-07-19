// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseUI.h"
#include "HealingPB.generated.h"

/**
 * 
 */

UCLASS()
class PROJECT_API UHealingPB : public UBaseUI
{
	GENERATED_BODY()
	
public:
    UHealingPB(const FObjectInitializer& ObjectInitializer);

    virtual void NativeConstruct() override;


    UFUNCTION(BlueprintCallable, Category = "HealingPB")
    void UpdateHealingPB(float currentHealing);

    UPROPERTY(BlueprintReadWrite, Category = "HealingPB")
    float m_fCurrentHealingRatio;

    UPROPERTY(BlueprintReadWrite, Category = "HealingPB")
    float m_fCurrentHealing;

    UPROPERTY(BlueprintReadWrite, Category = "HealingPB")
    float m_fMaxHealing;
};
