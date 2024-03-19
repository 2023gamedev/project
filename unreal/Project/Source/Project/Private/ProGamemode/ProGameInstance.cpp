// Fill out your copyright notice in the Description page of Project Settings.


#include "ProGamemode/ProGameInstance.h"
#include "GameFramework/GameModeBase.h"
#include "ProGamemode/OneGameModeBase.h"
#include "Kismet/GameplayStatics.h"




void UProGameInstance::Init()
{
    Super::Init();

    m_iChoicedCharacterNumber = 0;
    GameStateClass = nullptr;
}

void UProGameInstance::SetGameState(TSubclassOf<AGameStateBase> gamestate)
{
	GameStateClass = gamestate;
}



TSubclassOf<AGameStateBase> UProGameInstance::GetGameState()
{
	return GameStateClass;
}

void UProGameInstance::ChangeOneGameMode()
{
    // 현재 월드 가져오기
    UWorld* World = GetWorld();
    if (World)
    {
        // 새로운 게임 모드 클래스를 로드할 레벨 이름
        FString LevelName = "ThirdPersonMap";

        // 새로운 레벨 로드
        UGameplayStatics::OpenLevel(World, FName(*LevelName), true, "GameMode=OneGameModeBase");
    }
}
