// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ClientSocket.h"
#include "ZombieAIController.generated.h"



/**
 * 
 */
 // Zombie를 컨트롤하는 컨트롤러의 부모 클래스
// AI 추가되면 수정될 것 같은 것!!
UCLASS()
class PROJECT_API AZombieAIController : public AAIController
{
	GENERATED_BODY()
public:
	AZombieAIController();

	static const FName TargetKey;
	static const FName StartLocationKey;
	static const FName PatrolLocationKey;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void Send_Detected(ABaseCharacter* BaseCharacter);

	void Send_PlayerLost(ABaseCharacter* LastSeenPlayer);

	void Send_ZombieHP();

	void CheckAndSendMovement();

	void SetStartLocationValue(FVector startlocation);
	void SetPatrolLocationValue(FVector patrollocation);
	

	void UpdateLastKnownPositionByFootSound(FVector playerlocation);

	void UpdateLastKnownPositionByShoutingSound(FVector playerlocation);

	void StopAI();
	void StartAI();

	//bool IsZombiePathUpdate();

	//void ZombieTurn(float delasecond);
	
	void MoveTo(float deltasecond);

	UProGameInstance* GameInstance;

private:
	//UPROPERTY(EditAnywhere)
	//class UBehaviorTree* AIBehavior;
	//
	//UPROPERTY(EditAnywhere)
	//class UBlackboardData* BlackBoardAsset;

public:
	UPROPERTY(EditAnywhere)
	bool m_bFootSound;

	UPROPERTY(EditAnywhere)
	FVector m_vFootCharacterPos;

	UPROPERTY(EditAnywhere)
	FVector m_vShoutingPos;

	UPROPERTY(EditAnywhere)
	bool m_bShoutingSound;

	UPROPERTY(EditAnywhere)
	bool m_bPlayerInSight;


	int ZombiePathIndex;

	// path가 업데이트 되면 이것을 true로 바꿔주기
	//bool m_bIsPathUpdate;

	// path 도착하고 turn 할지 말지 bool 값
	//bool m_bIsTurn;

	//std::vector<std::tuple<float, float, float>> before_ZPath = {};

	//std::vector<std::tuple<float, float, float>> ZPath = {};

	ZombieData recvZombieData;
	ZombiePath recvZombiePath;
	ZombieHP recvZombieHP;

	uint32 ZombieId;
	uint32 AttackZombieId;
	FVector NewLocation;
	FVector PreviousLocation;
	FRotator PreviousRotation;

	ABaseCharacter* LastSeenPlayer;

	ANormalZombie* OwnerZombie;

	float PreviousHp;
};
