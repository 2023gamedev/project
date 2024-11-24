// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProUI/BaseUI.h"
#include "ProGameClearUI.generated.h"

/**
 * 
 */
class UTextBlock;
UCLASS()
class PROJECT_API UProGameClearUI : public UBaseUI
{
	GENERATED_BODY()

public:
    UProGameClearUI(const FObjectInitializer& ObjectInitializer);

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* OneAnswer;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* TwoAnswer;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* ThreeAnswer;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* ThreeAnswer2;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* FourAnswer;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* FiveAnswer;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* FiveAnswer2;


    UFUNCTION(BlueprintCallable)
    void SetMessage(int index, const FString& mes);

    UFUNCTION(BlueprintCallable)
    void SetScore(int index, int32 Score);

 

protected:
    virtual void NativeConstruct() override;
};
