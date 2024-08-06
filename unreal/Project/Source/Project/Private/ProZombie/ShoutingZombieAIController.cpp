// Fill out your copyright notice in the Description page of Project Settings.


#include "ProZombie/ShoutingZombieAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GStruct.pb.h"
#include "ProZombie/ShoutingZombie.h"
#include "ProCharacter/BaseCharacter.h"
#include "ProGamemode/OneGameModeBase.h"
#include "ProGamemode/ProGameInstance.h"

// AI 추가되면 수정될 것 같은 것!!

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

	}

	GameInstance = Cast<UProGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

}

void AShoutingZombieAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	AShoutingZombie* ShoutingZombie = Cast<AShoutingZombie>(GetPawn());


	if (PlayerPawn == nullptr || ShoutingZombie == nullptr) {
		return;
	}

	FVector ZombieForward = ShoutingZombie->GetActorForwardVector(); // 좀비의 전방 벡터
	FVector PlayerLocation = PlayerPawn->GetActorLocation(); // 플레이어의 위치
	FVector ZombieLocation = ShoutingZombie->GetActorLocation(); // 좀비의 위치

	FVector DirectionToPlayer = (PlayerLocation - ZombieLocation).GetSafeNormal(); // 플레이어로 향하는 방향 벡터

	float DotProduct = FVector::DotProduct(ZombieForward, DirectionToPlayer);
	float MaxSightRange = 1000.f; // 원하는 최대 시야 범위를 설정하세요.
	float Distance = FVector::Dist(PlayerLocation, ZombieLocation);

	// 시야각을 90도로 설정 (전방 180도)
	float FieldOfView = FMath::Cos(FMath::DegreesToRadians(90.0f / 2.0f)); // 전방 90도


	FVector TargetLocation = PlayerLocation + (ZombieForward * 100.f);

	if (Distance <= MaxSightRange && LineOfSightTo(PlayerPawn)) {
		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), TargetLocation);
		//GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), PlayerPawn->GetActorLocation());
	 	GetBlackboardComponent()->SetValueAsObject(TargetKey, PlayerPawn);
	}
	else {
		GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
		GetBlackboardComponent()->SetValueAsObject(TargetKey, nullptr);
	}

	CheckAndSendMovement();
}

void AShoutingZombieAIController::CheckAndSendMovement()
{
	auto* ZombiePawn = Cast<AShoutingZombie>(GetPawn());
	FVector CurrentLocation = ZombiePawn->GetActorLocation();
	FRotator CurrentRotation = ZombiePawn->GetActorRotation();
	ZombieId = ZombiePawn->GetZombieId();

	// 이전 위치와 현재 위치 비교 (움직임 감지)
	//if (PreviousLocation != CurrentLocation || PreviousRotation != CurrentRotation){}

	// Protobuf를 사용하여 TestPacket 생성
	Protocol::Zombie packet;
	packet.set_zombieid(ZombieId);
	packet.set_packet_type(2); // 원하는 유형 설정
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

	// 현재 위치를 이전 위치로 업데이트
	PreviousLocation = CurrentLocation;
	PreviousRotation = CurrentRotation;

}

void AShoutingZombieAIController::SetStartLocationValue(FVector startlocation)
{
	GetBlackboardComponent()->SetValueAsVector(StartLocationKey, startlocation);
}

void AShoutingZombieAIController::SetPatrolLocationValue(FVector patrollocation)
{
	GetBlackboardComponent()->SetValueAsVector(PatrolLocationKey, patrollocation);
}

void AShoutingZombieAIController::UpdateLastKnownPositionByFootSound(FVector playerlocation)
{
	m_bFootSound = true;
	m_vFootCharacterPos = playerlocation;
}

void AShoutingZombieAIController::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}

void AShoutingZombieAIController::StartAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StartTree(*this->ShoutingZombieAIBehavior, EBTExecutionMode::Looped);
	}
}
