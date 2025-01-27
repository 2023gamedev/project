// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProUI/BaseUI.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "GameChatUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UGameChatUI : public UBaseUI
{
	GENERATED_BODY()
	
public:

	void AddChatMessageToLog(const FString& Message);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UScrollBox* GameChatBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UEditableTextBox* ChatText;

};
