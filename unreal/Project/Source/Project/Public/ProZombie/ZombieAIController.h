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

	void CheckAndSendMovement();

	void SetStartLocationValue(FVector startlocation);
	void SetPatrolLocationValue(FVector patrollocation);
	

	void UpdateLastKnownPositionByFootSound(FVector playerlocation);

	void UpdateLastKnownPositionByShoutingSound(FVector playerlocation);

	void StopAI();
	void StartAI();

	UProGameInstance* GameInstance;

private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;

	UPROPERTY(EditAnywhere)
	class UBlackboardData* BlackBoardAsset;

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



	ZombieData recvZombieData;
	uint32 ZombieId;
	FVector NewLocation;
	FVector PreviousLocation;
	FRotator PreviousRotation;

	ABaseCharacter* LastSeenPlayer;


};
