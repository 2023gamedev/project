// Fill out your copyright notice in the Description page of Project Settings.


#include "ProGamemode/ProGameInstance.h"
#include "GameFramework/GameModeBase.h"
#include "ProGamemode/OneGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "ProUI/LoadingUI.h"
#include "Engine/AssetManager.h"





UProGameInstance::UProGameInstance()
{
    ClientSocketPtr = nullptr;
    ConnectNetwork = true;

    //// 로딩 UI를 생성 및 화면에 표시
    //if (!LoadingUI)
    //{
    //    TSubclassOf<UUserWidget> WidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/UI/BP_LoadingUI.BP_LoadingUI_C"));
    //    if (WidgetClass)
    //    {
    //        LoadingUI = Cast<ULoadingUI>(CreateWidget(GetWorld(), WidgetClass));
    //    }
    //    else
    //    {
    //        UE_LOG(LogTemp, Error, TEXT("Failed to load BP_LoadingUI class."));
    //        return;
    //    }
    //}
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
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Success!")));
  
    if (ClientSocketPtr->Init())
    {
        ConnectNetwork = false;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to initialize network connection."));
    }
}
//
//void UProGameInstance::LoadLevelWithLoadingUI(FName LevelName)
//{ 
//    // 로딩 UI를 생성 및 화면에 표시
//    if (!LoadingUI)
//    {
//        TSubclassOf<UUserWidget> WidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/UI/BP_LoadingUI.BP_LoadingUI"));
//        if (WidgetClass)
//        {
//            LoadingUI = Cast<ULoadingUI>(CreateWidget(GetWorld(), WidgetClass));
//            if (LoadingUI)
//            {
//                LoadingUI->AddToViewport();
//            }
//        }
//    }
//
//    // 비동기 로딩 시작
//    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
//    FSoftObjectPath LevelPath = FSoftObjectPath(LevelName.ToString());
//    Streamable.RequestAsyncLoad(
//        LevelPath,
//        FStreamableDelegate::CreateUObject(this, &UProGameInstance::OnLevelLoadComplete, LevelName),
//        FStreamableManager::AsyncLoadHighPriority
//    );
//
//    // 로딩 UI에 초기값 설정
//    if (LoadingUI)
//    {
//        LoadingUI->UpdateLoadingBar(0.0f, 100.0f);
//    }
//
//    // 진행 상태 추적 (Tick이나 Timer로 업데이트)
//    GetWorld()->GetTimerManager().SetTimer(
//        LoadingProgressHandle,
//        this,
//        &UProGameInstance::UpdateLoadingProgress,
//        0.1f,
//        true
//    );
//}
//
//void UProGameInstance::UpdateLoadingProgress()
//{
//    float Progress = FMath::RandRange(0.0f, 1.0f); // 비동기 진행률 추정 (API로 대체 가능)
//    if (LoadingUI)
//    {
//        LoadingUI->UpdateLoadingBar(Progress * 100.0f, 100.0f);
//    }
//}


