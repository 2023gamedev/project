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
	//if (m_iOtherPlayerUINumber == 1) {
	//	for (int i = 0; i < 2; ++i) {
	//		Character->OtherPlayerInven.Add(FItemDataStructure());
	//	}
	//}
	//else if (m_iOtherPlayerUINumber == 2) {
	//	for (int i = 0; i < 2; ++i) {
	//		Character->OtherPlayer2Inven.Add(FItemDataStructure());
	//	}
	//}
	//else if (m_iOtherPlayerUINumber == 3) {
	//	for (int i = 0; i < 2; ++i) {
	//		Character->OtherPlayer3Inven.Add(FItemDataStructure());
	//	}
	//}

	//Slots.Init(nullptr, 2);
	//TArray<UWidget*> widgets;
	//WidgetTree->GetAllWidgets(widgets);

	//USlot* slot = nullptr;

	//for (auto widget : widgets) {
	//	slot = Cast<USlot>(widget);

	//	if (!slot) {
	//		continue;
	//	}
	//	slot->Character = this->Character;
	//	slot->SetType(ESlotType::SLOT_OTHER_PLAYER);
	//	slot->Init();
	//	Slots[slot->SlotIndex] = slot;
	//}
}

void UOtherPlayerUI::Update()
{

}

void UOtherPlayerUI::UpdateOtherPlayerUI(float otherplayerid, float hp)
{

	UpdateHPBar(hp);
}

void UOtherPlayerUI::UpdateHPBar(float hp)
{
	if (iMaxHealthFlag == 0) {
		m_fMaxHealth = hp;

	}
	HPBar->UpdateHealthBar(hp, m_fMaxHealth);
}

void UOtherPlayerUI::RefreshSlot(int slotindex)
{
}
