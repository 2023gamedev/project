// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/Slot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "ProUI/DragOnSlot.h"
#include "ProGamemode/OneGameModeBase.h"
#include "ProItem/NormalWeaponActor.h"

void USlot::Init()
{
	Text->SetVisibility(ESlateVisibility::Hidden);
	ProgressBar->SetVisibility(ESlateVisibility::Hidden);
	Border->SetVisibility(ESlateVisibility::Hidden);

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
		if (Character->Inventory.IsValidIndex(SlotIndex) == false)	// 크래시 방지
			return;

		FItemDataStructure& data = Character->Inventory[SlotIndex];

		if (data.Texture != nullptr) {
			SetTexture(data.Texture);
		}

		ItemCount = data.Count;
		WeaponDurability = data.Durability;
		WeaponDurability_Max = data.Durability_Max;

		break;
	}
	case ESlotType::SLOT_QUICK:
	{
		if (Character->QuickSlot.IsValidIndex(SlotIndex) == false)	// 크래시 방지
			return;

		FItemDataStructure& dataquick = Character->QuickSlot[SlotIndex];

		if (dataquick.Texture != nullptr) {
			SetTexture(dataquick.Texture);
		}

		ItemCount = dataquick.Count;
		WeaponDurability = dataquick.Durability;
		WeaponDurability_Max = dataquick.Durability_Max;

		break;
	}
	case ESlotType::SLOT_PICK_UP:
	{
		if (Character->PickUpSlot.IsValidIndex(0) == false)	// 크래시 방지
			return;

		FItemDataStructure& datapick = Character->PickUpSlot[0];

		if (datapick.Texture != nullptr) {
			SetTexture(datapick.Texture);
		}

		ItemCount = datapick.Count;
		WeaponDurability = datapick.Durability;
		WeaponDurability_Max = datapick.Durability_Max;

		break;
	}

	}

	if (ItemCount <= 1) {
		Text->SetVisibility(ESlateVisibility::Hidden);
	}
	else {
		Text->SetVisibility(ESlateVisibility::Visible);
		Text->SetText(FText::FromString(FString::FromInt(ItemCount)));
	}

	if (WeaponDurability <= 0) {
		ProgressBar->SetVisibility(ESlateVisibility::Hidden);
	}
	else {
		ProgressBar->SetVisibility(ESlateVisibility::Visible);
		ProgressBar->SetPercent(GetWeaponDurabilityPercent());
	}


	if (Character->QuickSlot.IsValidIndex(4) == false) {	// 최초 업데이트 시에 크래시 방지 (퀵 슬롯 배열 초기화 전에 접근 방지)
		return;
	}

	if (Type == ESlotType::SLOT_ITEM) {
		// 퀵 슬롯에서 하나라도 해당 아이템이랑 연결되어(장착되어)있는 상태라면 
		if (Character->QuickSlot[0].SlotReference == SlotIndex || Character->QuickSlot[1].SlotReference == SlotIndex || Character->QuickSlot[2].SlotReference == SlotIndex
			|| Character->QuickSlot[3].SlotReference == SlotIndex || Character->QuickSlot[4].SlotReference == SlotIndex)	//=> 무지성한게 해당 슬롯은 무기인지 회복템인지 뭔지 모름
			Border->SetVisibility(ESlateVisibility::Visible);		// 퀵슬롯에 장착한 아이템 인벤토리 슬롯 테두리 표시
		else
			Border->SetVisibility(ESlateVisibility::Hidden);	// 아니라면 테두리 표시 해제
	}
	else if (Type == ESlotType::SLOT_QUICK) {
		if (ItemCount >= 1 || WeaponDurability > 0)
			Border->SetVisibility(ESlateVisibility::Visible);	// 퀵 슬롯에 아이템 장착 중임을 퀵 슬롯 테두리 표시를 통해 알림
		else
			Border->SetVisibility(ESlateVisibility::Hidden);	// 아니라면 테두리 표시 해제
	}
	else if (Type == ESlotType::SLOT_PICK_UP)
		Border->SetVisibility(ESlateVisibility::Hidden);		// 픽업 표시 슬롯은 아이템 테두리 표시 X

}

