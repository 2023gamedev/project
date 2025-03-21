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

	// 피격시에 파티클 생성수
	UPROPERTY(EditAnywhere)	// 1
	int32 blood_spawncount_normalhit_min = 1;
	UPROPERTY(EditAnywhere)	// 3
	int32 blood_spawncount_normalhit_max = 3;
	UPROPERTY(EditAnywhere)	// 5
	int32 blood_spawncount_cuthit_min = 5;
	UPROPERTY(EditAnywhere)	// 10
	int32 blood_spawncount_cuthit_max = 10;
	UPROPERTY(EditAnywhere)	// 1 - 원격 동기화 용 (동기화 때는 때린 무기를 알수가 없어서;;)
	int32 blood_spawncount_awaysynchit_min = 1;
	UPROPERTY(EditAnywhere)	// 10 - 원격 동기화 용 (동기화 때는 때린 무기를 알수가 없어서;;)
	int32 blood_spawncount_awaysynchit_max = 10;

	// 사망시에 파티클 생성수
	UPROPERTY(EditAnywhere)	// 5
	int32 blood_spawncount_normaldead_min = 5;	
	UPROPERTY(EditAnywhere)	// 10
	int32 blood_spawncount_normaldead_max = 10;	
	UPROPERTY(EditAnywhere) // 30 => cutdead는 피 파티클 2개 만드는 거 유념
	int32 blood_spawncount_cutdead_min = 30;	
	UPROPERTY(EditAnywhere) // 50 => cutdead는 피 파티클 2개 만드는 거 유념
	int32 blood_spawncount_cutdead_max = 50;	

	UPROPERTY(EditAnywhere)
	bool spawn_flag = false;


	FTimerHandle SpawnTimerHandle;

	FTimerHandle StopSpawnTimerHandle;

	FTimerHandle EndTimerHandle;


	UPROPERTY(EditAnywhere)	// 절단 사망시에 절단된 시체 부위를 따라 다니며 여러번 피 이펙트를 생성하려고 만든 플래그 값
	bool blood_spawnloop = false;
};
