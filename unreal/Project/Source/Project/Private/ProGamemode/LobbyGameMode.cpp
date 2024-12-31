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
            UE_LOG(LogTemp, Log, TEXT("DefaultPawn Complete"));
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
    //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "ALobbyGameMode::ChoiceGirl!");

    UProGameInstance* GameInstance = Cast<UProGameInstance>(GetGameInstance());

    GameInstance->SetChoicedCharacterNumber(1);
    SendChoice(1);
}

void ALobbyGameMode::ChoiceEmployee()
{
    //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "ALobbyGameMode::ChoiceEmployee!");

    UProGameInstance* GameInstance = Cast<UProGameInstance>(GetGameInstance());
    GameInstance->SetChoicedCharacterNumber(2);
    SendChoice(2);
}

void ALobbyGameMode::ChoiceIdol()
{
    //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "ALobbyGameMode::ChoiceIdol!");
    UProGameInstance* GameInstance = Cast<UProGameInstance>(GetGameInstance());
    GameInstance->SetChoicedCharacterNumber(3);
    SendChoice(3);
}

void ALobbyGameMode::ChoiceFireFighter()
{
    //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "ALobbyGameMode::ChoiceFireFighter!");

    UProGameInstance* GameInstance = Cast<UProGameInstance>(GetGameInstance());
    GameInstance->SetChoicedCharacterNumber(4);
    SendChoice(4);
}


void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void ALobbyGameMode::SendReady()
{
    UProGameInstance* GameInstance = Cast<UProGameInstance>(GetGameInstance());
    uint32 MyPlayerId = GameInstance->ClientSocketPtr->GetMyPlayerId();
    Protocol::SelectReady Packet;

    Packet.set_playerid(MyPlayerId);
    Packet.set_type(5);
    Packet.set_ready(true);

    // 직렬화
    std::string serializedData;
    Packet.SerializeToString(&serializedData);

    // 직렬화된 데이터를 서버로 전송
    bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());
}

void ALobbyGameMode::SendChoice(uint32 character_type)
{
    UProGameInstance* GameInstance = Cast<UProGameInstance>(GetGameInstance());
    uint32 MyPlayerId = GameInstance->ClientSocketPtr->GetMyPlayerId();
    Protocol::Select_Character Packet;

    Packet.set_playerid(MyPlayerId);
    Packet.set_type(7);
    Packet.set_character_type(character_type);

    // 직렬화
    std::string serializedData;
    Packet.SerializeToString(&serializedData);

    // 직렬화된 데이터를 서버로 전송
    bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());
}