// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseUI.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "ProData/ItemDataStructure.h"
#include "Components/ProgressBar.h"
#include "ProItem/NormalWeaponActor.h"
#include "Components/Border.h"
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
	void Init() override;
	
	void Update() override;
	void SetType(ESlotType type);
	void SetTexture(UTexture2D* tex);

	void Refresh();
	void InitOPU(int otherplayeruiindex);
	void RefreshOPU(int otherplayeruiindex);

	void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
	
	//void SpawnOnGround(int slotindex);

	float GetWeaponDurabilityPercent() const
	{
		float percent = 0.f;

		//UE_LOG(LogTemp, Warning, TEXT("GetWeaponDurabilityPercent() Durability - %f"), WeaponDurability);
		//UE_LOG(LogTemp, Warning, TEXT("GetWeaponDurabilityPercent() Durability_Max - %f"), WeaponDurability_Max);

		if (WeaponDurability > 0.f && WeaponDurability_Max > 0.f) {
			percent = WeaponDurability / WeaponDurability_Max;
			//UE_LOG(LogTemp, Warning, TEXT("GetWeaponDurabilityPercent() - %f"), percent);
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("GetWeaponDurabilityPercent() WeaponDurability/WeaponDurability_Max is Zero!!!"));
		}
		
		return percent;
	}
	

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<USlot> DragVisualClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (UIMax = 19, UIMin = -1))
	int SlotIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WeaponDurability = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WeaponDurability_Max = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDragging;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESlotType Type;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* Img;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Text;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* ProgressBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UBorder* Border;

};
