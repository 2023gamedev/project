// Fill out your copyright notice in the Description page of Project Settings.


#include "GameUI.h"
#include "Inventory.h"




UGameUI::UGameUI(const FObjectInitializer& ObejctInitializer)
	:Super(ObejctInitializer)
{
}

void UGameUI::Init()
{
	if (Inventory) {
		Inventory->Player = this->Player;
		Inventory->Init();
	}
}
