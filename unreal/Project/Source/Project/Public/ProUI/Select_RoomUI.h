// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "ProGamemode/ProGameInstance.h"
#include "Select_RoomUI.generated.h"

/**
 *
 */

DECLARE_DELEGATE(FMoveWaitingRoomUI);

UCLASS()
class USelect_RoomUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void Init();

	void SendJoin(uint32 roomid);

	UProGameInstance* GameInstance;

	FMoveWaitingRoomUI MoveWaitingRoomUI;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* Room1Button;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* Room2Button;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* Room3Button;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* Room4Button;

private:

	UFUNCTION(BlueprintCallable)
	void OnRoom1ButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnRoom2ButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnRoom3ButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnRoom4ButtonClicked();
};
