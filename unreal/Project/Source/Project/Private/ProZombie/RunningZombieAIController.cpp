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

// AI �߰��Ǹ� ������ �� ���� ��!!

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

}

void ARunningZombieAIController::BeginPlay()
{
	Super::BeginPlay();

	if (RunningZombieAIBehavior != nullptr) {
		RunBehaviorTree(RunningZombieAIBehavior);
	}

	GameInstance = Cast<UProGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	

}

void ARunningZombieAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	ARunningZombie* RunningZombie = Cast<ARunningZombie>(GetPawn());

	if (PlayerPawn == nullptr || RunningZombie == nullptr) {
		return;
	}

	FVector ZombieForward = RunningZombie->GetActorForwardVector(); // ������ ���� ����
	FVector PlayerLocation = PlayerPawn->GetActorLocation(); // �÷��̾��� ��ġ
	FVector ZombieLocation = RunningZombie->GetActorLocation(); // ������ ��ġ

	FVector DirectionToPlayer = (PlayerLocation - ZombieLocation).GetSafeNormal(); // �÷��̾�� ���ϴ� ���� ����

	float DotProduct = FVector::DotProduct(ZombieForward, DirectionToPlayer);
	float MaxSightRange = 1000.f; // ���ϴ� �ִ� �þ� ������ �����ϼ���.
	float Distance = FVector::Dist(PlayerLocation, ZombieLocation);

	// �þ߰��� 90���� ���� (���� 180��)
	float FieldOfView = FMath::Cos(FMath::DegreesToRadians(90.0f / 2.0f)); // ���� 90��


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

void ARunningZombieAIController::CheckAndSendMovement()
{
	auto* ZombiePawn = Cast<ARunningZombie>(GetPawn());
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

	// ���� ��ġ�� ���� ��ġ�� ������Ʈ
	PreviousLocation = CurrentLocation;
	PreviousRotation = CurrentRotation;

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

void ARunningZombieAIController::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}

void ARunningZombieAIController::StartAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StartTree(*this->RunningZombieAIBehavior, EBTExecutionMode::Looped);
	}
}
