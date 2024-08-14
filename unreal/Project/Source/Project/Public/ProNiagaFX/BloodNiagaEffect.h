// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
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

	UPROPERTY()
	UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	UNiagaraSystem* BloodFXSystem;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	UNiagaraComponent* BloodFXComponent;

	ABaseZombie* OwnerZombie;

};
