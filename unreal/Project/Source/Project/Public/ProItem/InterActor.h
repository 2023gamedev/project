// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "InterActor.generated.h"

UCLASS()
class PROJECT_API AInterActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInterActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, Category = Box)
	UBoxComponent* Trigger;

	UPROPERTY(EditAnywhere, Category = Box)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, Category = InterActor)
	FName InterActorName;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
