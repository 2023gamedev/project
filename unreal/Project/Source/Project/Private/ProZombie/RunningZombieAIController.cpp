// Fill out your copyright notice in the Description page of Project Settings.


#include "ProZombie/RunningZombieAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GStruct.pb.h"
#include "ProZombie/RunningZombie.h"
#include "ProCharacter/BaseCharacter.h"
#include "ProGamemode/OneGameModeBase.h"
#include "ProGamemode/ProGameInstance.h"


// AI 추가되면 수정될 것 같은 것!!

const FName ARunningZombieAIController::TargetKey(TEXT("Target"));
const FName ARunningZombieAIController::StartLocationKey(TEXT("StartLocation"));
const FName ARunningZombieAIController::PatrolLocationKey(TEXT("PatrolLocation"));

ARunningZombieAIController::ARunningZombieAIController()
{
	OwnerZombie = nullptr;

	attackPlayerID = 0;

	//static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/BB_RZombieV.BB_RZombieV"));
	//if (BBObject.Succeeded()) {
	//	RunningZombieBlackBoardAsset = BBObject.Object;
	//}

	//static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/BT_RZombieTree.BT_RZombieTree"));
	//if (BTObject.Succeeded()) {
	//	RunningZombieAIBehavior = BTObject.Object;
	//}

}

void ARunningZombieAIController::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<UProGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

void ARunningZombieAIController::ZombieMoveTo(float deltasecond, int& indx)
{
	FVector zomlocation;

	if (OwnerZombie) {
		zomlocation = OwnerZombie->GetActorLocation();
	}

	std::tuple<float, float, float> target = OwnerZombie->NextPath[indx];

	// 현재 목표 노드
	float PathX = get<0>(target);
	float PathY = get<1>(target);

	if (PathX != 0.f || PathY != 0.f) { // 좀비 이동경로 확인용 debugline
		FVector Pos;
		Pos.X = get<0>(target);
		Pos.Y = get<1>(target);
		Pos.Z = get<2>(target);
		FVector Start = Pos + FVector(0, 0, 100);
		FVector End = Pos - FVector(0, 0, 100);
		FCollisionQueryParams Params;

		DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 3.f);
	}

	//이미 도착지점에 도착했을때
	if (zomlocation.X == PathX && zomlocation.Y == PathY) {
		if (deltasecond > 0.3) {	// 만약 좀비가 제자리에 0.3초 이상 있을 시에
			OwnerZombie->CachedAnimInstance->SetCurrentPawnSpeed(0);	//애니메이션 정지
		}
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

		// 다음 목표 노드로 이동
		indx++;

		// 경로의 끝에 도착 = 최종 목표지점에 도착
		if (get<0>(OwnerZombie->NextPath[indx]) == 0 && get<1>(OwnerZombie->NextPath[indx]) == 0 && get<2>(OwnerZombie->NextPath[indx]) == 0) {
			indx--;
		}
		else {	// 꼭지점을 넘어 갈 때
			float mid_deltasecond = newDistance / OwnerZombie->GetSpeed() * 100.f;
			float after_deltasecond = deltasecond - mid_deltasecond;

			ZombieMoveTo(after_deltasecond, indx);
		}
	}
	else {
		// 타겟 방향으로 이동
		zomlocation.X += moveX;
		zomlocation.Y += moveY;
	}


	OwnerZombie->SetActorLocation(zomlocation);
	OwnerZombie->CachedAnimInstance->SetCurrentPawnSpeed(OwnerZombie->GetSpeed());
}

