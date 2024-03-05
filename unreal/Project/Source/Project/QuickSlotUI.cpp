// Fill out your copyright notice in the Description page of Project Settings.


#include "QuickSlotUI.h"

#include "Slot.h"
#include "Blueprint/WidgetTree.h"

UQuickSlotUI::UQuickSlotUI(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UQuickSlotUI::Init()
{
	for (int i = 0; i < 5; ++i) {
		Character->QuickSlot.Add(FItemDataStructure());
		Character->QuickSlot[i].Type = EItemType::ITEM_QUICK_NONE;
	}

	for (int i = 0; i < 5; ++i) {
		if (Character->QuickSlot[i].Type == EItemType::ITEM_QUICK_NONE) {
			Character->QuickSlot[i].Texture = DefTex;
		}
	}

	Slots.Init(nullptr, 5);
	TArray<UWidget*> widgets;
	WidgetTree->GetAllWidgets(widgets);

	USlot* slot = nullptr;

	for (auto widget : widgets) {
		slot = Cast<USlot>(widget);

		if (!slot) {
			continue;
		}
		slot->Character = this->Character;
		slot->SetType(ESlotType::SLOT_QUICK);
		slot->Init();
		Slots[slot->SlotIndex] = slot;
	}
}

void UQuickSlotUI::Update()
{
	for (int i = 0; i < 5; ++i) {
		Slots[i]->Update();
	}
}

void UQuickSlotUI::RefreshSlot(int slotindex)
{

	if (Character->QuickSlot[slotindex].Type == EItemType::ITEM_QUICK_NONE) {
		Character->QuickSlot[slotindex].Texture = DefTex;
	}

	Slots[slotindex]->Refresh();
}
