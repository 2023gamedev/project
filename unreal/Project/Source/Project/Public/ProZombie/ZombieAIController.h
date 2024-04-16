// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ClientSocket.h"
#include "ZombieAIController.generated.h"

/**
 * 
 */
 // Zombie�� ��Ʈ���ϴ� ��Ʈ�ѷ��� �θ� Ŭ����
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

	void CheckAndSendMovement();

	void SetStartLocationValue(FVector startlocation);
	void SetPatrolLocationValue(FVector patrollocation);

	UProGameInstance* GameInstance;

private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;

	UPROPERTY(EditAnywhere)
	class UBlackboardData* BlackBoardAsset;

public:
	ZombieData recvZombieData;
	uint32 ZombieId;
	FVector NewLocation;
	FVector PreviousLocation;
	FRotator PreviousRotation;


};