void ARunningZombieAIController::ZombieTurn(float deltasecond, int& indx)
{
	FVector zombieDest;

	// 좀비 사망시 => 그대로 고개 멈춤
	if (OwnerZombie->GetHP() < 0) { return; }

	// 좀비가 피격 중일때 => 그대로 고개 멈춤
	else if (OwnerZombie->CachedAnimInstance->Montage_IsPlaying(OwnerZombie->CachedAnimInstance->BeAttackedMontage) == true) { return; }

	// 좀비가 공격 중일때 => 플레이어 쪽으로 시선 돌리기
	else if (OwnerZombie->CachedAnimInstance->Montage_IsPlaying(OwnerZombie->CachedAnimInstance->AttackMontage) == true) {
		TArray<AActor*> Players;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), Players);
		ABaseCharacter* Char = nullptr;

		// 플레이어 찾기
		bool result = false;
		for (AActor* Player : Players)
		{
			Char = Cast<ABaseCharacter>(Player);

			// 플레이어 자기 자신일 때는 Char->GetPlayerId() 에 99가 담기므로
			if (Char->GetPlayerId() == 99) {
				if (GameInstance->ClientSocketPtr->MyPlayerId == attackPlayerID) {
					result = true;
					break;
				}
			}
			else if (Char->GetPlayerId() == attackPlayerID) {
				result = true;
				break;
			}
		}

		if (result == false) {
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("[ERROR] ZombieTurn(ToPlayer) - Couldn't find Player ID #%d"), attackPlayerID));
			return;
		}

		// 해당 플레이어 쪽으로 회전시키기
		zombieDest.X = Char->GetActorLocation().X;
		zombieDest.Y = Char->GetActorLocation().Y;
		zombieDest.Z = Char->GetActorLocation().Z;
	}
	// 다 아니면 이동 중이므로
	else {
		// 다음 행선지 쪽으로 회전시키기
		zombieDest.X = get<0>(OwnerZombie->NextPath[indx]);
		zombieDest.Y = get<1>(OwnerZombie->NextPath[indx]);
		zombieDest.Z = get<2>(OwnerZombie->NextPath[indx]);
	}

	FVector zomTarDir = zombieDest - OwnerZombie->GetActorLocation();

	FVector zomTarNDir = zomTarDir.GetSafeNormal();
	FRotator zomTarRot = zomTarNDir.Rotation();
	FRotator zomCurRot = OwnerZombie->GetActorRotation();

	// 회전 계산
	float RotationSpeed = OwnerZombie->GetTurningSpeed() * deltasecond;

	FRotator NewRotation = FMath::RInterpTo(zomCurRot, zomTarRot, deltasecond, RotationSpeed);


	OwnerZombie->SetActorRotation(NewRotation);
}

void ARunningZombieAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	ARunningZombie* RunningZombie = Cast<ARunningZombie>(GetPawn());

	if (PlayerPawn == nullptr || OwnerZombie == nullptr) {
		return;
	}

	Send_ZombieHP();

	// 좀비 사망시
	if (OwnerZombie->GetHP() < 0) {
		return;
	}

	int indx = 0;
	ZombieMoveTo(DeltaTime, indx);
	ZombieTurn(DeltaTime, indx);


	FVector ZombieForward = OwnerZombie->GetActorForwardVector(); // 좀비의 전방 벡터
	FVector ZombieLocation = OwnerZombie->GetActorLocation(); // 좀비의 위치

	float MaxSightRange = 1000.f; // 원하는 최대 시야 범위를 설정하세요.


	//========================================================================== 시야각 설정 설정 필요!
	// 좀비 시야각 (전방 120도)
	float FieldOfView = FMath::Cos(FMath::DegreesToRadians(120.0f / 2.0f));


	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacter::StaticClass(), Players);

	APawn* NearestPawn = nullptr;
	float NearestDist = FLT_MAX;

	for (AActor* Player : Players)
	{
		//좀비들의 시야 검사 "나 자신"에 대해서만 실시==========================
		ABaseCharacter* Char = Cast<ABaseCharacter>(Player);
		//
		uint32 myPlayerId = GameInstance->ClientSocketPtr->GetMyPlayerId();
		//
		if (Char->GetPlayerId() != 99)
			continue;
		//====================================================================

		PlayerPawn = Cast<APawn>(Player);

		FVector PlayerLocation = PlayerPawn->GetActorLocation(); // 플레이어의 위치
		FVector DirectionToPlayer = (PlayerLocation - ZombieLocation).GetSafeNormal(); // 플레이어로 향하는 방향 벡터

		float DotProduct = FVector::DotProduct(ZombieForward, DirectionToPlayer);

		float Distance = FVector::Dist(PlayerLocation, ZombieLocation);
		bool InZombieSight = FieldOfView <= DotProduct ? true : false;

		if (PlayerPawn && Distance <= MaxSightRange && LineOfSightTo(PlayerPawn) && InZombieSight)
		{
			float Dist = FVector::Dist(GetPawn()->GetActorLocation(), PlayerPawn->GetActorLocation());
			if (Dist < NearestDist)
			{
				NearestDist = Dist;
				NearestPawn = PlayerPawn;

				//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Detected Player ID #%d"), Char->GetPlayerId()));
				//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, FString::Printf(TEXT("My Player ID #%d"), myPlayerId));
				//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Purple, FString::Printf(TEXT("Detected Zombie ID #%d"), OwnerZombie->GetZombieId()));
				//UE_LOG(LogNet, Display, TEXT("Detected Zombie ID #%d"), OwnerZombie->GetZombieId());
			}
		}

		// NearestPawn에 따라 상태 변경
		if (NearestPawn) {	// NearestPawn 존재 O -> 나를 포착함
			if (m_bPlayerInSight == false) {
				m_bPlayerInSight = true;
				ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(NearestPawn);
				Send_Detected(); // 플레이어 감지 메시지 전송
				LastSeenPlayer = BaseCharacter;
				UE_LOG(LogNet, Display, TEXT("Zombie #%d Detected Player #%d"), OwnerZombie->GetZombieId(), myPlayerId);
			}
		}
		else {	// NearestPawn 존재 X -> 나를 못 봄
			if (m_bPlayerInSight == true) {
				m_bPlayerInSight = false;
				Send_PlayerLost(); // 플레이어를 놓쳤을 때 메시지 전송
				UE_LOG(LogNet, Display, TEXT("Zombie #%d Lost Player #%d"), OwnerZombie->GetZombieId(), myPlayerId);
			}
		}
	}

}

