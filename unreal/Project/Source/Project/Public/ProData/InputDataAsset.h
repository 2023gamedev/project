// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputAction.h"

#include "InputDataAsset.generated.h"

/**
 * 
 */
// InputAction 용 클래스
UCLASS()
class PROJECT_API UInputDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputMoveForward;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputMoveLeft;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputTurn;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputLookUp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputRun;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputGetItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputLightOnOff;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputInventoryOnOff;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputJump;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputAttack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputQuickNWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputQuickTWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputQuickBHItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputQuickHItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputQuickKeyItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputChatToggle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputUpSTR;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputDownSTR;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputUpSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputDownSpeed;
};
