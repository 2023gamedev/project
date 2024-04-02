// Fill out your copyright notice in the Description page of Project Settings.


#include "ProZombie/RunningZombieAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GStruct.pb.h"
#include "ProGamemode/OneGameModeBase.h"
#include "ProGamemode/ProGameInstance.h"

const FName ARunningZombieAIController::TargetKey(TEXT("Target"));
const FName ARunningZombieAIController::StartLocationKey(TEXT("StartLocation"));
const FName ARunningZombieAIController::PatrolLocationKey(TEXT("PatrolLocation"));

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

	ClientSocketPtr = nullptr;

}

void ARunningZombieAIController::BeginPlay()
{
	Super::BeginPlay();

	if (RunningZombieAIBehavior != nullptr) {
		RunBehaviorTree(RunningZombieAIBehavior);

	}

	auto GameInstance = Cast<UProGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		this->ClientSocketPtr = GameInstance->ClientSocketPtr;
	}

}

void ARunningZombieAIController::Tick(float DeltaTime)
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

	CheckAndSendMovement();

	if (ClientSocketPtr->Q_zombie.try_pop(recvZombieData))
	{
		UE_LOG(LogNet, Display, TEXT("Update Zombie: ZombieId=%d"), recvZombieData.ZombieId);
		// 현재 GameMode 인스턴스를 얻기
		if (AOneGameModeBase* MyGameMode = Cast<AOneGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			// GameMode 내의 함수 호출하여 다른 플레이어의 위치 업데이트
			MyGameMode->UpdateOtherPlayer(recvZombieData.ZombieId, recvZombieData.Location, recvZombieData.Rotation);
		}
	}
}

void ARunningZombieAIController::CheckAndSendMovement()
{
	APawn* ZombiePawn = GetPawn();
	FVector CurrentLocation = ZombiePawn->GetActorLocation();
	FRotator CurrentRotation = ZombiePawn->GetActorRotation();


	// 이전 위치와 현재 위치 비교 (움직임 감지)
	if (PreviousLocation != CurrentLocation || PreviousRotation != CurrentRotation)
	{
		// Protobuf를 사용하여 TestPacket 생성
		Protocol::Zombie packet;
		packet.set_zombieid(ZombieId);
		packet.set_type(2); // 원하는 유형 설정
		packet.set_x(CurrentLocation.X);
		packet.set_y(CurrentLocation.Y);
		packet.set_z(CurrentLocation.Z);
		packet.set_pitch(CurrentRotation.Pitch);
		packet.set_yaw(CurrentRotation.Yaw);
		packet.set_roll(CurrentRotation.Roll);

		// 직렬화
		std::string serializedData;
		packet.SerializeToString(&serializedData);

		// 직렬화된 데이터를 서버로 전송
		bool bIsSent = ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());

		// 현재 위치를 이전 위치로 업데이트
		PreviousLocation = CurrentLocation;
		PreviousRotation = CurrentRotation;
	}

}

void ARunningZombieAIController::SetStartLocationValue(FVector startlocation)
{
	GetBlackboardComponent()->SetValueAsVector(StartLocationKey, startlocation);
}

void ARunningZombieAIController::SetPatrolLocationValue(FVector patrollocation)
{
	GetBlackboardComponent()->SetValueAsVector(PatrolLocationKey, patrollocation);
}

