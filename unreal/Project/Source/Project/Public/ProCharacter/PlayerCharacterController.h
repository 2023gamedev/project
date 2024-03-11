// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProData/InputDataAsset.h"
#include "InputActionValue.h"

#include "PlayerCharacterController.generated.h"

/**
 * 
 */


class UInputMappingContext;
class UInputDataAsset;

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

	void QuickNWeapon(const FInputActionValue& Value);
	void QuickBHItem(const FInputActionValue& Value);
	void QuickHItem(const FInputActionValue& Value);
	void QuickTWeapon(const FInputActionValue& Value);
	void QuickKeyItem(const FInputActionValue& Value);

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void SetupInputComponent() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputDataAsset* InputActions;

};
