// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"

#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"


void AShooterAIController::BeginPlay()
{
    Super::BeginPlay();

    // APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);    

    // SetFocus(PlayerPawn);

    if(AIBehavior != nullptr){
        RunBehaviorTree(AIBehavior);

        APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
        

    }


}

void AShooterAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    // APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);    
    


    // if(LineOfSightTo(PlayerPawn)){
    //     GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
    //     GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), PlayerPawn->GetActorLocation());
    // }
    // else{
    //     GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
    // }
}

bool AShooterAIController::IsDead() const
{
    AShooterCharacter* ControlledCharacter = Cast<AShooterCharacter>(GetPawn());
    if(ControlledCharacter != nullptr){
        return ControlledCharacter->IsDead();
    }

    return true;
}