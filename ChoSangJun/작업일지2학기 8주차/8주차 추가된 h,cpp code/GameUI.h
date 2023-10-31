// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomUI.h"
#include "GameUI.generated.h"

class UInventory;

/**
 * 
 */
UCLASS()
class REALPROJECT_API UGameUI : public UCustomUI
{
	GENERATED_BODY()

public:
	UGameUI(const FObjectInitializer& ObejctInitializer);

	void Init() override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (BindWidget))
	UInventory* Inventory;
};
