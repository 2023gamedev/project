// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PlayerSight.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_API UPlayerSight : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerSight();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	bool GetIsHit() { return m_bHasHit; }
	AActor* GetHitActor() { return HitActor; }

private:
	UPROPERTY(EditAnywhere)
	float m_fMaxSightDistance = 500.f;

	UPROPERTY(EditAnywhere)
	float m_fRadius = 50.f;

	UPROPERTY(EditAnywhere)
	bool m_bHasHit = false;

	UPROPERTY(EditAnywhere)
	AActor* HitActor;
};
