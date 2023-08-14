// Fill out your copyright notice in the Description page of Project Settings.


#include "ToomTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"

void AToomTanksGameMode::ActorDied(AActor* DeadActor)
{
    if(DeadActor == Tank){
        Tank->HanleDestruction();
        if(ToonTankPlayerController){
            // Tank->DisableInput(Tank->GetTankPlayerController());
            // Tank->GetTankPlayerController()->bShowMouseCursor = false;
            ToonTankPlayerController->SetPlayerEnalbedState(false);
        }
        GameOver(false);
    }
    else if(ATower* DestroyedTower = Cast<ATower>(DeadAcotr)){
        DestroyedTower->HandleDestruction();
        --TargetTowers;
        if(TargetTowers == 0){
            GameOver(true);
        }
    }
}


void AToomTanksGameMode::BeginPlay()
{
    Super::BeginPlay();
    // Tank = UGameplayStatics::GetPlayerPawn(this, 0);
    HandleGameStart();
}

void AToomTanksGameMode::HandleGameStart()
{
    TargetTowers = GetTargetTowerCount();
    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
    ToonTankPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

    void StartGame();

    if(ToonTanksPlayerController){
        ToonTanksPlayerController->SetPlayerEnabledState(false);
        
        FTimerHandle PlayerEnableTimerHandle;
        FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreatedUObject(
            ToonTanksPlayerController,
            &AToonTanksPlayerController::SetPlayerEnabledState,
            true
        );

        GetWorldTimeManager().SetTimer(PlayerEnableTimerHandle,
            PlayerEnableTimerDelegate,
            StartDelay(),
            false
        );
    }
}

int32  AToomTanksGameMode::GetTargetTowerCount()
{
    TArray<AActor*> Towers;
    UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Towers);
    return Towers.Num();
}