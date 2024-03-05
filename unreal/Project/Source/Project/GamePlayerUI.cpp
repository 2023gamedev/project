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
	Inventory->RefreshSlot(slotindex); // �κ��丮 �ʿ��� ��ȭ�� ������ �� �������� �����ϵ��� �ؾ߰���

	// �ӽ�
	if (slotindex < 5) {
		QuickSlot->RefreshSlot(slotindex);
	}
}
