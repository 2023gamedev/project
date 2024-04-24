// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemActor.h"
#include "BleedingHealingItemActor.generated.h"

/**
 * 
 */

class ABaseCharacter;
UCLASS()
class PROJECT_API ABleedingHealingItemActor : public AItemActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	ABleedingHealingItemActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, Category = BleedingHealingItem)
	UStaticMeshComponent* BleedingHealingItem;

	// 무기의 스탯들
	UPROPERTY(EditAnywhere, Category = "ItemStat")
	float m_fHealingSuccessProbability;

	UPROPERTY(EditAnywhere)
	ABaseCharacter* OwnerCharacter;

	UPROPERTY(EditAnywhere)
	FName BHName;
};
