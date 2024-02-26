// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayerUI.h"
#include "InventoryUI.h"

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
}

void UGamePlayerUI::Update()
{
	if (Inventory) {
		Inventory->Update();
	}
}

void UGamePlayerUI::RefreshInventory(int slotindex)
{
	Inventory->RefreshSlot(slotindex);
}
