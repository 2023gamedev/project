// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/OtherPlayerUI.h"
#include "Components/Image.h"
#include "ProUI/HPBar.h"
#include "ProUI/Slot.h"
#include "Engine/Texture2D.h"      
#include "Slate/SlateBrushAsset.h"    
#include "UObject/ConstructorHelpers.h"
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


void UOtherPlayerUI::UpdateOtherPlayerUI(float hp, uint32 charactertype)
{
	if (!m_bPlayerImg) {
		m_bPlayerImg = true;

		if (charactertype == 0) {
			UTexture2D* LoadedTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/CharacterAsset/Girl/girl.girl"));
			if (LoadedTexture) {

				FSlateBrush Brush;
				Brush.SetResourceObject(LoadedTexture);   // 텍스처를 브러시에 설정
				Brush.ImageSize = FVector2D(64.0f, 32.0f); // 이미지 크기 설정 (가로 128, 세로 128)
				OtherPlayerImg->SetBrush(Brush);      // UImage에 브러시 설정

			}
		}
		else if (charactertype == 1) {
			UTexture2D* LoadedTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/CharacterAsset/Employee/emp.emp"));
			if (LoadedTexture) {

				FSlateBrush Brush;
				Brush.SetResourceObject(LoadedTexture);   // 텍스처를 브러시에 설정
				OtherPlayerImg->SetBrush(Brush);      // UImage에 브러시 설정

			}
		}
		else if (charactertype == 2) {
			UTexture2D* LoadedTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/CharacterAsset/Idol/idol.idol"));
			if (LoadedTexture) {

				FSlateBrush Brush;
				Brush.SetResourceObject(LoadedTexture);   // 텍스처를 브러시에 설정
				OtherPlayerImg->SetBrush(Brush);      // UImage에 브러시 설정

			}
		}
		else if (charactertype == 3) {
			UTexture2D* LoadedTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/CharacterAsset/FireFighter/fire.fire"));
			if (LoadedTexture) {

				FSlateBrush Brush;
				Brush.SetResourceObject(LoadedTexture);   // 텍스처를 브러시에 설정
				OtherPlayerImg->SetBrush(Brush);      // UImage에 브러시 설정

			}
		}
	}

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
