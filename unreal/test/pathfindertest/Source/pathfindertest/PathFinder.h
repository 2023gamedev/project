// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PathFinder.generated.h"

UCLASS()
class APathFinder : public AActor
{
	GENERATED_BODY()

public:
	APathFinder();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
		float LevelBoundary = 0.f;

	UPROPERTY(EditAnywhere)
		float _GridSize = 0.f;

	UPROPERTY(EditAnywhere)
	FVector StartLocation = FVector(0.f, 0.f, 0.f);

	TArray<FVector> GenerateNodes(UWorld* World, float GridSize);
	bool IsLocationNavigable(UWorld* World, FVector Location);
	TMap<FVector, TArray<FVector>> GenerateEdges(const TArray<FVector>& Nodes, float GridSize);

	const TArray<FVector> NeighborOffsets = {
	FVector(1, 0, 0), FVector(-1, 0, 0),
	FVector(0, 1, 0), FVector(0, -1, 0)
	};

	TArray<FVector> NodeArr;
	TMap<FVector, TArray<FVector>> EdgesMap;
};
