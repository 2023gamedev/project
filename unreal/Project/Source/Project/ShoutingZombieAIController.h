// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShoutingZombieAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API AShoutingZombieAIController : public AAIController
{
	GENERATED_BODY()
public:
	AShoutingZombieAIController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;




private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* ShoutingZombieAIBehavior;

	UPROPERTY(EditAnywhere)

	class UBlackboardData* ShoutingZombieBlackBoardAsset;

};
