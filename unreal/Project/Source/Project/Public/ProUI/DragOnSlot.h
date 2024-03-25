// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ProData/ItemDataStructure.h"
#include "DragOnSlot.generated.h"

/**
 * 
 */

// Slot에 있는 아이템을 Drag하기 위해 있는 Class

UCLASS()
class PROJECT_API UDragOnSlot : public UDragDropOperation
{
	GENERATED_BODY()
	
	UDragOnSlot() {};

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SlotIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ESlotType> Type;
};
