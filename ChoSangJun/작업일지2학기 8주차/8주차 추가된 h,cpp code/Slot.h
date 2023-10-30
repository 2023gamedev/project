// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CustomUI.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "DataStructure.h"
#include "Slot.generated.h"


UCLASS()
class REALPROJECT_API USlot : public UCustomUI
{
	GENERATED_BODY()

public:
	void Init() override;
	void SetType(ESlotType type);
	void SetTexture(UTexture2D* tex);

	void Refresh();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMAX = 31, UIMin = -1))
	int Slotnum;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int Count;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TEnumAsByte<ESlotType> Type;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* Img;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Text;

};
