// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseUI.h"
#include "ConditionUI.generated.h"

/**
 * 
 */

class UHPBar;
class UStaminaBar;
class UHealingPB;

// Player 캐릭터의 컨디션을 나타내는 Class

UCLASS()
class PROJECT_API UConditionUI : public UBaseUI
{
	GENERATED_BODY()

    UConditionUI(const FObjectInitializer& ObjectInitializer);

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UHealingPB* HealingPB;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UHPBar* HPBar;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UStaminaBar* StaminaBar;

    void UpdateBar();
    virtual void NativeConstruct() override;
};
