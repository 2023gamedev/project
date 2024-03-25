// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseUI.h"
#include "QuickSlotUI.generated.h"

/**
 * 
 */

class USlot;
// QuickSlot 클래스
UCLASS()
class PROJECT_API UQuickSlotUI : public UBaseUI
{
	GENERATED_BODY()
public:
	UQuickSlotUI(const FObjectInitializer& ObjectInitializer);

	void Init() override;
	void Update() override;

	void RefreshSlot(int slotindex);

public:
	// 텍스쳐 필요
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* DefTex;

private:
	TArray<USlot*> Slots; // 슬롯 각 종류별로 1개 씩 다섯개 예정
};
