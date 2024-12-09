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
	if (m_iOtherPlayerUIIndex == 1) {
		for (int i = 0; i < 2; ++i) {
			Character->OtherPlayerInven.Add(FItemDataStructure());
		}
		for (int i = 0; i < 2; ++i) {
			Character->OtherPlayerInven[i].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
		}
	}
	else if (m_iOtherPlayerUIIndex == 2) {
		for (int i = 0; i < 2; ++i) {
			Character->OtherPlayer2Inven.Add(FItemDataStructure());
		}
		for (int i = 0; i < 2; ++i) {
			Character->OtherPlayer2Inven[i].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
		}
	}
	else if (m_iOtherPlayerUIIndex == 3) {
		for (int i = 0; i < 2; ++i) {
			Character->OtherPlayer3Inven.Add(FItemDataStructure());
		}
		for (int i = 0; i < 2; ++i) {
			Character->OtherPlayer3Inven[i].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
		}
	}

	Slots.Init(nullptr, 2);
	TArray<UWidget*> widgets;
	WidgetTree->GetAllWidgets(widgets);

	USlot* slot = nullptr;

	for (auto widget : widgets) {
		slot = Cast<USlot>(widget);

		if (!slot) {
			continue;
		}
		slot->Character = this->Character;
		slot->SetType(ESlotType::SLOT_OTHER_PLAYER);
		slot->InitOPU(m_iOtherPlayerUIIndex);
		Slots[slot->SlotIndex] = slot;
	}
}

void UOtherPlayerUI::Update()
{
	for (int i = 0; i < 2; ++i) {
		Slots[i]->RefreshOPU(m_iOtherPlayerUIIndex);
	}

}


void UOtherPlayerUI::UpdateOtherPlayerUI(float hp, uint32 charactertype, std::string playername)
{
	if (!m_bPlayerImg) {
		m_bPlayerImg = true;

		FString PlayerName = playername.c_str();
		if (PlayerName.IsEmpty()) {
			UE_LOG(LogTemp, Warning, TEXT("PlayerName is empty or null"));
		}
		else {
			UE_LOG(LogTemp, Display, TEXT("PlayerN: %s"), *PlayerName);
		}

		SetVisibility(ESlateVisibility::Visible);

		if (charactertype == 1) {
			UTexture2D* LoadedTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/CharacterAsset/Girl/girl.girl"));
			if (LoadedTexture) {

				FSlateBrush Brush;
				Brush.SetResourceObject(LoadedTexture);   // 텍스처를 브러시에 설정
				Brush.ImageSize = FVector2D(64.0f, 32.0f); // 이미지 크기 설정 (가로 128, 세로 128)
				OtherPlayerImg->SetBrush(Brush);      // UImage에 브러시 설정

			}
		}
		else if (charactertype == 2) {
			UTexture2D* LoadedTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/CharacterAsset/Employee/emp.emp"));
			if (LoadedTexture) {

				FSlateBrush Brush;
				Brush.SetResourceObject(LoadedTexture);   // 텍스처를 브러시에 설정
				Brush.ImageSize = FVector2D(64.0f, 32.0f); // 이미지 크기 설정 (가로 128, 세로 128)
				OtherPlayerImg->SetBrush(Brush);      // UImage에 브러시 설정

			}
		}
		else if (charactertype == 3) {
			UTexture2D* LoadedTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/CharacterAsset/Idol/idol.idol"));
			if (LoadedTexture) {

				FSlateBrush Brush;
				Brush.SetResourceObject(LoadedTexture);   // 텍스처를 브러시에 설정
				Brush.ImageSize = FVector2D(64.0f, 32.0f); // 이미지 크기 설정 (가로 128, 세로 128)
				OtherPlayerImg->SetBrush(Brush);      // UImage에 브러시 설정

			}
		}
		else if (charactertype == 4) {
			UTexture2D* LoadedTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/CharacterAsset/FireFighter/fire.fire"));
			if (LoadedTexture) {

				FSlateBrush Brush;
				Brush.SetResourceObject(LoadedTexture);   // 텍스처를 브러시에 설정
				Brush.ImageSize = FVector2D(64.0f, 32.0f); // 이미지 크기 설정 (가로 128, 세로 128)
				OtherPlayerImg->SetBrush(Brush);      // UImage에 브러시 설정

			}
		}
		m_iCharacterType = charactertype;

		FString PlayerN = FString(playername.c_str());
		OtherPlayerID->SetText(FText::FromString(PlayerN));

	}


	//FString PlayerName = playername.c_str();
	//if (PlayerName.IsEmpty()) {
	//	UE_LOG(LogTemp, Warning, TEXT("PlayerName is empty or null"));
	//}
	//else {
	//	UE_LOG(LogTemp, Display, TEXT("PlayerN: %s"), *PlayerName);
	//}
	//FString PlayerN = FString(playername.c_str());
	//OtherPlayerID->SetText(FText::FromString(PlayerN));

	UpdateHPBar(hp);
}



