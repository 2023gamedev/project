#pragma once

#include "CoreMinimal.h"
//#include "PlayerCharacter.h"
#include "DataStructure.generated.h"


UENUM(BlueprintType)
enum EItemType
{
	ITEM_None			UMETA(DisplayName = "None"),
	ITEM_Useable		UMETA(DisplayName = "Useable"),
	ITEM_Equipment		UMETA(DisplayName = "Equipment")
};


USTRUCT(BlueprintType)
struct FItemData
{

	GENERATED_USTRUCT_BODY()

public:
	
	FItemData()
		: Name(TEXT("")), Texture(nullptr), Type(EItemType::ITEM_None), Count(0)
	{};

	//virtual void Clear();
	//virtual void Use(APlayerCharacter* player) {};

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
enum ESlotType
{
	SLOT_None		UMETA(DisplayName = "None"),
	SLOT_Item		UMETA(DisplayName = "Item"),
};