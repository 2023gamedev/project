// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseUI.h"
#include "GamePlayerUI.generated.h"

/**
 * 
 */

class UInventoryUI;
// ĳ����(�÷��̾�)�� ���� ��� UI �� ����
UCLASS()
class PROJECT_API UGamePlayerUI : public UBaseUI
{
	GENERATED_BODY()
	

public:
	UGamePlayerUI(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (BintWidget))
	UInventoryUI* Inventory;

	// ������, ü�¹�, ���¹̳� �κ��丮 ���
};