void USlot::RefreshOPU(int otherplayeruiindex)
{
	if (otherplayeruiindex == 1) {
		FItemDataStructure& dataopu = Character->OtherPlayerInven[SlotIndex];
		if (dataopu.Texture != nullptr) {
			SetTexture(dataopu.Texture);
		}

		ItemCount = dataopu.Count;
		//WeaponDurability = dataopu.Durability;
		//WeaponDurability_Max = dataopu.Durability_Max;

	}
	else if (otherplayeruiindex == 2) {
		FItemDataStructure& dataopu = Character->OtherPlayer2Inven[SlotIndex];
		if (dataopu.Texture != nullptr) {
			SetTexture(dataopu.Texture);
		}

		ItemCount = dataopu.Count;
		//WeaponDurability = dataopu.Durability;
		//WeaponDurability_Max = dataopu.Durability_Max;

	}
	else if (otherplayeruiindex == 3) {
		FItemDataStructure& dataopu = Character->OtherPlayer3Inven[SlotIndex];
		if (dataopu.Texture != nullptr) {
			SetTexture(dataopu.Texture);
		}

		ItemCount = dataopu.Count;
		//WeaponDurability = dataopu.Durability;
		//WeaponDurability_Max = dataopu.Durability_Max;

	}

	if (ItemCount <= 1) {
		Text->SetVisibility(ESlateVisibility::Hidden);
	}
	else {
		Text->SetVisibility(ESlateVisibility::Visible);
		Text->SetText(FText::FromString(FString::FromInt(ItemCount)));
	}

	//if (WeaponDurability <= 0) {
		ProgressBar->SetVisibility(ESlateVisibility::Hidden);	// 무기 내구도는 그냥 여기서 표시할 일이 없음 -> 다른 플레이어 아이템 표시는 키만 표시하니까
	//}
	//else {
	//	ProgressBar->SetVisibility(ESlateVisibility::Visible);
	//	ProgressBar->SetPercent(GetWeaponDurabilityPercent());
	//}

	Border->SetVisibility(ESlateVisibility::Hidden);	// 테두리 표시도 마찬가지로 표시할 필요 X

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

//void USlot::SpawnOnGround(int slotindex)	// 이건 왜 있는 거야...
//{
//	if (Character) {
//		if (Character->GetPlayerId() == 99) {
//			Character->SpawnOnGround(slotindex);
//		}
//	}
//}



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

			// 0번: 출혈회복, 1번: 상처회복, 2번: 투척무기, 3번: 키, 4번: 노말무기
		case EItemClass::BLEEDINGHEALINGITEM:
			if (Character->QuickSlot[0].Type == EItemType::ITEM_QUICK_NONE) {	// 퀵슬롯이 비어 있던 상태
				Character->QuickSlot[0].Type = EItemType::ITEM_QUICK_EQUIPMENT;
				Character->QuickSlot[0].Name = Character->Inventory[SlotIndex].Name;
				Character->QuickSlot[0].ItemClassType = Character->Inventory[SlotIndex].ItemClassType;
				Character->QuickSlot[0].Texture = Character->Inventory[SlotIndex].Texture;
				Character->QuickSlot[0].Count = Character->Inventory[SlotIndex].Count;
				Character->QuickSlot[0].SlotReference = SlotIndex;

				Character->Inventory[SlotIndex].Type = EItemType::ITEM_EQUIPMENT;
				
				Character->SpawnBleedingHealingItem();
			}
			else if (Character->QuickSlot[0].Type == EItemType::ITEM_QUICK_EQUIPMENT) {	// 퀵슬롯에 이미 장착되어 있던 상태	

				// 이전 착용중이던 아이템 인벤토리 상에서 상태 변경해주기
				int prevItemIvenIndx = Character->QuickSlot[0].SlotReference;
				if (Character->Inventory.IsValidIndex(prevItemIvenIndx)) {    // 접근 가능한 인덱스인지 먼저 확인
					Character->Inventory[prevItemIvenIndx].Type = EItemType::ITEM_USEABLE;	 // 퀵슬롯 장착 중(ITEM_EQUIPMENT)->장착하고 있지 않은 상태(ITEM_USEABLE)
				}

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
			if (Character->QuickSlot[1].Type == EItemType::ITEM_QUICK_NONE) {	// 퀵슬롯이 비어 있던 상태
				Character->QuickSlot[1].Type = EItemType::ITEM_QUICK_EQUIPMENT;
				Character->QuickSlot[1].Name = Character->Inventory[SlotIndex].Name;
				Character->QuickSlot[1].ItemClassType = Character->Inventory[SlotIndex].ItemClassType;
				Character->QuickSlot[1].Texture = Character->Inventory[SlotIndex].Texture;
				Character->QuickSlot[1].Count = Character->Inventory[SlotIndex].Count;
				Character->QuickSlot[1].SlotReference = SlotIndex;

				Character->Inventory[SlotIndex].Type = EItemType::ITEM_EQUIPMENT;

				Character->SpawnHealingItem();
			}
			else if (Character->QuickSlot[1].Type == EItemType::ITEM_QUICK_EQUIPMENT) {	// 퀵슬롯에 이미 장착되어 있던 상태

				// 이전 착용중이던 아이템 인벤토리 상에서 상태 변경해주기
				int prevItemIvenIndx = Character->QuickSlot[1].SlotReference;
				if (Character->Inventory.IsValidIndex(prevItemIvenIndx)) {    // 접근 가능한 인덱스인지 먼저 확인
					Character->Inventory[prevItemIvenIndx].Type = EItemType::ITEM_USEABLE;	 // 퀵슬롯 장착 중(ITEM_EQUIPMENT)->장착하고 있지 않은 상태(ITEM_USEABLE)
				}

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
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("HEALINGITEM!"));
			break;

		case EItemClass::THROWINGWEAPON:
			if (Character->QuickSlot[2].Type == EItemType::ITEM_QUICK_NONE) {	// 퀵슬롯이 비어 있던 상태
				Character->QuickSlot[2].Type = EItemType::ITEM_QUICK_EQUIPMENT;
				Character->QuickSlot[2].Name = Character->Inventory[SlotIndex].Name;
				Character->QuickSlot[2].ItemClassType = Character->Inventory[SlotIndex].ItemClassType;
				Character->QuickSlot[2].Texture = Character->Inventory[SlotIndex].Texture;
				Character->QuickSlot[2].Count = Character->Inventory[SlotIndex].Count;
				Character->QuickSlot[2].SlotReference = SlotIndex;

				Character->Inventory[SlotIndex].Type = EItemType::ITEM_EQUIPMENT;

				Character->SpawnThrowWeapon();
			}
			else if (Character->QuickSlot[2].Type == EItemType::ITEM_QUICK_EQUIPMENT) {	// 퀵슬롯에 이미 장착되어 있던 상태

				// 이전 착용중이던 아이템 인벤토리 상에서 상태 변경해주기
				int prevItemIvenIndx = Character->QuickSlot[2].SlotReference;
				if (Character->Inventory.IsValidIndex(prevItemIvenIndx)) {    // 접근 가능한 인덱스인지 먼저 확인
					Character->Inventory[prevItemIvenIndx].Type = EItemType::ITEM_USEABLE;	 // 퀵슬롯 장착 중(ITEM_EQUIPMENT)->장착하고 있지 않은 상태(ITEM_USEABLE)
				}

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
			if (Character->QuickSlot[3].Type == EItemType::ITEM_QUICK_NONE) {	// 퀵슬롯이 비어 있던 상태
				Character->QuickSlot[3].Type = EItemType::ITEM_QUICK_EQUIPMENT;
				Character->QuickSlot[3].Name = Character->Inventory[SlotIndex].Name;
				Character->QuickSlot[3].ItemClassType = Character->Inventory[SlotIndex].ItemClassType;
				Character->QuickSlot[3].Texture = Character->Inventory[SlotIndex].Texture;
				Character->QuickSlot[3].Count = Character->Inventory[SlotIndex].Count;
				Character->QuickSlot[3].SlotReference = SlotIndex;

				Character->Inventory[SlotIndex].Type = EItemType::ITEM_EQUIPMENT;

				Character->SpawnKeyItem();
			}
			else if (Character->QuickSlot[3].Type == EItemType::ITEM_QUICK_EQUIPMENT) {	// 퀵슬롯에 이미 장착되어 있던 상태

				// 이전 착용중이던 아이템 인벤토리 상에서 상태 변경해주기
				int prevItemIvenIndx = Character->QuickSlot[3].SlotReference;
				if (Character->Inventory.IsValidIndex(prevItemIvenIndx)) {    // 접근 가능한 인덱스인지 먼저 확인
					Character->Inventory[prevItemIvenIndx].Type = EItemType::ITEM_USEABLE;	 // 퀵슬롯 장착 중(ITEM_EQUIPMENT)->장착하고 있지 않은 상태(ITEM_USEABLE)
				}

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
			if (Character->QuickSlot[4].Type == EItemType::ITEM_QUICK_NONE) {	// 퀵슬롯이 비어 있던 상태
				Character->QuickSlot[4].Type = EItemType::ITEM_QUICK_EQUIPMENT;
				Character->QuickSlot[4].Name = Character->Inventory[SlotIndex].Name;
				Character->QuickSlot[4].ItemClassType = Character->Inventory[SlotIndex].ItemClassType;
				Character->QuickSlot[4].Texture = Character->Inventory[SlotIndex].Texture;
				Character->QuickSlot[4].Count = Character->Inventory[SlotIndex].Count;
				Character->QuickSlot[4].SlotReference = SlotIndex;
				Character->QuickSlot[4].Durability = Character->Inventory[SlotIndex].Durability;
				Character->QuickSlot[4].Durability_Max = Character->Inventory[SlotIndex].Durability_Max;

				Character->Inventory[SlotIndex].Type = EItemType::ITEM_EQUIPMENT;

				Character->SpawnNormalWeapon();
			}
			else if (Character->QuickSlot[4].Type == EItemType::ITEM_QUICK_EQUIPMENT) {	// 퀵슬롯에 이미 장착되어 있던 상태

				// 이전 착용중이던 아이템 인벤토리 상에서 상태 변경해주기
				int prevItemIvenIndx = Character->QuickSlot[4].SlotReference;
				if (Character->Inventory.IsValidIndex(prevItemIvenIndx)) {    // 접근 가능한 인덱스인지 먼저 확인
					Character->Inventory[prevItemIvenIndx].Type = EItemType::ITEM_USEABLE;	 // 퀵슬롯 장착 중(ITEM_EQUIPMENT)->장착하고 있지 않은 상태(ITEM_USEABLE)
				}

				Character->QuickSlot[4].Type = EItemType::ITEM_QUICK_NONE;
				Character->QuickSlot[4].Name = "nullptr";
				Character->QuickSlot[4].ItemClassType = EItemClass::NONE;
				Character->QuickSlot[4].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
				Character->QuickSlot[4].Count = 0;
				Character->QuickSlot[4].SlotReference = -1;
				Character->QuickSlot[4].Durability = 0;
				Character->QuickSlot[4].Durability_Max = 0;

				if (Character->IsBringCurrentWeapon()) {
					Character->DestroyNormalWeapon();
					Character->SetNWHandIn(false);
					Character->SetBringCurrentWeapon(false);
					//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag: NORMALWEAPON"));
				}
			}

			Character->GameUIUpdate();
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("NORMALWEAPON!"));
			break;

		case EItemClass::BAGITEM:
			if (Character->GetCarryBagName() == "BigBag") {	// 이미 큰가방을 먹어 인벤토리가 다 열린 상태
				break;
			}
			else if (Character->GetCarryBagName() == "Bag" || Character->GetCarryBagName() == "SuitCase") {
				if (Character->Inventory[SlotIndex].Name == "BigBag") {	// 이전에 들고 있던 가방이 중간크기 가방이였고 이제는 큰 가방을 든 상태 => 중간 크기 가방은 다시 가방안으로 돌아와서 다른 사람에게 줄 수 있음

					if (Character->GetCarryBagName() == "Bag")
						Character->Inventory[SlotIndex].Name = "Bag";
					else if (Character->GetCarryBagName() == "SuitCase")
						Character->Inventory[SlotIndex].Name = "SuitCase";
					Character->Inventory[SlotIndex].ItemClassType = EItemClass::BAGITEM;
					if (Character->GetCarryBagName() == "Bag")
						Character->Inventory[SlotIndex].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenBag.InvenBag"));
					else if (Character->GetCarryBagName() == "SuitCase")
						Character->Inventory[SlotIndex].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenSuitCase.InvenSuitCase"));
					Character->Inventory[SlotIndex].Count = 1;
					Character->Inventory[SlotIndex].Type = EItemType::ITEM_USEABLE;

					Character->SetCarryBagName("BigBag");
					Character->SetInvenSize(20);

					for (int i = 10; i < 20; ++i) {
						Character->Inventory[i].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
					}
				}
			}
			else {	// 이전에 가방을 아예 안 들고 있었던 상태
				if (Character->Inventory[SlotIndex].Name == "BigBag") {

					Character->Inventory[SlotIndex].Name = "nullptr";
					Character->Inventory[SlotIndex].ItemClassType = EItemClass::NONE;
					Character->Inventory[SlotIndex].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
					Character->Inventory[SlotIndex].Count = 0;
					Character->Inventory[SlotIndex].Type = EItemType::ITEM_NONE;

					Character->SetCarryBagName("BigBag");
					Character->SetInvenSize(20);

					for (int i = 5; i < 20; ++i) {
						Character->Inventory[i].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
					}
				}
				else if (Character->Inventory[SlotIndex].Name == "Bag" || Character->Inventory[SlotIndex].Name == "SuitCase") {

					auto prevBagName = Character->Inventory[SlotIndex].Name;

					Character->Inventory[SlotIndex].Name = "nullptr";
					Character->Inventory[SlotIndex].ItemClassType = EItemClass::NONE;
					Character->Inventory[SlotIndex].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
					Character->Inventory[SlotIndex].Count = 0;
					Character->Inventory[SlotIndex].Type = EItemType::ITEM_NONE;

					if (prevBagName == "Bag") 
						Character->SetCarryBagName("Bag");
					else if (prevBagName == "SuitCase") 
						Character->SetCarryBagName("SuitCase");
					Character->SetInvenSize(10);

					for (int i = 5; i < 10; ++i) {
						Character->Inventory[i].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
					}
				}
			}

			Character->GameUIUpdate();
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("BAG ITEM!"));
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


