// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseZombieCharacter.h"
#include "NormalZombieCharacter.generated.h"

/**
 * 
 */
UCLASS()
class REALPROJECT_API ANormalZombieCharacter : public ABaseZombieCharacter
{
	GENERATED_BODY()

	
public:
	ANormalZombieCharacter();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
