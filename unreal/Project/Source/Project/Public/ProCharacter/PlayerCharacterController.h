// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProData/InputDataAsset.h"
#include "InputActionValue.h"
#include "ClientSocket.h"
#include <atomic>

#include "PlayerCharacterController.generated.h"

/**
 * 
 */


class UInputMappingContext;
class UInputDataAsset;

// Player 캐릭터들의 컨트롤러(LobbyPlayer 제외)
UCLASS()
class PROJECT_API APlayerCharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerCharacterController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;

public:
	void MoveForward(const FInputActionValue& Value);
	void MoveLeft(const FInputActionValue& Value);
	void MoveForwardStop();
	void MoveLeftStop();
	void Turn(const FInputActionValue& Value);
	void LookUp(const FInputActionValue& Value);
	void Run(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void GetItem(const FInputActionValue& Value);
	void LightOnOff(const FInputActionValue& Value);
	void InventoryOnOff(const FInputActionValue& Value);

	void BehaviorToItem(const FInputActionValue& Value);
	void Attack(int attack_type);	// attack_type = 1: 세로-대각 베기 / = 2: 가로 베기
	void Healing();
	void BleedHealing();
	void PlayKey();
	void Throw();

	void QuickNWeapon(const FInputActionValue& Value);
	void QuickTWeapon(const FInputActionValue& Value);
	void QuickBHItem(const FInputActionValue& Value);
	void QuickHItem(const FInputActionValue& Value);
	void QuickKeyItem(const FInputActionValue& Value);

	void ChatToggle(const FInputActionValue& Value);

	void UpSTR(const FInputActionValue& Value);
	void DownSTR(const FInputActionValue& Value);
	void UpSpeed(const FInputActionValue& Value);
	void DownSpeed(const FInputActionValue& Value);

	void BehaviorToItem_2(const FInputActionValue& Value);

	FTimerHandle InputCoolTimeHandle;
	void InputCoolTime();
	bool m_bIsInputEnabled = true;

	void DisabledControllerInput();

	//void ServerHandleAttack();	//=> 안쓰는 듯?


public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void CheckAndSendMovement(float DeltaTime);

	void Send_Attack();

	void Send_Equipment();

	void Check_run();

	//void Send_run();

	void Send_jump();

	virtual void SetupInputComponent() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputMappingContext* InputMapping;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputDataAsset* InputActions;

private:

	//캐릭터의 이전 위치 저장 변수
	FVector PreviousLocation;
	FRotator PreviousRotation;
	float PreviouHP;

	bool m_bisLeftClickAction = false;
	 
public:
	UProGameInstance* GameInstance;
	PlayerData recvPlayerData;
	PlayerAttack recvPlayerAttack;
	EquipItem recvEquipItem;
	PlayerRun recvRun;
	PlayerJump recvJump;
	ZombieData recvZombieData;
	ZombieAttack recvZombieAttack;
	ZombieHP recvZombieHP;
	//ZombiePath recvZombiePath;
	Set_Item recvSetItem;
	Set_Car recvSetCar;
	Destroy_Item recvDestroyItem;
	Get_Key recvGetkey;
	Escape_Root recvEscapeRoot;
	GameClear recvGameClear;
	Drop_Item recvDropItem;
	DetachItem recvDetachItem;
	SliceVector recvSliceVector;
	ZombieShouting recvZombieShouting;
	GameChatting recvGameChat;

	uint32 PlayerId;
	uint32 ZombieId;
	uint32 MyCharacterNumber;
	uint32 OtherCharacterNumber;
	//FVector NewLocation;
	//std::atomic <bool> b_attack{ false };
	bool b_attack = false;
	uint32 m_attacktype;
	std::atomic <bool> b_GetItem{ false };
	bool e_NWeapon = false;
	bool e_TWeapon = false;
	bool e_BHItem = false;
	bool e_HItem = false;
	bool e_KeyItem = false;
	std::string ItemName;
	bool b_run = false;			// 뛰기 모드 ON/OFF
	bool IsRlyRun = false;			// 실제 캐릭터가 뛰고 있는지
	bool b_move_forward = false;
	bool b_move_left = false;
	bool sendRun = false;
	bool sendjump = false;
	bool b_clear = false;
	
	float TimeSinceLastSend;

};
