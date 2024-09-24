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
}

void AZombieAIController::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<UProGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

}

void AZombieAIController::ZombieMoveTo(float deltasecond)
{
	FVector zomlocation;
	
	if (OwnerZombie) {
		zomlocation = OwnerZombie->GetActorLocation();
	}
	
	std::tuple<float,float,float> target = OwnerZombie->NextPath;
	
	// 현재 목표 노드
	float PathX = get<0>(target);
	float PathY = get<1>(target);

	//이미 도착지점에 도착했을때
	if (zomlocation.X == PathX && zomlocation.Y == PathY) {
		OwnerZombie->CachedAnimInstance->SetCurrentPawnSpeed(0);	//애니메이션 정지
		
		return;
	}
	
	
	// 타겟 방향 계산
	float dx = PathX - zomlocation.X;
	float dy = PathY - zomlocation.Y;
	
	// 거리를 계산
	float distance = sqrt(dx * dx + dy * dy);
	
	// 이동 방향 벡터를 정규화
	float directionX = dx / distance;
	float directionY = dy / distance;
	
	// 이동 거리 계산
	float moveDistance = OwnerZombie->GetSpeed() * 100.f * deltasecond;
	
	// 이동 벡터 계산
	float moveX = directionX * moveDistance;
	float moveY = directionY * moveDistance;
	

	// 목표에 도착했는지 확인
	float newDistance = sqrt((PathX - zomlocation.X) * (PathX - zomlocation.X) + (PathY - zomlocation.Y) * (PathY - zomlocation.Y));
	
	if (newDistance < moveDistance) {
		zomlocation.X = PathX;
		zomlocation.Y = PathY;
	}
	else {
		// 타겟 방향으로 이동
		zomlocation.X  += moveX;
		zomlocation.Y  += moveY;
	}


	OwnerZombie->SetActorLocation(zomlocation);
	OwnerZombie->CachedAnimInstance->SetCurrentPawnSpeed(OwnerZombie->GetSpeed());
}

void AZombieAIController::ZombieTurn(float deltasecond)
{
	FVector zombieDest;
	zombieDest.X = get<0>(OwnerZombie->NextPath);
	zombieDest.Y = get<1>(OwnerZombie->NextPath);
	zombieDest.Z = get<2>(OwnerZombie->NextPath);

	FVector zomTarDir = zombieDest - OwnerZombie->GetActorLocation();

	FVector zomTarNDir = zomTarDir.GetSafeNormal();
	FRotator zomTarRot = zomTarNDir.Rotation();
	FRotator zomCurRot = OwnerZombie->GetActorRotation();
	
	// 회전 계산
	float RotationSpeed = OwnerZombie->GetTurningSpeed() * deltasecond;

	FRotator NewRotation = FMath::RInterpTo(zomCurRot, zomTarRot, deltasecond, RotationSpeed);


	OwnerZombie->SetActorRotation(NewRotation);
}

void AZombieAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ZombieMoveTo(DeltaTime);
	ZombieTurn(DeltaTime);

	//static float SearchInterval = 0.5f; // 0.5초마다 플레이어 검색
	//static float TimeSinceLastSearch = 0.0f;
	//TimeSinceLastSearch += DeltaTime;

	//if (GameInstance->ClientSocketPtr->GetMyPlayerId() == 1)
	//{
		//TimeSinceLastSearch = 0.0f; // 타이머 리셋

	Send_ZombieHP();

	OwnerZombie = Cast<ANormalZombie>(GetPawn());

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (PlayerPawn == nullptr || OwnerZombie == nullptr) {
		return;
	}

	FVector ZombieForward = OwnerZombie->GetActorForwardVector(); // 좀비의 전방 벡터
	FVector ZombieLocation = OwnerZombie->GetActorLocation(); // 좀비의 위치

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
		if (Char->GetPlayerId() != 99)
			continue;
		//====================================================================

		PlayerPawn = Cast<APawn>(Player);

		FVector PlayerLocation = PlayerPawn->GetActorLocation(); // 플레이어의 위치
		//FVector DirectionToPlayer = (PlayerLocation - ZombieLocation).GetSafeNormal(); // 플레이어로 향하는 방향 벡터
		//FVector TargetLocation = PlayerLocation + (ZombieForward * 150.f);
		//float DotProduct = FVector::DotProduct(ZombieForward, DirectionToPlayer);
		float Distance = FVector::Dist(PlayerLocation, ZombieLocation);

		if (PlayerPawn && Distance <= MaxSightRange && LineOfSightTo(PlayerPawn))
		{
			float Dist = FVector::Dist(GetPawn()->GetActorLocation(), PlayerPawn->GetActorLocation());
			if (Dist < NearestDist)
			{
				NearestDist = Dist;
				NearestPawn = PlayerPawn;
				
				//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Detected Player ID #%d"), Char->GetPlayerId()));
				//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, FString::Printf(TEXT("My Player ID #%d"), myPlayerId));
				//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Purple, FString::Printf(TEXT("Detected Zombie ID #%d"), OwnerZombie->GetZombieId()));
				UE_LOG(LogNet, Display, TEXT("Detected Zombie ID #%d"), OwnerZombie->GetZombieId());
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

		//if (PlayerPawn && Distance <= MaxSightRange && DotProduct > FieldOfView && LineOfSightTo(PlayerPawn))
		//{
		//	float Dist = FVector::Dist(GetPawn()->GetActorLocation(), PlayerPawn->GetActorLocation());
		//	if (Dist < NearestDist)
		//	{
		//		NearestDist = Dist;
		//		NearestPawn = PlayerPawn;
		//	}
		//}
	}

	//CheckAndSendMovement();

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

void AZombieAIController::Send_Detected(ABaseCharacter* Player)
{
	auto* ZombiePawn = Cast<ANormalZombie>(GetPawn());
	ZombieId = ZombiePawn->GetZombieId();
	uint32 PlayerId = Player->GetPlayerId();

	Protocol::Detected packet;
	packet.set_zombieid(ZombieId);
	packet.set_playerid(PlayerId);
	packet.set_player_insight(true);
	packet.set_packet_type(9);

	std::string serializedData;
	packet.SerializeToString(&serializedData);

	bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());
}

void AZombieAIController::Send_PlayerLost(ABaseCharacter* Player)
{
	auto* ZombiePawn = Cast<ANormalZombie>(GetPawn());
	ZombieId = ZombiePawn->GetZombieId();
	uint32 PlayerId = Player->GetPlayerId();

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
	if (!OwnerZombie) {
		UE_LOG(LogTemp, Warning, TEXT("ZombiePawn is null."));
		return;
	}

	if (PreviousHp != OwnerZombie->GetHP()) {
		ZombieId = OwnerZombie->GetZombieId();

		Protocol::Zombie_hp packet;
		packet.set_zombieid(ZombieId);
		packet.set_hp(OwnerZombie->GetHP());
		packet.set_packet_type(12);

		std::string serializedData;
		packet.SerializeToString(&serializedData);
		PreviousHp = OwnerZombie->GetHP();

		bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());
	}
}

// 이전 클라에서 BT 돌릴때 쓰던 통신용 함수 같음 - 혹시 맞으면 더이상 안쓰니까 지우기
//void AZombieAIController::CheckAndSendMovement()
//{
//	auto* ZombiePawn = Cast<ANormalZombie>(GetPawn());
//	FVector CurrentLocation = ZombiePawn->GetActorLocation();
//	FRotator CurrentRotation = ZombiePawn->GetActorRotation();
//	ZombieId = ZombiePawn->GetZombieId();
//
//	// 이전 위치와 현재 위치 비교 (움직임 감지)
//	if (PreviousLocation != CurrentLocation || PreviousRotation != CurrentRotation) {
//
//		// Protobuf를 사용하여 TestPacket 생성
//		Protocol::Zombie packet;
//		packet.set_zombieid(ZombieId);
//		packet.set_packet_type(2);
//		packet.set_x(CurrentLocation.X);
//		packet.set_y(CurrentLocation.Y);
//		packet.set_z(CurrentLocation.Z);
//		packet.set_pitch(CurrentRotation.Pitch);
//		packet.set_yaw(CurrentRotation.Yaw);
//		packet.set_roll(CurrentRotation.Roll);
//
//		// 직렬화
//		std::string serializedData;
//		packet.SerializeToString(&serializedData);
//
//		// 직렬화된 데이터를 서버로 전송
//		bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());
//		//UE_LOG(LogNet, Display, TEXT("Send Zombie: ZombieId=%d"), ZombieId);
//
//		PreviousLocation = CurrentLocation;
//		PreviousRotation = CurrentRotation;
//	}
//}


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