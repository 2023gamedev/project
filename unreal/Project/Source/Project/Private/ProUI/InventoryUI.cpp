// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/InventoryUI.h"
#include "ProUI/Slot.h"
#include "Blueprint/WidgetTree.h"

UInventoryUI::UInventoryUI(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UInventoryUI::Init()
{
	for (int i = 0; i < 20; ++i) {
		Character->Inventory.Add(FItemDataStructure());
	}

	for (int i = 0; i < 20; ++i) {
		if (Character->Inventory[i].Type == EItemType::ITEM_NONE) {
			Character->Inventory[i].Texture = DefTex;
		}
	}

	Slots.Init(nullptr, 20);
	TArray<UWidget*> widgets;
	WidgetTree->GetAllWidgets(widgets);

	USlot* slot = nullptr;

	for (auto widget : widgets) {
		slot = Cast<USlot>(widget);

		if (!slot) {
			continue;
		}
		slot->Character = this->Character;
		slot->SetType(ESlotType::SLOT_ITEM);
		slot->Init();
		Slots[slot->SlotIndex] = slot;
	}
}

void UInventoryUI::Update()
{
	for (int i = 0; i < 20; ++i) {
		Slots[i]->Update();
	}
}

void UInventoryUI::RefreshSlot(int slotindex)
{
	if (Character->Inventory[slotindex].Type == EItemType::ITEM_NONE) {
		Character->Inventory[slotindex].Texture = DefTex;
	}

	Slots[slotindex]->Refresh();
}
