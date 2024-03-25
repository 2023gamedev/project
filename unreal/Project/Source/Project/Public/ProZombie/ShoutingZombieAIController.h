// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
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

	void SetStartLocationValue(FVector startlocation);
	void SetPatrolLocationValue(FVector patrollocation);

private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* ShoutingZombieAIBehavior;

	UPROPERTY(EditAnywhere)

	class UBlackboardData* ShoutingZombieBlackBoardAsset;


};
