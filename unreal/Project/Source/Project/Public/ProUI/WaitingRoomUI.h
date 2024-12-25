// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "ProGamemode/ProGameInstance.h"
#include "WaitingRoomUI.generated.h"

/**
 * 
 */

DECLARE_DELEGATE(FMoveChoiceCharacterUI);
DECLARE_DELEGATE(FMoveStartGameUI);

UCLASS()
class PROJECT_API UWaitingRoomUI : public UUserWidget
{
	GENERATED_BODY()

public:

	void Init();
	void SendChat(const FString& FormattedMessage);
	int32 FindEmptySlot();
	void UpdatePlayerReadyState(uint32 playerid, bool Ready);
	void AddPlayerToList(const uint32 roomid, const FString& PlayerName);
	void RemovePlayerFromList(const FString& PlayerName);
	void AddChatMessage(const FString& Message);
	void AllReady();
	void PlayerReady(uint32 playerid, bool ready);

	UProGameInstance* GameInstance;
	FMoveChoiceCharacterUI MoveChoiceCharacterUI;
	FMoveStartGameUI MoveStartGameUI;

	TMap <uint32, FString> Waiting_LobbyPlayers;

	TArray<UHorizontalBox*> PlayerSlots;
	TArray<bool> SlotOccupied;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* ReadyButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* BackButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* SendButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* PlayerList;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* PlayerSlot1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* PlayerSlot2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* PlayerSlot3;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UHorizontalBox* PlayerSlot4;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UScrollBox* ChattingList;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UEditableTextBox* ChatText;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Ready_TextBlock;


private:

	UFUNCTION(BlueprintCallable)
	void OnReadyButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnBackButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnSendButtonClicked();

public:

	bool bIsReady = false;
	
};
