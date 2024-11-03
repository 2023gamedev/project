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
	bool Activate = false;

	UPROPERTY(VisibleAnywhere)
	bool Activated = false;

	UPROPERTY(EditAnywhere)
	bool SaveNodeFile = false;

	UPROPERTY(EditAnywhere)
	bool DrawNodes = true;

	UPROPERTY(EditAnywhere)
	bool Activate_Floor_B2 = false;
	UPROPERTY(EditAnywhere)
	bool Activate_Floor_B1 = false;
	UPROPERTY(EditAnywhere)
	bool Activate_Floor_F1 = false;
	UPROPERTY(EditAnywhere)
	bool Activate_Floor_F2 = false;

	UPROPERTY(EditAnywhere)
	float DrawNode_LifeTime = 100.f;

	UPROPERTY(EditAnywhere)
	float LevelBoundary = 5000.f;

	UPROPERTY(EditAnywhere)
	float _GridSize = 50.f;

	UPROPERTY(EditAnywhere)
	FVector StartLocation = FVector(1800.f, 2200.f, 0.f);

	UPROPERTY(EditAnywhere)
	float B2_Z = 101.f;
	UPROPERTY(EditAnywhere)
	float B1_Z = 1051.f;
	UPROPERTY(EditAnywhere)
	float F1_Z = 2001.f;
	UPROPERTY(EditAnywhere)
	float F2_Z = 2962.f;

	TArray<FVector> GenerateNodes(UWorld* World, float GridSize);
	bool IsLocationNavigable(UWorld* World, FVector Location);
	TMap<FVector, TArray<FEdgeData>> GenerateEdges(const TArray<FVector>& Nodes, float GridSize, int floor);

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
	float mapminY = -1000.f;
	float mapmaxY = 3960.f;

	TArray<FVector> NodeArr;

	TMap<FVector, TArray<FEdgeData>> EdgesMap;

	FTimerHandle TimerHandle;
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
