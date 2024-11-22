// Fill out your copyright notice in the Description page of Project Settings.


#include "ProGamemode/ProGameInstance.h"
#include "GameFramework/GameModeBase.h"
#include "ProGamemode/OneGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "ProUI/LoadingUI.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
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
    
    m_iChoicedCharacterNumber = 1;
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

void UProGameInstance::LoadLevelWithLoadingUI(FName LevelName)
{
    // 로딩 UI를 생성 및 화면에 표시
    if (!LoadingUI)
    {
        TSubclassOf<UUserWidget> WidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/PathTo/YourLoadingUI"));
        if (WidgetClass)
        {
            LoadingUI = Cast<ULoadingUI>(CreateWidget(GetWorld(), WidgetClass));
            if (LoadingUI)
            {
                LoadingUI->AddToViewport();
            }
        }
    }

    // 비동기 로딩 시작
    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
    FSoftObjectPath LevelPath = FSoftObjectPath(LevelName.ToString());
    Streamable.RequestAsyncLoad(
        LevelPath,
        FStreamableDelegate::CreateUObject(this, &UProGameInstance::OnLevelLoadComplete, LevelName),
        FStreamableManager::AsyncLoadHighPriority
    );

    // 로딩 UI에 초기값 설정
    if (LoadingUI)
    {
        LoadingUI->UpdateLoadingBar(0.0f, 100.0f);
    }

    // 진행 상태 추적 (Tick이나 Timer로 업데이트)
    GetWorld()->GetTimerManager().SetTimer(
        LoadingProgressHandle,
        this,
        &UProGameInstance::UpdateLoadingProgress,
        0.1f,
        true
    );
}

void UProGameInstance::UpdateLoadingProgress()
{
    float Progress = FMath::RandRange(0.0f, 1.0f); // 비동기 진행률 추정 (API로 대체 가능)
    if (LoadingUI)
    {
        LoadingUI->UpdateLoadingBar(Progress * 100.0f, 100.0f);
    }
}

void UProGameInstance::OnLevelLoadComplete(FName LevelName)
{
    // 로딩 완료 시 UI 제거
    if (LoadingUI)
    {
        LoadingUI->RemoveFromViewport();
        LoadingUI = nullptr;
    }

    // 현재 월드 가져오기
    UWorld* World = GetWorld();
    if (World)
    {

        // 레벨 전환
        UGameplayStatics::OpenLevel(World, LevelName, true, "GameMode=ProGamemode/OneGameModeBase");
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

        LoadLevelWithLoadingUI(FName(*LevelName));

        // 새로운 레벨 로드
        //UGameplayStatics::OpenLevel(World, FName(*LevelName), true, "GameMode=ProGamemode/OneGameModeBase");
    }
}

