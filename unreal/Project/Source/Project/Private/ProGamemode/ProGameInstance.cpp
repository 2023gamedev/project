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
    // ���� ���� ��������
    UWorld* World = GetWorld();
    if (World)
    {
        // ���ο� ���� ��� Ŭ������ �ε��� ���� �̸�
        FString LevelName = "ThirdPersonMap";

        // ���ο� ���� �ε�
        UGameplayStatics::OpenLevel(World, FName(*LevelName), true, "GameMode=OneGameModeBase");
    }
}
