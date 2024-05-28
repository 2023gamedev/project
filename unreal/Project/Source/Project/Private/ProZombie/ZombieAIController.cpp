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
#include "ProZombie/NormalZombie.h"
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



void AZombieAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//static float SearchInterval = 0.5f; // 0.5초마다 플레이어 검색
	//static float TimeSinceLastSearch = 0.0f;
	//TimeSinceLastSearch += DeltaTime;

	if (GameInstance->ClientSocketPtr->GetMyPlayerId() == 1)
	{
		//TimeSinceLastSearch = 0.0f; // 타이머 리셋



		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

		ANormalZombie* NormalZombie = Cast<ANormalZombie>(GetPawn());

		if (PlayerPawn == nullptr || NormalZombie == nullptr) {
			return;
		}

		FVector ZombieForward = NormalZombie->GetActorForwardVector(); // 좀비의 전방 벡터
		FVector PlayerLocation = PlayerPawn->GetActorLocation(); // 플레이어의 위치
		FVector ZombieLocation = NormalZombie->GetActorLocation(); // 좀비의 위치

		FVector DirectionToPlayer = (PlayerLocation - ZombieLocation).GetSafeNormal(); // 플레이어로 향하는 방향 벡터

		float DotProduct = FVector::DotProduct(ZombieForward, DirectionToPlayer);
		float MaxSightRange = 1000.f; // 원하는 최대 시야 범위를 설정하세요.
		float Distance = FVector::Dist(PlayerLocation, ZombieLocation);

		// 시야각을 90도로 설정 (전방 180도)
		float FieldOfView = FMath::Cos(FMath::DegreesToRadians(90.0f / 2.0f)); // 전방 90도


		FVector TargetLocation = PlayerLocation + (ZombieForward * 100.f);


		TArray<AActor*> Players;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), Players);

		APawn* NearestPawn = nullptr;
		float NearestDist = FLT_MAX;

		for (AActor* Player : Players)
		{
			APawn* TestPawn = Cast<APawn>(Player);
			if (TestPawn && Distance <= MaxSightRange && DotProduct > FieldOfView && LineOfSightTo(TestPawn))
			{
				float Dist = FVector::Dist(GetPawn()->GetActorLocation(), TestPawn->GetActorLocation());
				if (Dist < NearestDist)
				{
					NearestDist = Dist;
					NearestPawn = TestPawn;
				}
			}
		}

		// 블랙보드 업데이트
		if (NearestPawn)
		{
			GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), TargetLocation);
			//GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), NearestPawn->GetActorLocation());
			GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), NearestPawn->GetActorLocation());
			GetBlackboardComponent()->SetValueAsObject(TargetKey, NearestPawn);
		}
		else
		{
			GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
			GetBlackboardComponent()->SetValueAsObject(TargetKey, nullptr);
		}
	}

	static const float Timer = 0.1f;
	static float CheckTime = 0.0f;

	CheckTime += DeltaTime;
	if(CheckTime >= Timer){
		CheckTime = 0.0f;
		CheckAndSendMovement();
	}

	while (GameInstance->ClientSocketPtr->Q_zombie.try_pop(recvZombieData))
	{
		//UE_LOG(LogNet, Display, TEXT("try_pop Zombie: ZombieId=%d"), recvZombieData.ZombieId);
		// 현재 GameMode 인스턴스를 얻기
		if (AOneGameModeBase* MyGameMode = Cast<AOneGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			// GameMode 내의 함수 호출하여 다른 플레이어의 위치 업데이트
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

	// 이전 위치와 현재 위치 비교 (움직임 감지)
	//if (PreviousLocation != CurrentLocation || PreviousRotation != CurrentRotation){}

	// Protobuf를 사용하여 TestPacket 생성
	Protocol::Zombie packet;
	packet.set_zombieid(ZombieId);
	packet.set_packet_type(2);
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

void AZombieAIController::UpdateLastKnownPositionByFootSound(FVector playerlocation)
{
	m_bFootSound = true;
	m_vFootCharacterPos = playerlocation;
}

void AZombieAIController::UpdateLastKnownPositionByShoutingSound(FVector playerlocation)
{
	m_bShoutingSound = true;
	m_vShoutingPos = playerlocation;
}

void AZombieAIController::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}

void AZombieAIController::StartAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StartTree(*this->AIBehavior, EBTExecutionMode::Looped);
	}
}

