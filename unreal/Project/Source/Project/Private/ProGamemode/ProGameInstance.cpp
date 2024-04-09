// Fill out your copyright notice in the Description page of Project Settings.


#include "ProGamemode/ProGameInstance.h"
#include "GameFramework/GameModeBase.h"
#include "ProGamemode/OneGameModeBase.h"
#include "Kismet/GameplayStatics.h"


UProGameInstance::UProGameInstance()
{
    ClientSocketPtr = nullptr;
    ConnectNetwork = true;
}

UProGameInstance::~UProGameInstance()
{
}

void UProGameInstance::Init()
{
    Super::Init();
    
    m_iChoicedCharacterNumber = 0;
    GameStateClass = nullptr;
    InitSocket();
}

void UProGameInstance::InitSocket()
{
    if (!ConnectNetwork) return;
    if (ClientSocketPtr != nullptr) return;

    ClientSocketPtr = new ClientSocket(this);
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Success!")));
  
    if (ClientSocketPtr->Init())
    {
        ConnectNetwork = false;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to initialize network connection."));
    }
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
        //UGameplayStatics::OpenLevel(World, FName(*LevelName), true, "GameMode=ProGamemode/OneGameModeBase");
    }
}
