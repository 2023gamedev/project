// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProUI/BaseUI.h"
#include "GameTimerUI.generated.h"

/**
 * 
 */
class UTextBlock;
UCLASS()
class PROJECT_API UGameTimerUI : public UBaseUI
{
    GENERATED_BODY()

public:
    UGameTimerUI(const FObjectInitializer& ObjectInitializer);

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* TimerTxt;


    UFUNCTION(BlueprintCallable)
    void UpdateTimer();

    UFUNCTION(BlueprintCallable)
    void SetTimerStart();

    bool IsTimeUp() { return m_iMinites <= 0 && m_iSeconds; }

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float m_fStartTimer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 m_iMinites;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 m_iSeconds;

protected:
    virtual void NativeConstruct() override;
};
