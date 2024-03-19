// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameModeManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API AGameModeManager : public AGameStateBase // 필요없어짐 Gameinstance가 할일을 하는중
{
	GENERATED_BODY()

public:
    // Constructor
    AGameModeManager();

    // Function to change the game mode
    void ChangeGameMode(TSubclassOf<AGameModeBase> NewGameMode);



protected:
    // The variable to hold the new game mode class
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Mode")
    TSubclassOf<AGameModeBase> NewGameModeClass;

};
