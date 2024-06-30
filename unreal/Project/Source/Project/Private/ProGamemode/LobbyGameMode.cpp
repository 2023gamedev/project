// Fill out your copyright notice in the Description page of Project Settings.


#include "ProGamemode/LobbyGameMode.h"
#include "ProCharacter/LobbyPlayer.h"
#include "ProCharacter/LobbyPlayerController.h"
#include "ProUI/StartGameUI.h"
#include "ProUI/ChoiceCharacterUI.h"
#include "EngineUtils.h"
#include "GameFramework/DefaultPawn.h"
#include "ProGamemode/OneGameModeBase.h"
#include "ProGamemode/GameModeManager.h"
#include "Kismet/GameplayStatics.h"
#include "ProGamemode/ProGameInstance.h"
#include "ClientSocket.h"


ALobbyGameMode::ALobbyGameMode()
{
	DefaultPawnClass = ALobbyPlayer::StaticClass();
	PlayerControllerClass = ALobbyPlayerController::StaticClass();

	
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

    ALobbyPlayer* DefaultPawn = nullptr;
    UWorld* World = GetWorld();
    if (World) {

        for (TActorIterator<ALobbyPlayer> ActorItr(World); ActorItr; ++ActorItr) {
            UE_LOG(LogTemp, Error, TEXT("DefaultPawn Complete"));
            DefaultPawn = *ActorItr;
            if (DefaultPawn) {
                break;
            }
        }

        DefaultPawn->ChoicedGirlCharcaterD.BindUObject(this, &ALobbyGameMode::ChoiceGirl);
        DefaultPawn->ChoicedEmployeeCharcaterD.BindUObject(this, &ALobbyGameMode::ChoiceEmployee);
        DefaultPawn->ChoicedIdolCharcaterD.BindUObject(this, &ALobbyGameMode::ChoiceIdol);
        DefaultPawn->ChoicedFireFighterCharcaterD.BindUObject(this, &ALobbyGameMode::ChoiceFireFighter);
    }
}

// 이제 메인 맵과 게임모드로 이동
void ALobbyGameMode::LobbyStageClear()
{
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ALobbyGameMode::CheckServerType, 0.5f, true);
}

void ALobbyGameMode::CheckServerType()
{
    UProGameInstance* GameInstance = Cast<UProGameInstance>(GetGameInstance());
    if (GameInstance->ClientSocketPtr->CurrentServerType == ServerType::GAME_SERVER) {
        GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
        GameInstance->ChangeOneGameMode();
    }
}

void ALobbyGameMode::ChoiceGirl()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "ALobbyGameMode::ChoiceGirl!");

    UProGameInstance* GameInstance = Cast<UProGameInstance>(GetGameInstance());

    GameInstance->SetChoicedCharacterNumber(0);
    SendReady();
    LobbyStageClear();
}

void ALobbyGameMode::ChoiceEmployee()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "ALobbyGameMode::ChoiceEmployee!");

    UProGameInstance* GameInstance = Cast<UProGameInstance>(GetGameInstance());
    GameInstance->SetChoicedCharacterNumber(1);
    SendReady();
    LobbyStageClear();
}

void ALobbyGameMode::ChoiceIdol()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "ALobbyGameMode::ChoiceIdol!");
    UProGameInstance* GameInstance = Cast<UProGameInstance>(GetGameInstance());
    GameInstance->SetChoicedCharacterNumber(2);
    SendReady();
    LobbyStageClear();
}

void ALobbyGameMode::ChoiceFireFighter()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "ALobbyGameMode::ChoiceFireFighter!");

    UProGameInstance* GameInstance = Cast<UProGameInstance>(GetGameInstance());
    GameInstance->SetChoicedCharacterNumber(3);
    SendReady();
    LobbyStageClear();
}


void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void ALobbyGameMode::SendReady()
{
    UProGameInstance* GameInstance = Cast<UProGameInstance>(GetGameInstance());
    uint32 MyPlayerId = GameInstance->ClientSocketPtr->GetMyPlayerId();
    Protocol::CS_Ready Packet;

    Packet.set_playerid(MyPlayerId);
    Packet.set_ready(true);

    // 직렬화
    std::string serializedData;
    Packet.SerializeToString(&serializedData);

    // 직렬화된 데이터를 서버로 전송
    bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());
}

