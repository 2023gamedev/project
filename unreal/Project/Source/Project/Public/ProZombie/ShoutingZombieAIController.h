// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ClientSocket.h"
#include "ShoutingZombieAIController.generated.h"

/**
 * 
 */
 // ShoutingZombie(�Ҹ�ġ�� ����)�� ��Ʈ���ϴ� ��Ʈ�ѷ�
UCLASS()
class PROJECT_API AShoutingZombieAIController : public AAIController
{
	GENERATED_BODY()
public:
	AShoutingZombieAIController();

	static const FName TargetKey;
	static const FName StartLocationKey;
	static const FName PatrolLocationKey;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void CheckAndSendMovement();

	void SetStartLocationValue(FVector startlocation);
	void SetPatrolLocationValue(FVector patrollocation);

	UProGameInstance* GameInstance;

private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* ShoutingZombieAIBehavior;

	UPROPERTY(EditAnywhere)

	class UBlackboardData* ShoutingZombieBlackBoardAsset;

public:
	ZombieData recvZombieData;
	uint32 ZombieId;
	FVector NewLocation;
	FVector PreviousLocation;
	FRotator PreviousRotation;


};
