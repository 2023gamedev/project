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

// Player 캐릭터의 컨디션을 나타내는 Class

UCLASS()
class PROJECT_API UConditionUI : public UBaseUI
{
	GENERATED_BODY()

    UConditionUI(const FObjectInitializer& ObjectInitializer);

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
    UHPBar* HPBar;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
    UStaminaBar* StaminaBar;
};
