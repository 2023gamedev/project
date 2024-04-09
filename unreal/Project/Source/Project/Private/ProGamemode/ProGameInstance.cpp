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
    // 현재 월드 가져오기
    UWorld* World = GetWorld();
    if (World)
    {
        // 새로운 게임 모드 클래스를 로드할 레벨 이름
        FString LevelName = "/Game/ThirdPerson/Maps/ThirdPersonMap";

        // 새로운 레벨 로드
        //UGameplayStatics::OpenLevel(World, FName(*LevelName), true, "GameMode=ProGamemode/OneGameModeBase");
    }
}
