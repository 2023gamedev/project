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

// ĳ����(�÷��̾�)�� ���� ��� UI �� ����
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (BindWidget)) // ������Ʈ �ʿ�
	UConditionUI* Condition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (BindWidget)) // ������Ʈ �ʿ�
	UQuickSlotUI* QuickSlot;

	// ������, ü�¹�, ���¹̳� �κ��丮 ���
};
