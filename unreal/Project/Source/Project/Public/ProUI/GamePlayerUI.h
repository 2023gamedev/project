// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseUI.h"
#include "GamePlayerUI.generated.h"

/**
 * 
 */

class UInventoryUI;
class UConditionUI;
class UQuickSlotUI;

// 캐릭터(플레이어)가 쓰는 모든 UI 들어갈 예정
UCLASS()
class PROJECT_API UGamePlayerUI : public UBaseUI
{
	GENERATED_BODY()


public:
	UGamePlayerUI(const FObjectInitializer& ObjectInitializer);

	void Init() override;
	void Update() override;

	void RefreshInventory(int slotindex);
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	UInventoryUI* Inventory;  

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (BindWidget)) // 업데이트 필요
	UConditionUI* Condition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (BindWidget)) // 업데이트 필요
	UQuickSlotUI* QuickSlot;

	// 퀵슬롯, 체력바, 스태미나 인벤토리 등등
};
