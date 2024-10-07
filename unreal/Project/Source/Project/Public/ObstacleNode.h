// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObstacleNode.generated.h"


UCLASS()
class AObstacleNode : public AActor
{
	GENERATED_BODY()

public:
	AObstacleNode();

protected:
	virtual void BeginPlay() override;
	void GenerateNodesAndEdges();
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere)
	float LevelBoundary = 0.f;

	UPROPERTY(EditAnywhere)
	float _GridSize = 50.f;

	UPROPERTY(EditAnywhere)
	FVector StartLocation = FVector(1800.f, 2200.f, 0.f);

	TArray<FVector> GenerateNodes(UWorld* World, float GridSize);
	bool IsLocationNavigable(UWorld* World, FVector Location);
	TMap<FVector, TArray<FEdgeData>> GenerateEdges(const TArray<FVector>& Nodes, float GridSize);

	const TArray<FVector> NeighborOffsets = {
	// 상하좌우 방향
	FVector(1, 0, 0), FVector(-1, 0, 0),
	FVector(0, 1, 0), FVector(0, -1, 0),
	// 대각선 방향
	FVector(1, 1, 0), FVector(-1, -1, 0),
	FVector(-1, 1, 0), FVector(1, -1, 0)
	};

	float mapminX = 16.f;
	float mapmaxX = 2366.f;
	float mapminY = -1220.f;
	float mapmaxY = 3960.f;

	TArray<FVector> NodeArr;
	TMap<FVector, TArray<FEdgeData>> EdgesMap;
};

// FVector와 float 값을 포함하는 구조체 정의
USTRUCT(Atomic, BlueprintType)
struct FEdgeData
{
	GENERATED_USTRUCT_BODY()

	// FVector와 float 변수
	UPROPERTY()
	FVector Location;

	UPROPERTY()
	float Weight;

	// 생성자
	FEdgeData()
		: Location(FVector::ZeroVector), Weight(0.f) {}

	FEdgeData(FVector InLocation, float InWeight)
		: Location(InLocation), Weight(InWeight) {}
};
