// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemActor.h"
#include "ThrowWeaponActor.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API AThrowWeaponActor : public AItemActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AThrowWeaponActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, Category = ThrowWeaponActor)
	USkeletalMeshComponent* ThrowWeaponActor;

	UPROPERTY(EditAnywhere)
	FName TWName;
};
