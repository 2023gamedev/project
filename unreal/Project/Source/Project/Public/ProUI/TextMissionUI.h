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

    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* FadeOutMT1; // missiontext2

    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* FadeOutMT2; // missiontext5

    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* FadeOutMTitle1; // missiontext1

    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* FadeOutMTitle2; // missiontext4

    UPROPERTY(meta = (BindWidgetAnim), Transient)
    UWidgetAnimation* FadeOutMT11; // missiontext3

    void PlayFadeOutAnimation();


    void PlayFadeOutMT1();

    void PlayFadeOutMT2();

    void PlayFadeOutMTitle1();

    void PlayFadeOutMTitle2();

    void PlayFadeOutMT11();

    int m_iFindRoofKey = 0;
    int m_iFindCarKey = 0;
};
