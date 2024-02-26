// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseUI.h"
#include "GamePlayerUI.generated.h"

/**
 * 
 */

class UInventoryUI;
// 캐릭터(플레이어)가 쓰는 모든 UI 들어갈 예정
UCLASS()
class PROJECT_API UGamePlayerUI : public UBaseUI
{
	GENERATED_BODY()


public:
	UGamePlayerUI(const FObjectInitializer& ObjectInitializer);

	void Init() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	UInventoryUI* Inventory;  // 바인드 위젯을 하려면 bp랑 변수랑 이름이 같아야 하는 구나..

	// 퀵슬롯, 체력바, 스태미나 인벤토리 등등
};
