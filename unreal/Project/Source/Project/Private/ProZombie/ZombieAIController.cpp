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
	//static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/BB_ZombieV.BB_ZombieV"));
	//if (BBObject.Succeeded()) {
	//	BlackBoardAsset = BBObject.Object;
	//}
	//
	//static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/BT_ZombieTree.BT_ZombieTree"));
	//if (BTObject.Succeeded()) {
	//	AIBehavior = BTObject.Object;
	//}
	//
	//UE_LOG(LogNet, Display, TEXT("ZombieAIController On"));

}

void AZombieAIController::BeginPlay()
{
	Super::BeginPlay();

	OwnerZombie = Cast<ANormalZombie>(GetPawn());

	//
	//if (AIBehavior != nullptr) {
	//	RunBehaviorTree(AIBehavior);
	//
	//	AActor* OwningPawn = GetPawn();
	//
	//}
	//
	//GameInstance = Cast<UProGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));



}

//void AZombieAIController::IsZombiePathUpdate()
//{
//	ANormalZombie* NormalZombie = Cast<ANormalZombie>(GetPawn());
//
//	auto paths = GameInstance->ClientSocketPtr->Q_path;
//
//	ZombiePath zompath;
//
//	while (paths.unsafe_size() != 0) {
//
//		paths.try_pop(zompath);
//
//		if (zompath.ZombieId == NormalZombie->GetZombieId()) {
//
//			if (zompath.IsZombiePathUpdate == true) {
//
//				if (m_bIsPathUpdate == false) {
//					ZPath = zompath.Path;
//					ZombiePathIndex = 0;
//					m_bIsPathUpdate = true;
//				}
//			}
//			else {
//				m_bIsPathUpdate = false;
//			}
//
//			break;
//		}
//	}
//
//}


void AZombieAIController::MoveTo(float deltasecond)
{
	//FVector zomlocation;
	//
	//if (OwnerZombie) {
	//	zomlocation = OwnerZombie->GetActorLocation();
	//}
	//
	//std::tuple<float,float,float> target = ZPath.back();
	//
	//
	//if (zomlocation.X == get<0>(target) && zomlocation.Y == get<1>(target)) {
	//	ZombiePathIndex = 0;
	//	return;
	//}
	//
	//
	//// 현재 목표 노드
	//std::tuple<float, float, float> TargetNode = ZPath[ZombiePathIndex];
	//float PathX = get<0>(TargetNode);
	//float PathY = get<1>(TargetNode);
	//
	//
	//// 타겟 방향 계산
	//float dx = PathX - zomlocation.X;
	//float dy = PathY - zomlocation.Y;
	//
	//// 거리를 계산
	//float distance = sqrt(dx * dx + dy * dy);
	//
	//// 이동 방향 벡터를 정규화
	//float directionX = dx / distance;
	//float directionY = dy / distance;
	//
	//// 이동 거리 계산
	//float moveDistance = OwnerZombie->GetSpeed() * 100.f * deltasecond;
	//
	//// 이동 벡터 계산
	//float moveX = directionX * moveDistance;
	//float moveY = directionY * moveDistance;
	//
	//
	//
	//// 목표에 도착했는지 확인 (옵션)
	//float newDistance = sqrt((PathX - zomlocation.X) * (PathX - zomlocation.X) + (PathY - zomlocation.Y) * (PathY - zomlocation.Y));
	//
	//if (newDistance < moveDistance) {
	//	zomlocation.X = PathX;
	//	zomlocation.Y = PathY;
	//
	//	// 다음 목표 노드로 이동
	//	ZombiePathIndex++;
	//	m_bIsTurn = true;
	//}
	//else {
	//	// 타겟 방향으로 이동
	//	zomlocation.X  += moveX;
	//	zomlocation.Y  += moveY;
	//
	//}
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

	Send_ZombieHP();


	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (PlayerPawn == nullptr || OwnerZombie == nullptr) {
		return;
	}

	FVector ZombieForward = OwnerZombie->GetActorForwardVector(); // 좀비의 전방 벡터
	FVector ZombieLocation = OwnerZombie->GetActorLocation(); // 좀비의 위치
	
	FVector PlayerLocation = PlayerPawn->GetActorLocation(); // 플레이어의 위치
	//FVector DirectionToPlayer = (PlayerLocation - ZombieLocation).GetSafeNormal(); // 플레이어로 향하는 방향 벡터
	FVector TargetLocation = PlayerLocation + (ZombieForward * 150.f);
	//float DotProduct = FVector::DotProduct(ZombieForward, DirectionToPlayer);
	float Distance = FVector::Dist(PlayerLocation, ZombieLocation);

	float MaxSightRange = 1000.f; // 원하는 최대 시야 범위를 설정하세요.


	//========================================================================== 시야각 설정 설정 필요!
	// 시야각을 90도로 설정 (전방 180도)
	//float FieldOfView = FMath::Cos(FMath::DegreesToRadians(90.0f / 2.0f)); // 전방 90도


	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), Players);

	APawn* NearestPawn = nullptr;
	float NearestDist = FLT_MAX;

	for (AActor* Player : Players)
	{
		//좀비들의 시야 검사 나 자신에 대해서만 실시==========================
		ABaseCharacter* Char = Cast<ABaseCharacter>(Player);
		//
		uint32 myPlayerId = GameInstance->ClientSocketPtr->GetMyPlayerId();
		//
		if (Char->GetPlayerId() != 99/*myPlayerId*/)
			continue;
		//====================================================================

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
				//IsZombiePathUpdate();
				//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Purple, FString::Printf(TEXT("Detected Player ID #%d"), Char->GetPlayerId()));
				//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Purple, FString::Printf(TEXT("My Player ID #%d"), myPlayerId));
			}
		}

		// NearestPawn에 따라 상태 변경
		if (NearestPawn) {
			if (!m_bPlayerInSight) {
				m_bPlayerInSight = true;
				ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(NearestPawn);
				Send_Detected(BaseCharacter); // 플레이어 감지 메시지 전송
				LastSeenPlayer = BaseCharacter;
			}
		}
		else {
			if (m_bPlayerInSight) {
				m_bPlayerInSight = false;
				Send_PlayerLost(LastSeenPlayer); // 플레이어를 놓쳤을 때 메시지 전송
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
	//if (NearestPawn)
	//{
	//	GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), TargetLocation);
	//	//GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), NearestPawn->GetActorLocation());
	//	GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), NearestPawn->GetActorLocation());
	//	GetBlackboardComponent()->SetValueAsObject(TargetKey, NearestPawn);
	//}
	//else
	//{
	//	GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
	//	GetBlackboardComponent()->SetValueAsObject(TargetKey, nullptr);
	//}
	//}

	//CheckAndSendMovement();

	//Walk(DeltaTime);


	// 계속 공격 애니메이션 재생되니 일단 주석처리
	//if (GameInstance->ClientSocketPtr->Q_zattack.try_pop(AttackZombieId))
	//{
	//	if (AOneGameModeBase* MyGameMode = Cast<AOneGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
	//	{
	//		MyGameMode->UpdateZombieAttack(AttackZombieId);
	//		UE_LOG(LogNet, Display, TEXT("Update Attack Zombie: ZombieId=%d"), AttackZombieId);
	//	}
	//}
	//
	//if (GameInstance->ClientSocketPtr->Q_zhp.try_pop(recvZombieHP))
	//{
	//	if (AOneGameModeBase* MyGameMode = Cast<AOneGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
	//	{
	//		MyGameMode->UpdateZombieHP(recvZombieHP.ZombieId, recvZombieHP.Hp);
	//		UE_LOG(LogNet, Display, TEXT("Update Zombie HP: ZombieId=%d"), recvZombieHP.ZombieId);
	//	}
	//}
}

void AZombieAIController::Send_Detected(ABaseCharacter* BaseCharacter)
{
	auto* ZombiePawn = Cast<ANormalZombie>(GetPawn());
	ZombieId = ZombiePawn->GetZombieId();
	uint32 PlayerId = BaseCharacter->GetPlayerId();

	Protocol::Detected packet;
	packet.set_zombieid(ZombieId);
	packet.set_playerid(PlayerId);
	packet.set_player_insight(true);
	packet.set_packet_type(9);

	std::string serializedData;
	packet.SerializeToString(&serializedData);

	bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());
}

