// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */



// LobbyMap에서의 Gamemode 클래스
UCLASS()
class PROJECT_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALobbyGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	void SendReady();
	void SendChoice(uint32 character_type);

	virtual void BeginPlay() override;
	void LobbyStageClear();

	void CheckServerType();

	void ChoiceGirl();
	void ChoiceEmployee();
	void ChoiceIdol();
	void ChoiceFireFighter();

	FTimerHandle TimerHandle;
};
