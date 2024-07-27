// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "HealingNiagaEffect.generated.h"

UCLASS()
class PROJECT_API AHealingNiagaEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY()
	UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	UNiagaraSystem* HealingFXSystem;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	UNiagaraComponent* HealingFXComponent;

};
