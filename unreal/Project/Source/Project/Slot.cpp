// Fill out your copyright notice in the Description page of Project Settings.


#include "Slot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "DragOnSlot.h"

void USlot::Init()
{
	Refresh();
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
	}
}

// Shift + f1 마우스 커서 이런식으로 동작해서 드래그되는 과정이 안보이는지 모르겠다. 그런것 같긴 함
void USlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (OutOperation == nullptr) {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag: Drag Start"));

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
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag: Drag Again"));
	}
}

bool USlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	UDragOnSlot* oper = Cast<UDragOnSlot>(InOperation);

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag: Drag End"));

	if (oper != nullptr) {
		Character->DraggingSwap(oper->SlotIndex, oper->Type, this->SlotIndex, this->Type);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag: Drag Success"));
		return true;
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag: Drag Fail"));
		return false;
	}
}

FReply USlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply eventreply;
	eventreply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton) == true) { // 퀵슬롯에 넣어주거나 해제하는 역할 할 예정
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag: Right Button Down"));

		// 수정 무지막지하게 필요 일단 화면에 뜬다만 보여주는 부분
		switch (Character->Inventory[SlotIndex].ItemClassType) {

		case EItemClass::BLEEDINGHEALINGITEM:
			Character->QuickSlot[0].Type = Character->Inventory[SlotIndex].Type;
			Character->QuickSlot[0].Name = Character->Inventory[SlotIndex].Name;
			Character->QuickSlot[0].ItemClassType = Character->Inventory[SlotIndex].ItemClassType;
			Character->QuickSlot[0].Texture = Character->Inventory[SlotIndex].Texture;
			Character->QuickSlot[0].Count = Character->Inventory[SlotIndex].Count;

			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("BLEEDINGHEALINGITEM!"));
			break;
		case EItemClass::HEALINGITEM:
			Character->QuickSlot[1].Type = Character->Inventory[SlotIndex].Type;
			Character->QuickSlot[1].Name = Character->Inventory[SlotIndex].Name;
			Character->QuickSlot[1].ItemClassType = Character->Inventory[SlotIndex].ItemClassType;
			Character->QuickSlot[1].Texture = Character->Inventory[SlotIndex].Texture;
			Character->QuickSlot[1].Count = Character->Inventory[SlotIndex].Count;

			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("HEALINGITEM!"));
			break;
		case EItemClass::THROWINGWEAPON:
			Character->QuickSlot[2].Type = Character->Inventory[SlotIndex].Type;
			Character->QuickSlot[2].Name = Character->Inventory[SlotIndex].Name;
			Character->QuickSlot[2].ItemClassType = Character->Inventory[SlotIndex].ItemClassType;
			Character->QuickSlot[2].Texture = Character->Inventory[SlotIndex].Texture;
			Character->QuickSlot[2].Count = Character->Inventory[SlotIndex].Count;

			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("THROWINGWEAPON!"));
			break;
		case EItemClass::KEYITEM:
			Character->QuickSlot[3].Type = Character->Inventory[SlotIndex].Type;
			Character->QuickSlot[3].Name = Character->Inventory[SlotIndex].Name;
			Character->QuickSlot[3].ItemClassType = Character->Inventory[SlotIndex].ItemClassType;
			Character->QuickSlot[3].Texture = Character->Inventory[SlotIndex].Texture;
			Character->QuickSlot[3].Count = Character->Inventory[SlotIndex].Count;

			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("KEYITEM!"));
			break;
		case EItemClass::NORMALWEAPON:
			if (Character->QuickSlot[4].Type == EItemType::ITEM_QUICK_NONE) {
				Character->QuickSlot[4].Type = EItemType::ITEM_QUICK_EQUIPMENT;
				Character->QuickSlot[4].Name = Character->Inventory[SlotIndex].Name;
				Character->QuickSlot[4].ItemClassType = Character->Inventory[SlotIndex].ItemClassType;
				Character->QuickSlot[4].Texture = Character->Inventory[SlotIndex].Texture;
				Character->QuickSlot[4].Count = Character->Inventory[SlotIndex].Count;
			}
			else if (Character->QuickSlot[4].Type == EItemType::ITEM_QUICK_EQUIPMENT) {
				Character->QuickSlot[4].Type = EItemType::ITEM_QUICK_NONE;
				Character->QuickSlot[4].Name = "nullptr";
				Character->QuickSlot[4].ItemClassType = EItemClass::NONE;
				Character->QuickSlot[4].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
				Character->QuickSlot[4].Count = 0;
			}

			
			Character->GameUIUpdate();

			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Normal Weapon!"));
			break;

		}
	}
	else if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) == true) {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag: Left Button Down"));

		switch (Type) {
		case ESlotType::SLOT_NONE: case ESlotType::SLOT_QUICK: break;

		case ESlotType::SLOT_ITEM: case ESlotType::SLOT_QUICK_ITEM:

			if (Character->Inventory[SlotIndex].Type != EItemType::ITEM_NONE) {
				eventreply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
				break;
			}
		}
	}


	return eventreply.NativeReply;
}
