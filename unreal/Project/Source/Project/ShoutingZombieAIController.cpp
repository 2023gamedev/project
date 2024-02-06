// Fill out your copyright notice in the Description page of Project Settings.


#include "ShoutingZombieAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AShoutingZombieAIController::TargetKey(TEXT("Target"));
const FName AShoutingZombieAIController::StartLocationKey(TEXT("StartLocation"));

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


		// ���� �ʿ�
		if (OwningPawn)
		{
			APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
			GetBlackboardComponent()->SetValueAsVector(StartLocationKey, GetPawn()->GetActorLocation());
		//	GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());

		}
		else // gamemode����(C++�ڵ�) ���� ������ �κ��� ũ���ð� ���. GetPawn()����
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

	//// ���� �ʿ�
	//if (PlayerPawn)
	//{
	//	GetBlackboardComponent()->SetValueAsVector(StartLocationKey, GetPawn()->GetActorLocation());
	//	//	GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
	//}
	//else // gamemode����(C++�ڵ�) ���� ������ �κ��� ũ���ð� ���. GetPawn()����
	//{
	//	UE_LOG(LogTemp, Error, TEXT("Owning pawn is NULL in AZombieAIController::BeginPlay"));
	//	return;
	//}

	if (LineOfSightTo(PlayerPawn)) {
		GetBlackboardComponent()->SetValueAsVector(StartLocationKey, GetPawn()->GetActorLocation()); // ���� �ʿ�
		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), PlayerPawn->GetActorLocation());
	 	GetBlackboardComponent()->SetValueAsObject(TargetKey, PlayerPawn);
	}
	else {
		GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
		GetBlackboardComponent()->SetValueAsObject(TargetKey, nullptr);
	}
}

//void AShoutingZombieAIController::OnPossess(APawn* aPawn)
//{
//	OnPossess(aPawn);
//}
