// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "ResurrectNiagaEffect.generated.h"

/**
 * 
 */

UCLASS()
class PROJECT_API AResurrectNiagaEffect : public AActor
{
	GENERATED_BODY()

public:
	AResurrectNiagaEffect();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void EndPlay(EEndPlayReason::Type type) override;

	void SpawnResurrectEffect();

	void EndResurrectEffect();


	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "FX")
	UNiagaraSystem* ResurrectFXSystem;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FX")
	UNiagaraComponent* ResurrectFXComponent;

	UPROPERTY(EditAnywhere)
	bool spawn_flag = false;


	FTimerHandle EndTimerHandle;

};
