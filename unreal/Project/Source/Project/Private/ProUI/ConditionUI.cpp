// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/ConditionUI.h"
#include "ProUI/HPBar.h"
#include "ProUI/StaminaBar.h"
#include "Components/Image.h"
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

void UConditionUI::BloodImageVisible(ESlateVisibility visibillity)
{
	BloodImg->SetVisibility(visibillity);
}

void UConditionUI::SmokingImageVisible(ESlateVisibility visibillity)
{
	SmokingImg->SetVisibility(visibillity);
}

void UConditionUI::NativeConstruct()
{
	Super::NativeConstruct();

	UTexture2D* LoadedTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/bloodicon.bloodicon"));
	if (LoadedTexture) {

		FSlateBrush Brush;
		Brush.SetResourceObject(LoadedTexture);   
		BloodImg->SetBrush(Brush);     
		BloodImg->SetVisibility(ESlateVisibility::Hidden);
	}

	UTexture2D* LoadedSmokingTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/smokingicon.smokingicon"));
	if (LoadedSmokingTexture) {

		FSlateBrush BrushS;
		BrushS.SetResourceObject(LoadedSmokingTexture);
		SmokingImg->SetBrush(BrushS);
		SmokingImg->SetVisibility(ESlateVisibility::Hidden);
	}
}
