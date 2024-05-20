// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/PickUpUI.h"
#include "ProUI/Slot.h"
#include "Blueprint/WidgetTree.h"

UPickUpUI::UPickUpUI(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UPickUpUI::Init()
{

	Character->PickUpSlot.Add(FItemDataStructure());



	if (Character->PickUpSlot[0].Type == EItemType::ITEM_NONE) {
		Character->PickUpSlot[0].Texture = DefTex;
	}


	Slot.Init(nullptr, 1);
	TArray<UWidget*> widgets;
	WidgetTree->GetAllWidgets(widgets);

	USlot* slot = nullptr;

	for (auto widget : widgets) {
		slot = Cast<USlot>(widget);

		if (!slot) {
			continue;
		}
		slot->Character = this->Character;
		slot->SetType(ESlotType::SLOT_PICK_UP);
		slot->Init();
		Slot[slot->SlotIndex] = slot;
	}
}

void UPickUpUI::Update()
{
	Slot[0]->Update();


	FName ItemName = Character->PickUpSlot[0].Name;

	FText FormattedText = FText::Format(FText::FromString(TEXT("\n ItemPickUp : {0} ")), FText::FromName(ItemName));

	ItemTxt->SetText(FormattedText);
}

void UPickUpUI::RefreshSlot(int slotindex)
{
	Slot[0]->Refresh();
}
