// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemActor.h"
#include "HealingItemActor.generated.h"

/**
 * 
 */
class ABaseCharacter;

UCLASS()
class PROJECT_API AHealingItemActor : public AItemActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHealingItemActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, Category = HealingItem)
	UStaticMeshComponent* HealingItem;

	// 무기의 스탯들
	UPROPERTY(EditAnywhere, Category = "ItemStat")
	float m_fHealingSpeed;

	UPROPERTY(EditAnywhere, Category = "ItemStat")
	float m_fHealingDuration;

	UPROPERTY(EditAnywhere)
	ABaseCharacter* OwnerCharacter;

	UPROPERTY(EditAnywhere)
	FName HName;
};
