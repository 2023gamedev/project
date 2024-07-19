// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/ConditionUI.h"
#include "ProUI/HPBar.h"
#include "ProUI/StaminaBar.h"
#include "ProUI/HealingPB.h"

UConditionUI::UConditionUI(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UConditionUI::UpdateBar()
{
	HPBar->UpdateHealthBar(Character->GetHP(), Character->GetMaxHP());
	StaminaBar->UpdateStaminaBar(Character->GetStamina());
	HealingPB->UpdateHealingPB(Character->GetHealing());
}

void UConditionUI::NativeConstruct()
{
	Super::NativeConstruct();

}
