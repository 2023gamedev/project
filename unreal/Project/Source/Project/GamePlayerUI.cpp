// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayerUI.h"
#include "InventoryUI.h"
#include "QuickSlotUI.h"
#include "ConditionUI.h"

UGamePlayerUI::UGamePlayerUI(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UGamePlayerUI::Init()
{
	if (Inventory) {
		Inventory->Character = this->Character;
		Inventory->Init();
	}
	if (QuickSlot) {
		QuickSlot->Character = this->Character;
		QuickSlot->Init();
	}
}

void UGamePlayerUI::Update()
{
	if (Inventory) {
		Inventory->Update();
	}
	if (QuickSlot) {
		QuickSlot->Update();
	}
}

void UGamePlayerUI::RefreshInventory(int slotindex)
{
	Inventory->RefreshSlot(slotindex); // 인벤토리 쪽에서 변화가 생겼을 때 퀵슬롯을 수정하도록 해야겠지

	// 임시
	if (slotindex < 5) {
		QuickSlot->RefreshSlot(slotindex);
	}
}
