// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "ShooterAIController.h"

 void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled)
 {
    Super::PawnKilled(PawnKilled);

    //UE_LOG(LogTemp, Warning, TEXT("A pawn was killed"));

    APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
    if(PlayerController != nullptr){
        //PlayerController->GameHasEnded(nullptr, false);
        EndGame(false);
    }

    for(AShooterAIController* Controller : TActorRange<AShooterAIController>(GetWorld())){
        if(!Controller->IsDead()){
            return;
        }
    }

    EndGame(true);
 }

 void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner)
 {
    for(AController* Controller : TActorRange<AController>(GetWorld())){
        // bool bIsPlayerController = Controller->IsPlayerController();


        // if(bIsPlayerWinner){
        //     Controller->GameHasEnded(nullptr, bIsPlayerController)
        // }
        // else{
        //     Controller->GameHasEnded(nullptr, !bIsPlayerController)
        // }
        bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
        Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
    }
 }
