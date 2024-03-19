// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ProGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UProGameInstance : public UGameInstance
{
	GENERATED_BODY()

    virtual void Init() override;

public:

    void SetChoicedCharacterNumber(int number) { m_iChoicedCharacterNumber = number; }
    int GetChoicedCharacterNumber() { return m_iChoicedCharacterNumber; }

    UPROPERTY(BlueprintReadOnly)
    TSubclassOf<AGameStateBase> GameStateClass;

    UFUNCTION(BlueprintCallable)
    void SetGameState(TSubclassOf<AGameStateBase> gamestate);

    UFUNCTION(BlueprintCallable)
    TSubclassOf<AGameStateBase> GetGameState();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Mode")
    TSubclassOf<AGameModeBase> NewGameModeClass;

    UPROPERTY(VisibleAnywhere)
    int m_iChoicedCharacterNumber;

    void ChangeOneGameMode();

};
