// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"

#include "FireFighterCharacter.generated.h"

/**
 * 
 */
 // FireFighter 캐릭터 클래스
UCLASS()
class PROJECT_API AFireFighterCharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AFireFighterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	FTimerHandle SmokeHandle;
	FTimerHandle DyingHandle;
	void SmokeTimer();
	void LimitSmoking();
	void NoSmokeIsDying();

	virtual void Smoking(AHealingItemActor* smokeActor) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
