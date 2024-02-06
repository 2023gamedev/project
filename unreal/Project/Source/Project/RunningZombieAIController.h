// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RunningZombieAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API ARunningZombieAIController : public AAIController
{
	GENERATED_BODY()
public:
	ARunningZombieAIController();

	static const FName TargetKey;
	static const FName StartLocationKey;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	//virtual void OnPossess(APawn* aPawn) override;


private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* RunningZombieAIBehavior;

	UPROPERTY(EditAnywhere)

	class UBlackboardData* RunningZombieBlackBoardAsset;
	


};
