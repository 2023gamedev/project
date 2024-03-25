// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDataStructure.generated.h"

/**
 * 
 */

// Inventory Slot�� ����ִ� �Ӽ���(����ü,Enum)�� ����ִ� .h

class USlot;

class PROJECT_API ItemDataStructure
{
public:
	ItemDataStructure();
	~ItemDataStructure();
};

UENUM(BlueprintType)
enum class EItemType
{
	ITEM_NONE				UMETA(DisplayName = "None"),
	ITEM_USEABLE			UMETA(DisplayName = "Useable"),
	ITEM_EQUIPMENT			UMETA(DisplayName = "Equipment"),
	ITEM_QUICK_NONE			UMETA(DisplayName = "QuickNone"),
	ITEM_QUICK_EQUIPMENT	UMETA(DisplayName = "QuickEquipment"),
};

UENUM(BlueprintType)
enum class EItemClass
{
	NORMALWEAPON			UMETA(DisplayName = "NormalWeapon"),
	THROWINGWEAPON			UMETA(DisplayName = "ThrowingWeapon"),
	HEALINGITEM				UMETA(DisplayName = "HealingItem"),
	BLEEDINGHEALINGITEM		UMETA(DisplayName = "BleedingHealingItem"),
	KEYITEM					UMETA(DisplayName = "KeyItem"),
	NONE					UMETA(DisplayName = "KeyItem"),
};

USTRUCT(Atomic, BlueprintType)
struct FItemDataStructure
{
	// ���Կ� ���Ǵ� ����ü
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) // ���� ���������� �������� �븻��������, ��ô��������
	TEnumAsByte<EItemClass> ItemClassType;

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
	SLOT_QUICK_ITEM			UMETA(DisplayName = "QuickItem"),
};