void UProGameInstance::LoadLevelWithLoadingUI(FName LevelName)
{
    // 로딩 UI를 생성 및 화면에 표시
    if (!LoadingUI)
    {
        TSubclassOf<UUserWidget> WidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/UI/BP_LoadingUI.BP_LoadingUI_C"));
        if (WidgetClass)
        {
            LoadingUI = Cast<ULoadingUI>(CreateWidget(GetWorld(), WidgetClass));
            if (LoadingUI)
            {
                LoadingUI->AddToViewport();
                UE_LOG(LogTemp, Log, TEXT("Loading UI displayed."));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to load BP_LoadingUI class."));
            return;
        }
    }

    if (LoadingUI)
    {
        LoadingUI->AddToViewport();
        UE_LOG(LogTemp, Log, TEXT("Loading UI displayed."));
    }

    // 비동기 로딩 시작
    FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
    FSoftObjectPath LevelPath = FSoftObjectPath(LevelName.ToString());
    StreamableHandle = Streamable.RequestAsyncLoad(
        LevelPath,
        FStreamableDelegate::CreateUObject(this, &UProGameInstance::OnLevelLoadComplete, LevelName),
        FStreamableManager::AsyncLoadHighPriority
    );

    if (!StreamableHandle.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to initiate async load for level: %s"), *LevelName.ToString());
        return;
    }

    // 진행 상태 추적 (Tick 또는 Timer로 업데이트)
    LevelNameToLoad = LevelName; // 로드 중인 레벨 이름 저장
    GetWorld()->GetTimerManager().SetTimer(
        LoadingProgressHandle,
        this,
        &UProGameInstance::UpdateLoadingProgress,
        0.5f, // 업데이트 주기
        true
    );

    // 로딩 UI 초기화
    if (LoadingUI)
    {
        LoadingUI->UpdateLoadingBar(0.0f, 100.0f); // 초기값 설정
    }
}

void UProGameInstance::UpdateLoadingProgress()
{
    if (StreamableHandle.IsValid())
    {
        float Progress = StreamableHandle->GetProgress(); // 0.0f ~ 1.0f 사이 값 반환
        if (LoadingUI)
        {
            LoadingUI->UpdateLoadingBar(Progress * 100.0f, 100.0f); // UI에 진행률 업데이트
        }

        // 로딩 완료 시 타이머 해제
        if (Progress >= 1.0f)
        {
            GetWorld()->GetTimerManager().ClearTimer(LoadingProgressHandle);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("StreamableHandle is invalid during progress update."));
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
        UGameplayStatics::OpenLevel(World, LevelName);
        UE_LOG(LogTemp, Log, TEXT("Level %s loaded successfully."), *LevelName.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get the current world during level load."));
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
    //ShowLoadingScreen();
    //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "UProGameInstance::ChangeOneGameMode()");
   
    // 현재 월드 가져오기
    UWorld* World = GetWorld();
    if (World)
    {
        // 새로운 게임 모드 클래스를 로드할 레벨 이름
        FString LevelName = "/Game/ThirdPerson/Maps/ThirdPersonMap";

        //LoadLevelWithLoadingUI(FName(*LevelName));


        LoadLevelWithLoadingScreen(FName(*LevelName));

        // 새로운 레벨 로드
        // UGameplayStatics::OpenLevel(World, FName(*LevelName), true, "GameMode=ProGamemode/OneGameModeBase");
    }
}

void UProGameInstance::ShowLoadingScreen()
{
    // 로딩 UI를 생성 및 화면에 표시
    if (!LoadingUI)
    {
        LoadingScreenClass = LoadClass<ULoadingUI>(nullptr, TEXT("/Game/UI/BP_LoadingUI.BP_LoadingUI_C"));
        if (LoadingScreenClass)
        {
            LoadingUI = Cast<ULoadingUI>(CreateWidget(GetWorld(), LoadingScreenClass));
            if (LoadingUI)
            {
                LoadingUI->AddToViewport();
                UE_LOG(LogTemp, Log, TEXT("Loading UI displayed."));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to load BP_LoadingUI class."));
            return;
        }
    }

    //// 렌더링 강제 업데이트
    //FSlateApplication::Get().Tick();
    //FSlateApplication::Get().GetRenderer()->FlushCommands();
    //FPlatformProcess::Sleep(0.1f); // 아주 짧은 대기 시간을 추가
}

void UProGameInstance::HideLoadingScreen()
{
    if (LoadingScreenWidget)
    {
        LoadingScreenWidget->RemoveFromViewport();
        LoadingScreenWidget = nullptr;
    }
}

void UProGameInstance::LoadLevelWithLoadingScreen(FName LevelName)
{
    ShowLoadingScreen();

    // GameMode URL 추가
    //FString LevelWithGameMode = FString::Printf(TEXT("%s?game=%s"), *LevelName.ToString(), TEXT("ProGamemode/OneGameModeBase"));

    //UE_LOG(LogTemp, Log, TEXT("Attempting to open level: %s"), *LevelWithGameMode);

    //// 맵 전환
    //UGameplayStatics::OpenLevel(this, FName(*LevelWithGameMode));


    // 현재 월드 가져오기
    UWorld* World = GetWorld();
    if (World)
    {

        UGameplayStatics::OpenLevel(World, LevelName, true, "GameMode=ProGamemode/OneGameModeBase");
    }



    //// GameMode URL 추가
    //FString LevelWithGameMode = FString::Printf(TEXT("%s?game=%s"), *LevelName.ToString(), TEXT("ProGamemode/OneGameModeBase"));

    //FLatentActionInfo LatentInfo;
    //LatentInfo.CallbackTarget = this;
    //LatentInfo.ExecutionFunction = "OnLevelLoaded";
    //LatentInfo.Linkage = 0;
    //LatentInfo.UUID = __LINE__;

    //// 맵 로드
    //UGameplayStatics::LoadStreamLevel(this, FName(*LevelWithGameMode), true, false, LatentInfo);
}

void UProGameInstance::OnLevelLoaded()
{
    HideLoadingScreen();
}

