// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseZombie.h"
#include "RunningZombie.generated.h"

/**
 * 
 */
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

	//bool SetIsShouting(bool isshouting) { m_bIsShouting = isshouting; }
	//bool GetIsShouting() { return m_bIsShouting; }

	//UPROPERTY(EditAnywhere)
	//bool m_bIsShouting = false;
};
