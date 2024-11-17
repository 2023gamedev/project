// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "ProGamemode/ProGameInstance.h"
#include "WaitingRoomUI.generated.h"

/**
 * 
 */

DECLARE_DELEGATE(FMoveChoiceCharacterUI);

UCLASS()
class PROJECT_API UWaitingRoomUI : public UUserWidget
{
	GENERATED_BODY()

public:

	void Init();
	void SendChat(const FString& FormattedMessage);
	void AddPlayerToList(const FString& PlayerName);
	void AddChatMessage(const FString& Message);

	UProGameInstance* GameInstance;
	FMoveChoiceCharacterUI MoveChoiceCharacterUI;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* ReadyButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* BackButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* SendButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* PlayerList;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UScrollBox* ChattingList;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UEditableTextBox* ChatText;


private:

	UFUNCTION(BlueprintCallable)
	void OnReadyButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnBackButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnSendButtonClicked();
	
};
