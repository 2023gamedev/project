// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDataStructure.generated.h"

/**
 * 
 */
class PROJECT_API ItemDataStructure
{
public:
	ItemDataStructure();
	~ItemDataStructure();
};

UENUM(BlueprintType)
enum class EItemType
{
	ITEM_NONE			UMETA(DisplayName = "None"),
	ITEM_EQUIPMENT		UMETA(DisplayName = "EQUIPMENT")
};

USTRUCT(Atomic, BlueprintType)
struct FItemDataStructure
{

	GENERATED_USTRUCT_BODY()

public:


	// virtual void Clear();
	// virtual void Use(APlyaerCharacter* player) {};

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Texture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EItemType> Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Count;
};

UENUM(BlueprintType)
enum class ESlotType
{
	SLOT_NONE				UMETA(DisplayName = "None"),
	SLOT_ITEM				UMETA(DisplayName = "Item"),
	SLOT_QUICK				UMETA(DisplayName = "Quick"),
	SLOT_QUICK_ITEM			UMETA(DisplayName = "Item"),
};