void UOtherPlayerUI::UpdateHPBar(float hp)
{
	if (iMaxHealthFlag == 0) {
		m_fMaxHealth = hp;
		++iMaxHealthFlag;
	}

	if (hp <= 0 && !m_bPlayerDeadImg) {
		m_bPlayerDeadImg = true;
		UpdateDeadUI();
	}
	HPBar->UpdateHealthBar(hp, m_fMaxHealth);
}

void UOtherPlayerUI::UpdateDeadUI()
{
	if (m_iCharacterType == 1) {
		UTexture2D* LoadedTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/CharacterAsset/Girl/girlD.girlD"));
		if (LoadedTexture) {

			FSlateBrush Brush;
			Brush.SetResourceObject(LoadedTexture);   // 텍스처를 브러시에 설정
			Brush.ImageSize = FVector2D(64.0f, 32.0f); // 이미지 크기 설정 (가로 128, 세로 128)
			OtherPlayerImg->SetBrush(Brush);      // UImage에 브러시 설정

		}
	}
	else if (m_iCharacterType == 2) {
		UTexture2D* LoadedTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/CharacterAsset/Employee/empD.empD"));
		if (LoadedTexture) {

			FSlateBrush Brush;
			Brush.SetResourceObject(LoadedTexture);   // 텍스처를 브러시에 설정
			Brush.ImageSize = FVector2D(64.0f, 32.0f); // 이미지 크기 설정 (가로 128, 세로 128)
			OtherPlayerImg->SetBrush(Brush);      // UImage에 브러시 설정

		}
	}
	else if (m_iCharacterType == 3) {
		UTexture2D* LoadedTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/CharacterAsset/Idol/idolD.idolD"));
		if (LoadedTexture) {

			FSlateBrush Brush;
			Brush.SetResourceObject(LoadedTexture);   // 텍스처를 브러시에 설정
			Brush.ImageSize = FVector2D(64.0f, 32.0f); // 이미지 크기 설정 (가로 128, 세로 128)
			OtherPlayerImg->SetBrush(Brush);      // UImage에 브러시 설정

		}
	}
	else if (m_iCharacterType == 4) {
		UTexture2D* LoadedTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/CharacterAsset/FireFighter/fireD.fireD"));
		if (LoadedTexture) {

			FSlateBrush Brush;
			Brush.SetResourceObject(LoadedTexture);   // 텍스처를 브러시에 설정
			Brush.ImageSize = FVector2D(64.0f, 32.0f); // 이미지 크기 설정 (가로 128, 세로 128)
			OtherPlayerImg->SetBrush(Brush);      // UImage에 브러시 설정

		}
	}
}

void UOtherPlayerUI::RefreshSlot(int slotindex)
{
	Slots[slotindex]->Refresh();
}
