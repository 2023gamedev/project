// Fill out your copyright notice in the Description page of Project Settings.


#include "ProZombie/ZombieAIController.h"
#include "Kismet/GameplayStatics.h"
#include "ProCharacter/BaseCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GStruct.pb.h"
#include "ProZombie/BaseZombie.h"
#include "ProGamemode/OneGameModeBase.h"
#include "ProGamemode/ProGameInstance.h"

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

	}

	GameInstance = Cast<UProGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));



}

//void AZombieAIController::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
//
//
//	if (LineOfSightTo(PlayerPawn)) {
//		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
//		GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), PlayerPawn->GetActorLocation());
//		GetBlackboardComponent()->SetValueAsObject(TargetKey, PlayerPawn);
//	}
//	else {
//		GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
//		GetBlackboardComponent()->SetValueAsObject(TargetKey, nullptr);
//	}
//}

void AZombieAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//static float SearchInterval = 0.5f; // 0.5�ʸ��� �÷��̾� �˻�
	//static float TimeSinceLastSearch = 0.0f;
	//TimeSinceLastSearch += DeltaTime;

	//if (TimeSinceLastSearch >= SearchInterval)
	{
		//TimeSinceLastSearch = 0.0f; // Ÿ�̸� ����

		TArray<AActor*> Players;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), Players);

		APawn* NearestPawn = nullptr;
		float NearestDist = FLT_MAX;

		for (AActor* Player : Players)
		{
			APawn* TestPawn = Cast<APawn>(Player);
			if (TestPawn && LineOfSightTo(TestPawn))
			{
				float Dist = FVector::Dist(GetPawn()->GetActorLocation(), TestPawn->GetActorLocation());
				if (Dist < NearestDist)
				{
					NearestDist = Dist;
					NearestPawn = TestPawn;
				}
			}
		}

		// ������ ������Ʈ
		if (NearestPawn)
		{
			GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), NearestPawn->GetActorLocation());
			GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), NearestPawn->GetActorLocation());
			GetBlackboardComponent()->SetValueAsObject(TargetKey, NearestPawn);
		}
		else
		{
			GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
			GetBlackboardComponent()->SetValueAsObject(TargetKey, nullptr);
		}
	}

	CheckAndSendMovement();

	while (GameInstance->ClientSocketPtr->Q_zombie.try_pop(recvZombieData))
	{
		//UE_LOG(LogNet, Display, TEXT("try_pop Zombie: ZombieId=%d"), recvZombieData.ZombieId);
		// ���� GameMode �ν��Ͻ��� ���
		if (AOneGameModeBase* MyGameMode = Cast<AOneGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			// GameMode ���� �Լ� ȣ���Ͽ� �ٸ� �÷��̾��� ��ġ ������Ʈ
			MyGameMode->UpdateZombie(recvZombieData.ZombieId, recvZombieData.Location, recvZombieData.Rotation);
			//UE_LOG(LogNet, Display, TEXT("Update call Zombie: ZombieId=%d"), recvZombieData.ZombieId);
		}
	}
}

void AZombieAIController::CheckAndSendMovement()
{
	auto* ZombiePawn = Cast<ABaseZombie>(GetPawn());
	FVector CurrentLocation = ZombiePawn->GetActorLocation();
	FRotator CurrentRotation = ZombiePawn->GetActorRotation();
	ZombieId = ZombiePawn->GetZombieId();

	// ���� ��ġ�� ���� ��ġ �� (������ ����)
	//if (PreviousLocation != CurrentLocation || PreviousRotation != CurrentRotation){}

	// Protobuf�� ����Ͽ� TestPacket ����
	Protocol::Zombie packet;
	packet.set_zombieid(ZombieId);
	packet.set_packet_type(2); // ���ϴ� ���� ����
	packet.set_x(CurrentLocation.X);
	packet.set_y(CurrentLocation.Y);
	packet.set_z(CurrentLocation.Z);
	packet.set_pitch(CurrentRotation.Pitch);
	packet.set_yaw(CurrentRotation.Yaw);
	packet.set_roll(CurrentRotation.Roll);

	// ����ȭ
	std::string serializedData;
	packet.SerializeToString(&serializedData);

	// ����ȭ�� �����͸� ������ ����
	bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());
	//UE_LOG(LogNet, Display, TEXT("Send Zombie: ZombieId=%d"), ZombieId);
}


void AZombieAIController::SetStartLocationValue(FVector startlocation)
{
	GetBlackboardComponent()->SetValueAsVector(StartLocationKey, startlocation);
}

void AZombieAIController::SetPatrolLocationValue(FVector patrollocation)
{
	GetBlackboardComponent()->SetValueAsVector(PatrolLocationKey, patrollocation);
}



