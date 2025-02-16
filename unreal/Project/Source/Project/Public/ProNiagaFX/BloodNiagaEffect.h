// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "ProceduralMeshComponent.h"
#include "BloodNiagaEffect.generated.h"
 
class ABaseZombie;

UCLASS()
class PROJECT_API ABloodNiagaEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABloodNiagaEffect(); 

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void EndPlay(EEndPlayReason::Type type) override;

	void SpawnBloodEffect();

	void StopSpawnBloodEffect();

	void EndBloodEffect();


	UPROPERTY(EditAnywhere)
	UProceduralMeshComponent* ProcMesh;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "FX")
	UNiagaraSystem* BloodFXSystem;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FX")
	TArray<UNiagaraComponent*> BloodFXComponents;

	UPROPERTY(EditAnywhere)
	int32 blood_spawncount;

	UPROPERTY(EditAnywhere)
	bool spawn_flag = false;

	FTimerHandle SpawnTimerHandle;

	FTimerHandle StopSpawnTimerHandle;

	FTimerHandle EndTimerHandle;

	UPROPERTY(EditAnywhere)
	bool blood_spawnloop = false;
};
