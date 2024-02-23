// Fill out your copyright notice in the Description page of Project Settings.


#include "ShoutingZombieAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AShoutingZombieAIController::TargetKey(TEXT("Target"));
const FName AShoutingZombieAIController::StartLocationKey(TEXT("StartLocation"));
const FName AShoutingZombieAIController::PatrolLocationKey(TEXT("PatrolLocation"));

AShoutingZombieAIController::AShoutingZombieAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/BB_SZombieV.BB_SZombieV"));
	if (BBObject.Succeeded()) {
		ShoutingZombieBlackBoardAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/BT_SZombieTree.BT_SZombieTree"));
	if (BTObject.Succeeded()) {
		ShoutingZombieAIBehavior = BTObject.Object;
	}

}

void AShoutingZombieAIController::BeginPlay()
{
	Super::BeginPlay();

	if (ShoutingZombieAIBehavior != nullptr) {
		RunBehaviorTree(ShoutingZombieAIBehavior);

		AActor* OwningPawn = GetPawn();


		// 수정 필요
		if (OwningPawn)
		{
			APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
			//GetBlackboardComponent()->SetValueAsVector(StartLocationKey, GetPawn()->GetActorLocation());
		//	GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());

		}
		else // gamemode에서(C++코드) 만들어서 적용한 부분은 크래시가 뜬다. GetPawn()에서
		{
			UE_LOG(LogTemp, Error, TEXT("Owning pawn is NULL in AZombieAIController::BeginPlay"));
			return;
		}
	}
}

void AShoutingZombieAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);


	if (LineOfSightTo(PlayerPawn)) {
		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), PlayerPawn->GetActorLocation());
	 	GetBlackboardComponent()->SetValueAsObject(TargetKey, PlayerPawn);
	}
	else {
		GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
		GetBlackboardComponent()->SetValueAsObject(TargetKey, nullptr);
	}
}

void AShoutingZombieAIController::SetStartLocationValue(FVector startlocation)
{
	GetBlackboardComponent()->SetValueAsVector(StartLocationKey, startlocation);
}

void AShoutingZombieAIController::SetPatrolLocationValue(FVector patrollocation)
{
	GetBlackboardComponent()->SetValueAsVector(PatrolLocationKey, patrollocation);
}

