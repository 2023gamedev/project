// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZombieAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API AZombieAIController : public AAIController
{
	GENERATED_BODY()
public:
	AZombieAIController();

	static const FName TargetKey;
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;




private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;

	UPROPERTY(EditAnywhere)

	class UBlackboardData* BlackBoardAsset;

};
