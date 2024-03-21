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

        //DefaultPawn->StartGameUIWidget->QuitGame.BindUObject(this, &ALobbyGameMode::QuitGame);
        DefaultPawn->ChoicedGirlCharcaterD.BindUObject(this, &ALobbyGameMode::ChoiceGirl);
        DefaultPawn->ChoicedEmployeeCharcaterD.BindUObject(this, &ALobbyGameMode::ChoiceEmployee);
        DefaultPawn->ChoicedIdolCharcaterD.BindUObject(this, &ALobbyGameMode::ChoiceIdol);
        DefaultPawn->ChoicedFireFighterCharcaterD.BindUObject(this, &ALobbyGameMode::ChoiceFireFighter);
    }
}

// 이제 메인 맵과 게임모드로 이동
void ALobbyGameMode::LobbyStageClear()
{
   UProGameInstance* GameInstance = Cast<UProGameInstance>(GetGameInstance());
   GameInstance->ChangeOneGameMode();
}

void ALobbyGameMode::ChoiceGirl()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "ALobbyGameMode::ChoiceGirl!");

    UProGameInstance* GameInstance = Cast<UProGameInstance>(GetGameInstance());

    GameInstance->SetChoicedCharacterNumber(0);
    LobbyStageClear();
}

void ALobbyGameMode::ChoiceEmployee()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "ALobbyGameMode::ChoiceEmployee!");

    UProGameInstance* GameInstance = Cast<UProGameInstance>(GetGameInstance());
    GameInstance->SetChoicedCharacterNumber(1);
    LobbyStageClear();
}

void ALobbyGameMode::ChoiceIdol()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "ALobbyGameMode::ChoiceIdol!");
    UProGameInstance* GameInstance = Cast<UProGameInstance>(GetGameInstance());
    GameInstance->SetChoicedCharacterNumber(2);
    LobbyStageClear();
}

void ALobbyGameMode::ChoiceFireFighter()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "ALobbyGameMode::ChoiceFireFighter!");

    UProGameInstance* GameInstance = Cast<UProGameInstance>(GetGameInstance());
    GameInstance->SetChoicedCharacterNumber(3);
    LobbyStageClear();
}

void ALobbyGameMode::QuitGame()
{
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}


