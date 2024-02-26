// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseUI.h"
#include "InventoryUI.generated.h"

/**
 * 
 */
class USlot;

// 인벤토리 클래스
UCLASS()
class PROJECT_API UInventoryUI : public UBaseUI
{
	GENERATED_BODY()
	
public:
	UInventoryUI(const FObjectInitializer& ObjectInitializer);

	void Init() override;
	//void Update() override;

public:
	// 텍스쳐 필요
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* DefTex;

private:
	TArray<USlot*> Slots;
};
