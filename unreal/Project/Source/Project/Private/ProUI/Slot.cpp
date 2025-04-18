// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/Slot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "ProUI/DragOnSlot.h"
#include "ProGamemode/OneGameModeBase.h"

void USlot::Init()
{
	Refresh();
}

void USlot::InitOPU(int otherplayeruiindex)
{
	RefreshOPU(otherplayeruiindex);
}


void USlot::Update()
{
	Refresh();
}

void USlot::SetType(ESlotType type)
{
	Type = type;
}



void USlot::SetTexture(UTexture2D* tex)
{
	if (tex == nullptr) return;

	Img->SetBrushFromTexture(tex);
}

void USlot::Refresh()
{
	switch (Type) {
	case ESlotType::SLOT_ITEM:
	{
		FItemDataStructure& data = Character->Inventory[SlotIndex];

		if (data.Texture != nullptr) {
			SetTexture(data.Texture);
		}

		ItemCount = data.Count;

		if (ItemCount <= 1) {
			Text->SetVisibility(ESlateVisibility::Hidden);
		}
		else {
			Text->SetVisibility(ESlateVisibility::Visible);
			Text->SetText(FText::FromString(FString::FromInt(ItemCount)));
		}
		break;
	}
	case ESlotType::SLOT_QUICK:
	{

		FItemDataStructure& dataquick = Character->QuickSlot[SlotIndex];

		if (dataquick.Texture != nullptr) {
			SetTexture(dataquick.Texture);
		}

		ItemCount = dataquick.Count;

		if (ItemCount <= 1) {
			Text->SetVisibility(ESlateVisibility::Hidden);
		}
		else {
			Text->SetVisibility(ESlateVisibility::Visible);
			Text->SetText(FText::FromString(FString::FromInt(ItemCount)));
		}
		break;
	}
	case ESlotType::SLOT_PICK_UP:
	{

		FItemDataStructure& dataquick = Character->PickUpSlot[0];

		if (dataquick.Texture != nullptr) {
			SetTexture(dataquick.Texture);
		}

		ItemCount = dataquick.Count;

		if (ItemCount <= 1) {
			Text->SetVisibility(ESlateVisibility::Hidden);
		}
		else {
			Text->SetVisibility(ESlateVisibility::Visible);
			Text->SetText(FText::FromString(FString::FromInt(ItemCount)));
		}
		break;
	}

	}
}

void USlot::RefreshOPU(int otherplayeruiindex)
{
	if (otherplayeruiindex == 1) {
		FItemDataStructure& dataquick = Character->OtherPlayerInven[SlotIndex];
		if (dataquick.Texture != nullptr) {
			SetTexture(dataquick.Texture);
		}

		ItemCount = dataquick.Count;


	}
	else if (otherplayeruiindex == 2) {
		FItemDataStructure& dataquick = Character->OtherPlayer2Inven[SlotIndex];
		if (dataquick.Texture != nullptr) {
			SetTexture(dataquick.Texture);
		}

		ItemCount = dataquick.Count;

	}
	else if (otherplayeruiindex == 3) {
		FItemDataStructure& dataquick = Character->OtherPlayer3Inven[SlotIndex];
		if (dataquick.Texture != nullptr) {
			SetTexture(dataquick.Texture);
		}

		ItemCount = dataquick.Count;

	}

	if (ItemCount <= 1) {
		Text->SetVisibility(ESlateVisibility::Hidden);
	}
	else {
		Text->SetVisibility(ESlateVisibility::Visible);
		Text->SetText(FText::FromString(FString::FromInt(ItemCount)));
	}
}

// Shift + f1 마우스 커서 이런식으로 동작해서 드래그되는 과정이 안보이는지 모르겠다. 그런것 같긴 함
void USlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (OutOperation == nullptr) {
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag: Drag Start"));
		//bIsDragging = true;
		UDragOnSlot* oper = NewObject<UDragOnSlot>();
		OutOperation = oper;
		oper->SlotIndex = this->SlotIndex;
		oper->Type = this->Type;
		
		if (DragVisualClass != nullptr) {
			USlot* visual = CreateWidget<USlot>(Cast<APlayerController>(Character->Controller), DragVisualClass);
			visual->Type = this->Type;
			visual->Character = this->Character;
			visual->SlotIndex = this->SlotIndex;
			visual->Refresh();
			oper->DefaultDragVisual = visual;
		}
	}
	else {
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag: Drag Again"));
	}
}
//
//FReply USlot::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
//{
//	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
//		// 드래그 상태 확인
//		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag: NativeOnMouseButtonUp"));
//		if (bIsDragging) {
//			if (Character) {
//				if (Character->GetPlayerId() == 99) {
//					Character->SpawnOnGround(SlotIndex);
//				}
//			}
//			bIsDragging = false; // 드래그 상태 종료
//			return FReply::Handled();
//		}
//	}
//	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
//}

