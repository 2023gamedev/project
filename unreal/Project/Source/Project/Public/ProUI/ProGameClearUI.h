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
    UTextBlock* Message;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
    UTextBlock* SText;

    UFUNCTION(BlueprintCallable)
    void SetMessage(const FString& mes);

    UFUNCTION(BlueprintCallable)
    void SetScore(int32 Score);

 

protected:
    virtual void NativeConstruct() override;
};
