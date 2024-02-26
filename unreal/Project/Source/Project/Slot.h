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
	void Init() override;
	void Update() override;
	void SetType(ESlotType type);
	void SetTexture(UTexture2D* tex);

	void Refresh();


	void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<USlot> DragVisualClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMax = 19, UIMin = -1))
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
