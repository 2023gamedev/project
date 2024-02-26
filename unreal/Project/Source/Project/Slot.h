// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseUI.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "ItemDataStructure.h"
#include "Slot.generated.h"

/**
 * 
 */
// ½½·Ô Å¬·¡½º
UCLASS()
class PROJECT_API USlot : public UBaseUI
{
	GENERATED_BODY()


public:
	virtual void Init() override;
	//void Updata() override;
	void SetType(ESlotType type);
	void SetTexture(UTexture2D* tex);

	void Refresh();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SlotIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ESlotType> Type;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* Img;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Text;
};
