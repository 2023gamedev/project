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
}

void AShoutingZombieAIController::CheckAndSendMovement()
{
	auto* ZombiePawn = Cast<AShoutingZombie>(GetPawn());
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