bool USlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	UDragOnSlot* oper = Cast<UDragOnSlot>(InOperation);

	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag: Drag End"));

	if (oper != nullptr) {
		// DragSwap을 사용하지 않기에 그냥 주석처리 --- 안하면 검은칸이 회색칸으로 바뀔수가 있음
		//Character->DraggingSwap(oper->SlotIndex, oper->Type, this->SlotIndex, this->Type);
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag: Drag Success"));

		return true;
	}
	else {
		// fall 드랍하자
		if (Character) {
			if (Character->GetPlayerId() == 99) {
				Character->SpawnOnGround(SlotIndex);
			}
		}
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag: Drag Fail"));
		return false;
	}

}

void USlot::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);

	
	// 드래그 중인지 확인하고 처리
	if (Character && this->Type != ESlotType::SLOT_QUICK) { // InOperation이 nullptr인 경우에만 처리
		if (Character->GetPlayerId() == 99) {
			Character->SpawnOnGround(SlotIndex);
		}
	}
}

void USlot::SpawnOnGround(int slotindex)
{
	if (Character) {
		if (Character->GetPlayerId() == 99) {
			Character->SpawnOnGround(slotindex);
		}
	}
}



FReply USlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply eventreply;
	eventreply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	// QuickSlot을 클릭하면 아무 동작도 하지 않도록 처리
	if (Type == ESlotType::SLOT_QUICK) {
		return FReply::Handled();
	}

	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) == true) { // 퀵슬롯에 넣어주거나 해제하는 역할 할 예정
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag: Right Button Down"));

		// 빈칸에 마우스 우클릭 시 팅기는 부분 방지
		if (Character->Inventory[SlotIndex].Type == EItemType::ITEM_NONE) {
			return eventreply.NativeReply;
		}

		switch (Character->Inventory[SlotIndex].ItemClassType) {
		
			// 0번: 출혈회복, 1번: 상처회복 2번: 투척무기 3번: 키 4번: 노말무기
		case EItemClass::BLEEDINGHEALINGITEM:
			if (Character->QuickSlot[0].Type == EItemType::ITEM_QUICK_NONE) {
				Character->QuickSlot[0].Type = EItemType::ITEM_QUICK_EQUIPMENT;
				Character->QuickSlot[0].Name = Character->Inventory[SlotIndex].Name;
				Character->QuickSlot[0].ItemClassType = Character->Inventory[SlotIndex].ItemClassType;
				Character->QuickSlot[0].Texture = Character->Inventory[SlotIndex].Texture;
				Character->QuickSlot[0].Count = Character->Inventory[SlotIndex].Count;
				Character->QuickSlot[0].SlotReference = SlotIndex;

				Character->Inventory[SlotIndex].Type = EItemType::ITEM_EQUIPMENT;

				Character->SpawnBleedingHealingItem();
			}
			else if (Character->QuickSlot[0].Type == EItemType::ITEM_QUICK_EQUIPMENT) {
				Character->QuickSlot[0].Type = EItemType::ITEM_QUICK_NONE;
				Character->QuickSlot[0].Name = "nullptr";
				Character->QuickSlot[0].ItemClassType = EItemClass::NONE;
				Character->QuickSlot[0].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
				Character->QuickSlot[0].Count = 0;
				Character->QuickSlot[0].SlotReference = -1;

				Character->Inventory[SlotIndex].Type = EItemType::ITEM_USEABLE;

				if (Character->IsBringCurrentBleedingHealingItem()) {
					Character->DestroyBleedingHealingItem();
					Character->SetBHHandIn(false);
					Character->SetBringCurrentBleedingHealingItem(false);
					//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag: BLEEDINGHEALINGITEM"));
				}
			}
			Character->GameUIUpdate();
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("BLEEDINGHEALINGITEM!"));
			break;
		case EItemClass::HEALINGITEM:
			if (Character->QuickSlot[1].Type == EItemType::ITEM_QUICK_NONE) {
				Character->QuickSlot[1].Type = EItemType::ITEM_QUICK_EQUIPMENT;
				Character->QuickSlot[1].Name = Character->Inventory[SlotIndex].Name;
				Character->QuickSlot[1].ItemClassType = Character->Inventory[SlotIndex].ItemClassType;
				Character->QuickSlot[1].Texture = Character->Inventory[SlotIndex].Texture;
				Character->QuickSlot[1].Count = Character->Inventory[SlotIndex].Count;
				Character->QuickSlot[1].SlotReference = SlotIndex;

				Character->Inventory[SlotIndex].Type = EItemType::ITEM_EQUIPMENT;

				Character->SpawnHealingItem();
			}
			else if (Character->QuickSlot[1].Type == EItemType::ITEM_QUICK_EQUIPMENT) {
				Character->QuickSlot[1].Type = EItemType::ITEM_QUICK_NONE;
				Character->QuickSlot[1].Name = "nullptr";
				Character->QuickSlot[1].ItemClassType = EItemClass::NONE;
				Character->QuickSlot[1].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
				Character->QuickSlot[1].Count = 0;
				Character->QuickSlot[1].SlotReference = -1;

				Character->Inventory[SlotIndex].Type = EItemType::ITEM_USEABLE;


				if (Character->IsBringCurrentHealingItem()) {
					Character->DestroyHealingItem();
					Character->SetHealHandIn(false);
					Character->SetBringCurrentHealingItem(false);
					//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag: HEALINGITEM"));
				}
			}

			Character->GameUIUpdate();
			////GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("HEALINGITEM!"));
			break;
		case EItemClass::THROWINGWEAPON:
			if (Character->QuickSlot[2].Type == EItemType::ITEM_QUICK_NONE) {
				Character->QuickSlot[2].Type = EItemType::ITEM_QUICK_EQUIPMENT;
				Character->QuickSlot[2].Name = Character->Inventory[SlotIndex].Name;
				Character->QuickSlot[2].ItemClassType = Character->Inventory[SlotIndex].ItemClassType;
				Character->QuickSlot[2].Texture = Character->Inventory[SlotIndex].Texture;
				Character->QuickSlot[2].Count = Character->Inventory[SlotIndex].Count;
				Character->QuickSlot[2].SlotReference = SlotIndex;

				Character->Inventory[SlotIndex].Type = EItemType::ITEM_EQUIPMENT;


				Character->SpawnThrowWeapon();
			}
			else if (Character->QuickSlot[2].Type == EItemType::ITEM_QUICK_EQUIPMENT) {
				Character->QuickSlot[2].Type = EItemType::ITEM_QUICK_NONE;
				Character->QuickSlot[2].Name = "nullptr";
				Character->QuickSlot[2].ItemClassType = EItemClass::NONE;
				Character->QuickSlot[2].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
				Character->QuickSlot[2].Count = 0;
				Character->QuickSlot[2].SlotReference = -1;


				Character->Inventory[SlotIndex].Type = EItemType::ITEM_USEABLE;
				
				if (Character->IsBringCurrentThrowWeapon()) {
					Character->DestroyThrowWeapon();
					Character->SetThrowWHandIn(false);
					Character->SetBringCurrentThrowWeapon(false);
					//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag: THROWINGWEAPON"));
				}
			}
			Character->GameUIUpdate();
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("THROWINGWEAPON!"));
			break;
		case EItemClass::KEYITEM:
			if (Character->QuickSlot[3].Type == EItemType::ITEM_QUICK_NONE) {
				Character->QuickSlot[3].Type = EItemType::ITEM_QUICK_EQUIPMENT;
				Character->QuickSlot[3].Name = Character->Inventory[SlotIndex].Name;
				Character->QuickSlot[3].ItemClassType = Character->Inventory[SlotIndex].ItemClassType;
				Character->QuickSlot[3].Texture = Character->Inventory[SlotIndex].Texture;
				Character->QuickSlot[3].Count = Character->Inventory[SlotIndex].Count;
				Character->QuickSlot[3].SlotReference = SlotIndex;

				Character->Inventory[SlotIndex].Type = EItemType::ITEM_EQUIPMENT;

				Character->SpawnKeyItem();
			}
			else if (Character->QuickSlot[3].Type == EItemType::ITEM_QUICK_EQUIPMENT) {
				Character->QuickSlot[3].Type = EItemType::ITEM_QUICK_NONE;
				Character->QuickSlot[3].Name = "nullptr";
				Character->QuickSlot[3].ItemClassType = EItemClass::NONE;
				Character->QuickSlot[3].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
				Character->QuickSlot[3].Count = 0;
				Character->QuickSlot[3].SlotReference = -1;

				Character->Inventory[SlotIndex].Type = EItemType::ITEM_USEABLE;


				if (Character->IsBringCurrentKeyItem()) {
					Character->DestroyKeyItem();
					Character->SetKeyHandIn(false);
					Character->SetBringCurrentKeyItem(false);
					//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag: KEYITEM"));
				}
			}

			Character->GameUIUpdate();
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("KEYITEM!"));
			break;
		case EItemClass::NORMALWEAPON:
			if (Character->QuickSlot[4].Type == EItemType::ITEM_QUICK_NONE) {
				Character->QuickSlot[4].Type = EItemType::ITEM_QUICK_EQUIPMENT;
				Character->QuickSlot[4].Name = Character->Inventory[SlotIndex].Name;
				Character->QuickSlot[4].ItemClassType = Character->Inventory[SlotIndex].ItemClassType;
				Character->QuickSlot[4].Texture = Character->Inventory[SlotIndex].Texture;
				Character->QuickSlot[4].Count = Character->Inventory[SlotIndex].Count;
				Character->QuickSlot[4].SlotReference = SlotIndex;

				Character->Inventory[SlotIndex].Type = EItemType::ITEM_EQUIPMENT;

				Character->SpawnNormalWeapon();
			}
			else if (Character->QuickSlot[4].Type == EItemType::ITEM_QUICK_EQUIPMENT) {
				Character->QuickSlot[4].Type = EItemType::ITEM_QUICK_NONE;
				Character->QuickSlot[4].Name = "nullptr";
				Character->QuickSlot[4].ItemClassType = EItemClass::NONE;
				Character->QuickSlot[4].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
				Character->QuickSlot[4].Count = 0;
				Character->QuickSlot[4].SlotReference = -1;

				Character->Inventory[SlotIndex].Type = EItemType::ITEM_USEABLE;


				if (Character->IsBringCurrentWeapon()) {
					Character->DestroyNormalWeapon();
					Character->SetNWHandIn(false);
					Character->SetBringCurrentWeapon(false);
					//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag: NORMALWEAPON"));
				}
			}

			Character->GameUIUpdate();
			break;
		case EItemClass::BAGITEM:
			if (Character->GetCarryBagName() == "BigBagActor") {

				break;
			}
			else if (Character->GetCarryBagName() == "BagActor") {

				if (Character->Inventory[SlotIndex].Name == "BigBagActor") {
					Character->Inventory[SlotIndex].Name = "BagActor";
					Character->Inventory[SlotIndex].ItemClassType = EItemClass::BAGITEM;
					Character->Inventory[SlotIndex].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenBag.InvenBag"));
					Character->Inventory[SlotIndex].Count = 1;

					Character->Inventory[SlotIndex].Type = EItemType::ITEM_USEABLE;

					Character->SetCarryBagName("BigBagActor");
					Character->SetInvenSize(20);

					for (int i = 10; i < 20; ++i) {
						Character->Inventory[i].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
					}

				}
			}
			else {
				if (Character->Inventory[SlotIndex].Name == "BigBagActor") {
					Character->Inventory[SlotIndex].Name = "nullptr";
					Character->Inventory[SlotIndex].ItemClassType = EItemClass::NONE;
					Character->Inventory[SlotIndex].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
					Character->Inventory[SlotIndex].Count = 0;

					Character->Inventory[SlotIndex].Type = EItemType::ITEM_NONE;

					Character->SetCarryBagName("BigBagActor");
					Character->SetInvenSize(20);

					for (int i = 5; i < 20; ++i) {
						Character->Inventory[i].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
					}
				}
				else if (Character->Inventory[SlotIndex].Name == "BagActor") {
					Character->Inventory[SlotIndex].Name = "nullptr";
					Character->Inventory[SlotIndex].ItemClassType = EItemClass::NONE;
					Character->Inventory[SlotIndex].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
					Character->Inventory[SlotIndex].Count = 0;

					Character->Inventory[SlotIndex].Type = EItemType::ITEM_NONE;

					Character->SetCarryBagName("BagActor");
					Character->SetInvenSize(10);

					for (int i = 5; i < 10; ++i) {
						Character->Inventory[i].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
					}

				}
			}

			Character->GameUIUpdate();
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Normal Weapon!"));
			break;

		}



	}
	else if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) == true) {
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag: Left Button Down"));

		switch (Type) {
		case ESlotType::SLOT_NONE: case ESlotType::SLOT_QUICK: break;

		case ESlotType::SLOT_ITEM:

			if (Character->Inventory[SlotIndex].Type != EItemType::ITEM_NONE) {
				eventreply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
				break;
			}
		}
	}


	return eventreply.NativeReply;
}


