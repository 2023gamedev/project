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
	void Turn(const FInputActionValue& Value);
	void LookUp(const FInputActionValue& Value);
	void Run(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void GetItem(const FInputActionValue& Value);
	void LightOnOff(const FInputActionValue& Value);
	void InventoryOnOff(const FInputActionValue& Value);

	void BehaviorToItem(const FInputActionValue& Value);
	void Attack();
	void Healing();
	void BleedHealing();
	void PlayKey();
	void Throw();

	void QuickNWeapon(const FInputActionValue& Value);
	void QuickTWeapon(const FInputActionValue& Value);
	void QuickBHItem(const FInputActionValue& Value);
	void QuickHItem(const FInputActionValue& Value);
	void QuickKeyItem(const FInputActionValue& Value);

	void DisabledControllerInput();


	void ServerHandleAttack();


public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void CheckAndSendMovement();

	void Send_Attack();

	void Send_Equipment();

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

public:
	UProGameInstance* GameInstance;
	PlayerData recvPlayerData;
	PlayerAttack recvPlayerAttack;
	uint32 PlayerId;
	uint32 MyCharacterNumber;
	uint32 OtherCharacterNumber;
	FVector NewLocation;
	//std::atomic <bool> b_attack{ false };
	bool b_attack = false;
	std::atomic <bool> b_GetItem{ false };
	bool e_NWeapon = false;
	bool e_TWeapon = false;
	bool e_BHItem = false;
	bool e_HItem = false;
	bool e_KeyItem = false;


};
