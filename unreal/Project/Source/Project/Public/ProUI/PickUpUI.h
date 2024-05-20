// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProUI/BaseUI.h"
#include "PickUpUI.generated.h"

/**
 * 
 */

class USlot;
class UTextBlock;

UCLASS()
class PROJECT_API UPickUpUI : public UBaseUI
{
	GENERATED_BODY()

public:
	UPickUpUI(const FObjectInitializer& ObjectInitializer);

	void Init() override;
	void Update() override;

	void RefreshSlot(int slotindex);

public:
	// 텍스쳐 필요
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* DefTex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ItemTxt;


private:
	TArray<USlot*> Slot;
};
