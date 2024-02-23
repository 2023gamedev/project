// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseUI.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Slot.generated.h"

/**
 * 
 */
// 슬롯 클래스
UCLASS()
class PROJECT_API USlot : public UBaseUI
{
	GENERATED_BODY()


public:
	virtual void Init() override;



public:

	// 갯수, 번호, 텍스쳐 ...
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* Img;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Text;
};
