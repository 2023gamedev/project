// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "ShoutingNiagaEffect.generated.h"

class ABaseZombie;

UCLASS()
class PROJECT_API AShoutingNiagaEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShoutingNiagaEffect();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void EndPlay(EEndPlayReason::Type type) override;

	UPROPERTY()
	UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "FX")
	UNiagaraSystem* ShoutingFXSystem;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "FX")
	UNiagaraComponent* ShoutingFXComponent;
};
