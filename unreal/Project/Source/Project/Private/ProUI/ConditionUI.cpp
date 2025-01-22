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
	StaminaBar->UpdateStaminaBar(Character->GetStamina(), Character->GetMaxStamina());
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

void UConditionUI::WalkImageVisible(ESlateVisibility visibillity)
{
	WalkImg->SetVisibility(visibillity);
}

void UConditionUI::RunImageVisible(ESlateVisibility visibillity)
{
	RunImg->SetVisibility(visibillity);
}

void UConditionUI::KeyImageVisible(ESlateVisibility visibillity)
{
	KeyImg->SetVisibility(visibillity);
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

	UTexture2D* LoadedRunningTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/runicon.runicon"));
	if (LoadedRunningTexture) {

		FSlateBrush BrushR;
		BrushR.SetResourceObject(LoadedRunningTexture);
		RunImg->SetBrush(BrushR);
		RunImg->SetVisibility(ESlateVisibility::Hidden);
	}

	UTexture2D* LoadedWalkingTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/walkicon.walkicon"));
	if (LoadedWalkingTexture) {

		FSlateBrush BrushW;
		BrushW.SetResourceObject(LoadedWalkingTexture);
		WalkImg->SetBrush(BrushW);
		WalkImg->SetVisibility(ESlateVisibility::Visible);
	}

	UTexture2D* LoadedKeyTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/keyicon.keyicon"));
	if (LoadedKeyTexture) {

		FSlateBrush BrushS;
		BrushS.SetResourceObject(LoadedKeyTexture);
		KeyImg->SetBrush(BrushS);
		KeyImg->SetVisibility(ESlateVisibility::Hidden);
	}
}
