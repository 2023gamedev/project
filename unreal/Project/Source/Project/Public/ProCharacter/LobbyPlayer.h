// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "ProUI/ChoiceCharacterUI.h"
#include "ProGamemode/LobbyGameMode.h"
#include "Components/CapsuleComponent.h"
#include "LobbyPlayer.generated.h"

class UStartGameUI;
class USelect_RoomUI;
class UChoiceCharacterUI;
class ULoginUI;
class UWaitingRoomUI;

DECLARE_DELEGATE(FChoicedGirlCharcaterD);
DECLARE_DELEGATE(FChoicedEmployeeCharcaterD);
DECLARE_DELEGATE(FChoicedIdolCharcaterD);
DECLARE_DELEGATE(FChoicedFireFighterCharcaterD);


// Lobby에서 UI를 고르는 Player 클래스
UCLASS()
class PROJECT_API ALobbyPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ALobbyPlayer();

	TMap<uint32, TMap<uint32, FString>> RoomLobbyPlayers;

	const TMap<uint32, TMap<uint32, FString>>& GetLobbyPlayers() const { return RoomLobbyPlayers; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UChoiceCharacterUI* choicecharacterui;
	UProGameInstance* GameInstance;

	FChoicedGirlCharcaterD ChoicedGirlCharcaterD;
	FChoicedEmployeeCharcaterD ChoicedEmployeeCharcaterD;
	FChoicedIdolCharcaterD ChoicedIdolCharcaterD;
	FChoicedFireFighterCharcaterD ChoicedFireFighterCharcaterD;
	
	void ChoicedGirlCharacter();
	void ChoicedEmployeeCharacter();
	void ChoicedIdolCharacter();
	void ChoicedFireFighterCharacter();
	
	void MoveChoiceCharacterUI();
	void MoveStartGameUI();
	void MoveWaitingRoomUI();
	void OnMouseLeftClick();

	CharacterSelect recvSelect;
	Chatting recvChat;
	JoinPlayer recvJplayer;
	LeavePlayer recvLplayer;
	WaitingReady recvWready;
	SelectReady recvSready;

	bool waitingready = false;

	// 카메라
	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	UCapsuleComponent* Capsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<USelect_RoomUI> StartGameUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	USelect_RoomUI* StartGameUIWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UChoiceCharacterUI> ChoiceCharacterUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UChoiceCharacterUI* ChoiceCharacterUIWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<ULoginUI> LoginUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	ULoginUI* LoginUIWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UWaitingRoomUI> WaitingRoomUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UWaitingRoomUI* WaitingRoomUIWidget;


};
