// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseZombie.h"
#include "RunningZombie.generated.h"

/**
 * 
 */
 // RunningZombie(달리는 좀비) 클래스
UCLASS()
class PROJECT_API ARunningZombie : public ABaseZombie
{
	GENERATED_BODY()
public:
	ARunningZombie();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;

};
