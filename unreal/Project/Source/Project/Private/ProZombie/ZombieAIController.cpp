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

// AI 추가되면 수정될 것 같은 것!!

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

	UE_LOG(LogNet, Display, TEXT("ZombieAIController On"));

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

	//if (GameInstance->ClientSocketPtr->GetMyPlayerId() == 1)
	//{
		//TimeSinceLastSearch = 0.0f; // 타이머 리셋



		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

		ANormalZombie* NormalZombie = Cast<ANormalZombie>(GetPawn());

		if (PlayerPawn == nullptr || NormalZombie == nullptr) {
			return;
		}

		FVector ZombieForward = NormalZombie->GetActorForwardVector(); // 좀비의 전방 벡터
		FVector ZombieLocation = NormalZombie->GetActorLocation(); // 좀비의 위치

		FVector PlayerLocation = PlayerPawn->GetActorLocation(); // 플레이어의 위치
		//FVector DirectionToPlayer = (PlayerLocation - ZombieLocation).GetSafeNormal(); // 플레이어로 향하는 방향 벡터
		FVector TargetLocation = PlayerLocation + (ZombieForward * 150.f);
		//float DotProduct = FVector::DotProduct(ZombieForward, DirectionToPlayer);
		float Distance = FVector::Dist(PlayerLocation, ZombieLocation);

		float MaxSightRange = 1000.f; // 원하는 최대 시야 범위를 설정하세요.


		// 시야각을 90도로 설정 (전방 180도)
		//float FieldOfView = FMath::Cos(FMath::DegreesToRadians(90.0f / 2.0f)); // 전방 90도




		TArray<AActor*> Players;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), Players);

		APawn* NearestPawn = nullptr;
		float NearestDist = FLT_MAX;

		for (AActor* Player : Players)
		{
			APawn* TestPawn = Cast<APawn>(Player);

			PlayerLocation = TestPawn->GetActorLocation(); // 플레이어의 위치
			// DirectionToPlayer = (PlayerLocation - ZombieLocation).GetSafeNormal(); // 플레이어로 향하는 방향 벡터
			TargetLocation = PlayerLocation + (ZombieForward * 150.f);
			// DotProduct = FVector::DotProduct(ZombieForward, DirectionToPlayer);
			Distance = FVector::Dist(PlayerLocation, ZombieLocation);

			if (TestPawn && Distance <= MaxSightRange && LineOfSightTo(TestPawn))
			{
				float Dist = FVector::Dist(GetPawn()->GetActorLocation(), TestPawn->GetActorLocation());
				if (Dist < NearestDist)
				{
					NearestDist = Dist;
					NearestPawn = TestPawn;

			
					// 바로 아래에 '블랙보드 업데이트' 안에 잘못 불리고 있었던거 옮김
					// 생각해보니 보고있는(시야에 들어오는) 상황을 실시간으로 체크해야 했음
					//=>	보고 있는지 아닌지를 틱마다 보내는 건 아닌 것 같고, 변수를 하나 여기(언리얼)이랑 서버에 만들어서
					//		일단 여기서 시야각에 들어오면 해당 변수를 true로 만들고 시야각 들어옴을 서버에 알리고 (현재 만들어 놓은 상태 -> 하지만 한번만 보내자[수정함])
					//		시야를 벗어나 true인 상태를 벗어나면, 그때 벗어났다고 서버에 다시 알려줘야 할 것 같음 (만들어야 함)

					// 감지한 좀비와 플레이어 아이디 전송
					if (m_bPlayerInSight == false) {
						m_bPlayerInSight = true;
						ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(NearestPawn);
						Send_Detected(BaseCharacter);
					}
				}
			}

			//if (TestPawn && Distance <= MaxSightRange && DotProduct > FieldOfView && LineOfSightTo(TestPawn))
			//{
			//	float Dist = FVector::Dist(GetPawn()->GetActorLocation(), TestPawn->GetActorLocation());
			//	if (Dist < NearestDist)
			//	{
			//		NearestDist = Dist;
			//		NearestPawn = TestPawn;
			//	}
			//}
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
	//}

	//CheckAndSendMovement();
}

void AZombieAIController::Send_Detected(ABaseCharacter* BaseCharacter)
{
	auto* ZombiePawn = Cast<ANormalZombie>(GetPawn());
	ZombieId = ZombiePawn->GetZombieId();
	uint32 PlayerId = BaseCharacter->GetPlayerId();

	Protocol::Detected packet;
	packet.set_zombieid(ZombieId);
	packet.set_playerid(PlayerId);
	packet.set_packet_type(9);

	std::string serializedData;
	packet.SerializeToString(&serializedData);

	bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());
}

void AZombieAIController::CheckAndSendMovement()
{
	auto* ZombiePawn = Cast<ANormalZombie>(GetPawn());
	FVector CurrentLocation = ZombiePawn->GetActorLocation();
	FRotator CurrentRotation = ZombiePawn->GetActorRotation();
	ZombieId = ZombiePawn->GetZombieId();

	// 이전 위치와 현재 위치 비교 (움직임 감지)
	if (PreviousLocation != CurrentLocation || PreviousRotation != CurrentRotation) {

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

		PreviousLocation = CurrentLocation;
		PreviousRotation = CurrentRotation;
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

