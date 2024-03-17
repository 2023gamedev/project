// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ALobbyGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void BeginPlay() override;

	// 시작 시 로비 게임모드와 맵에서 시작, 버튼을 누를 시(캐릭터가 정해질 시) 맵과 게임모드 전환예정

	// 만들어야 할 부분들
	// 버튼UI, 마지막 입력값을 저장할 변수(캐릭터가 뭔지를 정하기 위해)
};
