// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemActor.h"
#include "NormalWeaponActor.generated.h"

/**
 * 
 */

// 무기 액터

UCLASS()
class PROJECT_API ANormalWeaponActor : public AItemActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANormalWeaponActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, Category= NormalWeapon)
	USkeletalMeshComponent* NormalWeapon;
};
