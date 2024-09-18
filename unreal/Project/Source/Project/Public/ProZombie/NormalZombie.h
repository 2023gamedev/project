// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseZombie.h"
#include "NormalZombie.generated.h"

/**
 * 
 */


 // NormalZombie(평범한 좀비) 클래스
UCLASS()
class PROJECT_API ANormalZombie : public ABaseZombie
{
	GENERATED_BODY()

public:
	ANormalZombie();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;

public:
	UZombieAnimInstance* CachedAnimInstance;
};
