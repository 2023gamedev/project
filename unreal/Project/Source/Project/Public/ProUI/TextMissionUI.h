// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TextMissionUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UTextMissionUI : public UUserWidget
{
	GENERATED_BODY()

public:
   
    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* FadeOutAnimation;


    void PlayFadeOutAnimation();
};
