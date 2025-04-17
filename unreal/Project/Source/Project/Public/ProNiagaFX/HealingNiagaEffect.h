// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "ProCharacter/BaseCharacter.h"
#include "HealingNiagaEffect.generated.h"

class ABaseCharacter;

UCLASS()
class PROJECT_API AHealingNiagaEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	AHealingNiagaEffect();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void EndPlay(EEndPlayReason::Type type) override;


	UPROPERTY()
	UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "FX")
	UNiagaraSystem* HealingFXSystem;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FX")
	UNiagaraComponent* HealingFXComponent;

	UPROPERTY(EditAnywhere)
	ACharacter* OwnerChar;

	UPROPERTY(EditAnywhere)
	FVector spawn_offset = FVector(0, 0, 0.f);

	UPROPERTY(EditAnywhere)	// 1-플레이어(초록), 2-좀비(빨강)
	int materialType = 0;	

	UPROPERTY(EditAnywhere)	
	bool spawn_flag = false;

};
