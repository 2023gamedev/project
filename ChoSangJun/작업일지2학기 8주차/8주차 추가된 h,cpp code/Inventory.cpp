// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include "Slot.h"
#include "Blueprint/WidgetTree.h"


UInventory::UInventory(const FObjectInitializer& ObejctInitializer)
	:Super(ObejctInitializer)
{
}

void UInventory::Init()
{
	// 슬롯을 생성하여 TArray에 추가합니다.
	for (int i = 0; i < 32; ++i) {
		Player->Inventory.Add(FItemData());
	}

	for (int i = 0; i < 32; ++i) {
		if (Player->Inventory[i].Type == EItemType::ITEM_None) {
			Player->Inventory[i].Texture = DefTex;
		}
	}

	Slots.Init(nullptr, 32);
	TArray<UWidget*> widgets;
	WidgetTree->GetAllWidgets(widgets);

	USlot* slot = nullptr;

	for (auto widget : widgets) {
		slot = Cast<USlot>(widget);

		if (!slot) continue;
		slot->Player = this->Player;
		slot->SetType(ESlotType::SLOT_Item);
		slot->Init();
		Slots[slot->Slotnum] = slot;
	}
}
