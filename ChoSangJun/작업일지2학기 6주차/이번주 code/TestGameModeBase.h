// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TestGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class REALPROJECT_API ATestGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ATestGameModeBase();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	
};
