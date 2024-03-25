// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseUI.h"
#include "QuickSlotUI.generated.h"

/**
 * 
 */

class USlot;
// QuickSlot Ŭ����
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
	// �ؽ��� �ʿ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* DefTex;

private:
	TArray<USlot*> Slots; // ���� �� �������� 1�� �� �ټ��� ����
};
