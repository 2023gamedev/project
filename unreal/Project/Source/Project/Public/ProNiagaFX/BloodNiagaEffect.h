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
	int32 blood_spawncount_normalhit_min = 20;
	UPROPERTY(EditAnywhere)
	int32 blood_spawncount_normalhit_max = 40;
	UPROPERTY(EditAnywhere)
	int32 blood_spawncount_cuthit_min = 30;
	UPROPERTY(EditAnywhere)
	int32 blood_spawncount_cuthit_max = 60;

	UPROPERTY(EditAnywhere)	// 60
	int32 blood_spawncount_normaldead_min = 60;	
	UPROPERTY(EditAnywhere)	// 100
	int32 blood_spawncount_normaldead_max = 100;	
	UPROPERTY(EditAnywhere) // 60 => cutdead는 피 파티클 2개 만드는 거 유념
	int32 blood_spawncount_cutdead_min = 60;	
	UPROPERTY(EditAnywhere) // 90 => cutdead는 피 파티클 2개 만드는 거 유념
	int32 blood_spawncount_cutdead_max = 90;	

	UPROPERTY(EditAnywhere)
	bool spawn_flag = false;


	FTimerHandle SpawnTimerHandle;

	FTimerHandle StopSpawnTimerHandle;

	FTimerHandle EndTimerHandle;


	UPROPERTY(EditAnywhere)
	bool blood_spawnloop = false;
};
