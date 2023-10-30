// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerCharacter.h"
#include "CustomUI.generated.h"

/**
 * 
 */
UCLASS()
class REALPROJECT_API UCustomUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UCustomUI(const FObjectInitializer& ObjectInitializer);
	
	virtual void Init() {};

public:
	APlayerCharacter* Player;
};
