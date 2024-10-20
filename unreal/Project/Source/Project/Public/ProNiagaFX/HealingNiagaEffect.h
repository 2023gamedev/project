// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
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

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	UNiagaraSystem* HealingFXSystem;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	UNiagaraComponent* HealingFXComponent;

	ABaseCharacter* OwnerChar;
};
