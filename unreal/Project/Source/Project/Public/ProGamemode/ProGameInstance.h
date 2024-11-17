// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ClientSocket.h"
#include "ProGameInstance.generated.h"

/**
 * 
 */

// Instance들 (전달하고 싶은(Global 변수처럼) 모은 Class

UCLASS()
class PROJECT_API UProGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
    
    UProGameInstance();
    virtual ~UProGameInstance() override;

    virtual void Init() override;

    void InitSocket();

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

    //UPROPERTY(VisibleAnywhere)
    ClientSocket* ClientSocketPtr;

    bool ConnectNetwork;

    FString MyUserName;

};
