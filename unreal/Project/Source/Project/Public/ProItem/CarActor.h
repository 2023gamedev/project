// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProItem/InterActor.h"
#include "Components/RectLightComponent.h"

#include "CarActor.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API ACarActor : public AInterActor
{
	GENERATED_BODY()
	
public:
	ACarActor();

	
	FTimerHandle ClearTimer;

	void UnLock();
	void ClearAddScore();



	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	FName CarKeyName;

	UPROPERTY(EditAnywhere)
	bool bIsUnlock;

	UPROPERTY(EditAnywhere)
	URectLightComponent* RectLight_1;
	UPROPERTY(EditAnywhere)
	URectLightComponent* RectLight_1_f;
	UPROPERTY(EditAnywhere)
	URectLightComponent* RectLight_2;
	UPROPERTY(EditAnywhere)
	URectLightComponent* RectLight_2_f;

};
