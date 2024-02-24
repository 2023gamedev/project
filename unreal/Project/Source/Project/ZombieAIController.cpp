// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AZombieAIController::TargetKey(TEXT("Target"));
const FName AZombieAIController::StartLocationKey(TEXT("StartLocation"));
const FName AZombieAIController::PatrolLocationKey(TEXT("PatrolLocation"));

AZombieAIController::AZombieAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/BB_ZombieV.BB_ZombieV"));
	if (BBObject.Succeeded()) {
		BlackBoardAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/BT_ZombieTree.BT_ZombieTree"));
	if (BTObject.Succeeded()) {
		AIBehavior = BTObject.Object;
	}

}

void AZombieAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIBehavior != nullptr) {
		RunBehaviorTree(AIBehavior);

		AActor* OwningPawn = GetPawn();

		// ���� �ʿ�
		if (OwningPawn)
		{
			// GetPawn()�� ��ȿ�� ��� ������ ��� �����մϴ�.
			// ...
			//GetBlackboardComponent()->SetValueAsVector(StartLocationKey, GetPawn()->GetActorLocation());
			//GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
		}
		else // gamemode����(C++�ڵ�) ���� ������ �κ��� ũ���ð� ���. GetPawn()����
		{
			UE_LOG(LogTemp, Error, TEXT("Owning pawn is NULL in AZombieAIController::BeginPlay"));
			return;
		}
	}


}

void AZombieAIController::Tick(float DeltaTime)
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

void AZombieAIController::SetStartLocationValue(FVector startlocation)
{
	GetBlackboardComponent()->SetValueAsVector(StartLocationKey, startlocation);
}

void AZombieAIController::SetPatrolLocationValue(FVector patrollocation)
{
	GetBlackboardComponent()->SetValueAsVector(PatrolLocationKey, patrollocation);
}



