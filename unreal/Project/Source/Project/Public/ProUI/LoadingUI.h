// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API ULoadingUI : public UUserWidget
{
	GENERATED_BODY()
public:
    ULoadingUI(const FObjectInitializer& ObjectInitializer);

    virtual void NativeConstruct() override;


    UFUNCTION(BlueprintCallable, Category = "loadingui")
    void UpdateLoadingBar(float currentloading, float maxloading);



    // 현재 체력
    UPROPERTY(BlueprintReadWrite, Category = "loadingui")
    float m_fCurrentLoadingRatio = 0.f;


    // 현재 체력
    UPROPERTY(BlueprintReadWrite, Category = "loadingui")
    float m_fCurrentLoading = 0.f;

    // 최대 체력
    UPROPERTY(BlueprintReadWrite, Category = "loadingui")
    float m_fMaxLoading = 100.f;
};
