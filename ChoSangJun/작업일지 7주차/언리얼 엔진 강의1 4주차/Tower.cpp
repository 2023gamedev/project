// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"


void ATower::Tick(flaot DeltaTime)
{
    Super::Tick(DeltaTime);

    // // Find the distance to the Tank
    // if(Tank){
    //     float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());

    //     // Check to see if the Tank is in range
    //     if(Distance <= FireRange){
    //         // If in range, rotate turret toward Tank
    //         RotateTurret(Tank->GetActorLocation());
    //     }
    // }

    if(InFireRange()){
        RotateTurret(Tank->GetActorLocation());
    }
}

void ATower::HandleDestruction()
{
    Super::HandleDestrtuction();
    Destroy();
}

void ATower::BeginPlay()
{
    Super::BeginPlay();

    Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

    GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}


void ATower::CheckFireCondition()
{
    // if(Tank){
    //     float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
    //     if (Distance <= FireRange){
    //         Fire();
    //     }
    // }

    if(Tank == nullptr){
        return;
    }
    if(InFireRange() && Tank->bAlive){
        Fire();
    }
}

bool ATower::InFireRange()
{
    if (Tank){
        float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());
        if(Distance <= FireRange){
            return true;
        }
    }

    return false;
}