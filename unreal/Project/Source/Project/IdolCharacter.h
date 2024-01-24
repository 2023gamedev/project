// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "IdolCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API AIdolCharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AIdolCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
