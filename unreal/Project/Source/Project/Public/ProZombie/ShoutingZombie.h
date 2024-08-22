// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseZombie.h"
#include "ShoutingZombie.generated.h"

/**
 * 
 */


 // ShoutingZombie(소리치는 좀비) 클래스
UCLASS()
class PROJECT_API AShoutingZombie : public ABaseZombie
{
	GENERATED_BODY()
public:
	AShoutingZombie();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;
};
