// Fill out your copyright notice in the Description page of Project Settings.


#include "RunningZombieAIController.h"


ARunningZombieAIController::ARunningZombieAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/BB_RZombieV.BB_RZombieV"));
	if (BBObject.Succeeded()) {
		RunningZombieBlackBoardAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/BT_RZombieTree.BT_RZombieTree"));
	if (BTObject.Succeeded()) {
		RunningZombieAIBehavior = BTObject.Object;
	}
}

void ARunningZombieAIController::BeginPlay()
{
	Super::BeginPlay();

	//if (RunningZombieAIBehavior != nullptr) {
	//	RunBehaviorTree(RunningZombieAIBehavior);

	//	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	//	GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
	//}
}

void ARunningZombieAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	//if (LineOfSightTo(PlayerPawn)) {
	//	GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
	//	GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), PlayerPawn->GetActorLocation());
	//}
	//else {
	//	GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
	//}
}
