// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PlayerSight.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REALPROJECT_API UPlayerSight : public USceneComponent
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

	
	bool GetIsHit() { return HasHit;}
	AActor* GetHitActor() { return HitActor;}

private:
	UPROPERTY(EditAnywhere)
	float MaxSightDistance = 400.f;

	UPROPERTY(EditAnywhere)
	float Radius = 40.f;

	UPROPERTY(EditAnywhere)
	bool HasHit = false;

	UPROPERTY(EditAnywhere)
	AActor* HitActor;
};
