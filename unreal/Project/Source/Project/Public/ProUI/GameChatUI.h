// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProUI/BaseUI.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "ProGamemode/ProGameInstance.h"
#include "ProUI/SEditableTextBox_IgnoreEnter.h"
#include "GameChatUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UGameChatUI : public UBaseUI
{
	GENERATED_BODY()
	
public:

	UProGameInstance* GameInstance;

	void Init();

	bool IsChatVisible() const;

	void AddChatMessage(const FString& Message);

	void SendChat(const FString& FormattedMessage);

	void ToggleChatVisibility(bool bChatVisible);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UScrollBox* GameChatBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UEditableTextBox* ChatText;

	bool bIsChatVisible = false;

private:

	UFUNCTION(BlueprintCallable)
	void OnSendButtonEntered(const FText& Text, ETextCommit::Type CommitMethod);

	//TSharedPtr<SEditableTextBox_IgnoreEnter> ChatText_IE;
};
