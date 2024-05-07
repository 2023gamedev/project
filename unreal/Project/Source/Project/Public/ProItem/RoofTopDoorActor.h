// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProItem/InterActor.h"
#include "RoofTopDoorActor.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API ARoofTopDoorActor : public AInterActor
{
	GENERATED_BODY()
public:
	ARoofTopDoorActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;


	FTimerHandle DisableUnlockTimer;

	void StartDisableUnlock();
	void DisableUnlock();

	FTimerHandle ClearTimer;

	void UnlockKey1();
	void UnlockKey2();

	void UnlockDoor();

	void ClearAddScore();

	UPROPERTY(EditAnywhere)
	bool bIsDisableUnlock;


	UPROPERTY(EditAnywhere)
	bool bIsUnlockKey1;

	UPROPERTY(EditAnywhere)
	bool bIsUnlockKey2;
};
