// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/OtherPlayerUI.h"
#include "Components/Image.h"
#include "ProUI/HPBar.h"
#include "ProUI/Slot.h"
#include "Blueprint/WidgetTree.h"

UOtherPlayerUI::UOtherPlayerUI(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UOtherPlayerUI::Init()
{
	/*for (int i = 0; i < 2; ++i) {
		Character->Inventory.Add(FItemDataStructure());
	}

	for (int i = 0; i < 5; ++i) {
		if (Character->Inventory[i].Type == EItemType::ITEM_NONE) {
			Character->Inventory[i].Texture = DefTex;
		}
	}
	for (int i = 5; i < 20; ++i) {
		if (Character->Inventory[i].Type == EItemType::ITEM_NONE) {
			Character->Inventory[i].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/EngineResources/Black.Black"));
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
	}*/
}

void UOtherPlayerUI::Update()
{
}

void UOtherPlayerUI::RefreshSlot(int slotindex)
{
}