void ARunningZombieAIController::Send_Detected()
{
	auto* ZombiePawn = Cast<ARunningZombie>(GetPawn());
	ZombieId = ZombiePawn->GetZombieId();
	uint32 PlayerId = GameInstance->ClientSocketPtr->MyPlayerId;

	Protocol::Detected packet;
	packet.set_zombieid(ZombieId);
	packet.set_playerid(PlayerId);
	packet.set_player_insight(true);
	packet.set_packet_type(9);

	std::string serializedData;
	packet.SerializeToString(&serializedData);

	bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());
}

void ARunningZombieAIController::Send_PlayerLost()
{
	auto* ZombiePawn = Cast<ARunningZombie>(GetPawn());
	ZombieId = ZombiePawn->GetZombieId();
	uint32 PlayerId = GameInstance->ClientSocketPtr->MyPlayerId;

	Protocol::Detected packet;
	packet.set_zombieid(ZombieId);
	packet.set_playerid(PlayerId);
	packet.set_player_insight(false);
	packet.set_packet_type(9);

	std::string serializedData;
	packet.SerializeToString(&serializedData);

	bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());
}

void ARunningZombieAIController::Send_ZombieHP()
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


void ARunningZombieAIController::SetStartLocationValue(FVector startlocation)
{
	GetBlackboardComponent()->SetValueAsVector(StartLocationKey, startlocation);
}

void ARunningZombieAIController::SetPatrolLocationValue(FVector patrollocation)
{
	GetBlackboardComponent()->SetValueAsVector(PatrolLocationKey, patrollocation);
}

void ARunningZombieAIController::UpdateLastKnownPositionByFootSound(FVector playerlocation)
{
	m_bFootSound = true;
	m_vFootCharacterPos = playerlocation;
}
void ARunningZombieAIController::UpdateLastKnownPositionByShoutingSound(FVector playerlocation)
{
	m_bShoutingSound = true;
	m_vShoutingPos = playerlocation;
}


//
//void ARunningZombieAIController::CheckAndSendMovement()
//{
//	auto* ZombiePawn = Cast<ARunningZombie>(GetPawn());
//	FVector CurrentLocation = ZombiePawn->GetActorLocation();
//	FRotator CurrentRotation = ZombiePawn->GetActorRotation();
//	ZombieId = ZombiePawn->GetZombieId();
//
//	// 이전 위치와 현재 위치 비교 (움직임 감지)
//	//if (PreviousLocation != CurrentLocation || PreviousRotation != CurrentRotation){}
//
//	// Protobuf를 사용하여 TestPacket 생성
//	Protocol::Zombie packet;
//	packet.set_zombieid(ZombieId);
//	packet.set_packet_type(2); // 원하는 유형 설정
//	packet.set_x(CurrentLocation.X);
//	packet.set_y(CurrentLocation.Y);
//	packet.set_z(CurrentLocation.Z);
//	packet.set_pitch(CurrentRotation.Pitch);
//	packet.set_yaw(CurrentRotation.Yaw);
//	packet.set_roll(CurrentRotation.Roll);
//
//	// 직렬화
//	std::string serializedData;
//	packet.SerializeToString(&serializedData);
//
//	// 직렬화된 데이터를 서버로 전송
//	bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());
//
//	// 현재 위치를 이전 위치로 업데이트
//	PreviousLocation = CurrentLocation;
//	PreviousRotation = CurrentRotation;
//
//}

//
//void ARunningZombieAIController::StopAI()
//{
//	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
//	if (nullptr != BehaviorTreeComponent)
//	{
//		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
//	}
//}
//
//void ARunningZombieAIController::StartAI()
//{
//	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
//	if (nullptr != BehaviorTreeComponent)
//	{
//		BehaviorTreeComponent->StartTree(*this->RunningZombieAIBehavior, EBTExecutionMode::Looped);
//	}
//}
