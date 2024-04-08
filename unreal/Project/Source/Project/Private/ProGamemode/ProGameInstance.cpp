// Fill out your copyright notice in the Description page of Project Settings.


#include "ProGamemode/ProGameInstance.h"
#include "GameFramework/GameModeBase.h"
#include "ProGamemode/OneGameModeBase.h"
#include "Kismet/GameplayStatics.h"


UProGameInstance::UProGameInstance()
{
    ClientSocketPtr = nullptr;
}

UProGameInstance::~UProGameInstance()
{
}

void UProGameInstance::Init()
{
    Super::Init();

    m_iChoicedCharacterNumber = 0;
    GameStateClass = nullptr;
}

void UProGameInstance::InitSocket()
{
	if (!ConnectNetwork) return;

	// ClientSocket �ν��Ͻ� ���� �� �ʱ�ȭ
    ClientSocketPtr = new ClientSocket(this);
    ConnectNetwork = false;
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
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "UProGameInstance::ChangeOneGameMode()");
    // ���� ���� ��������
    UWorld* World = GetWorld();
    if (World)
    {
        // ���ο� ���� ��� Ŭ������ �ε��� ���� �̸�
        FString LevelName = "/Game/ThirdPerson/Maps/ThirdPersonMap";

        // ���ο� ���� �ε�
        UGameplayStatics::OpenLevel(World, FName(*LevelName), true, "GameMode=ProGamemode/OneGameModeBase");
    }
}