void AZombieAIController::Send_PlayerLost(ABaseCharacter* BaseCharacter)
{
	auto* ZombiePawn = Cast<ANormalZombie>(GetPawn());
	ZombieId = ZombiePawn->GetZombieId();
	uint32 PlayerId = BaseCharacter->GetPlayerId();

	Protocol::Detected packet;
	packet.set_zombieid(ZombieId);
	packet.set_playerid(PlayerId);
	packet.set_player_insight(false);
	packet.set_packet_type(9);

	std::string serializedData;
	packet.SerializeToString(&serializedData);

	bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());
}

void AZombieAIController::Send_ZombieHP()
{
	auto* ZombiePawn = Cast<ANormalZombie>(GetPawn());
	if (PreviousHp != ZombiePawn->GetHP()) {
		ZombieId = ZombiePawn->GetZombieId();

		Protocol::Zombie_hp packet;
		packet.set_zombieid(ZombieId);
		packet.set_hp(ZombiePawn->GetHP());
		packet.set_packet_type(12);

		std::string serializedData;
		packet.SerializeToString(&serializedData);
		PreviousHp = ZombiePawn->GetHP();

		bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());
	}
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
	//auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	//if (nullptr != BehaviorTreeComponent)
	//{
	//	BehaviorTreeComponent->StartTree(*this->AIBehavior, EBTExecutionMode::Looped);
	//}
}

//void AZombieAIController::ZombieTurn(float delasecond)
//{
//	ANormalZombie* NormalZombie = Cast<ANormalZombie>(GetPawn());
//
//	FRotator zomCurRot = NormalZombie->GetActorRotation();
//
//	FVector zomDir = NormalZombie->GetActorLocation() - zDestination;
//
//	FVector zomNDir = zomDir.GetSafeNormal();
//	FRotator zomTarRot = zomNDir.Rotation();
//
//	// 회전 계산
//	float RotationSpeed = NormalZombie->GetTurningSpeed() * deltasecond;
//
//	FRotator NewRotation = FMath::RInterpTo(zomCurRot, zomTarRot, deltasecond, RotationSpeed);
//
//
//	NormalZombie->SetActorRotation(NewRotation);
//
//}

