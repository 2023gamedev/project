// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFinder.h"

APathFinder::APathFinder()
{
    PrimaryActorTick.bCanEverTick = true;

}

void APathFinder::BeginPlay()
{
    Super::BeginPlay();

    GenerateEdges(GenerateNodes(GetWorld(), _GridSize), _GridSize);
    UE_LOG(LogTemp, Warning, TEXT("DebugTest"));
}

void APathFinder::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

TArray<FVector> APathFinder::GenerateNodes(UWorld* World, float GridSize)
{
    TArray<FVector> Nodes;

    // 레벨의 경계를 기준으로 노드를 생성
    for (float x = -LevelBoundary; x <= LevelBoundary; x += GridSize)
    {
        for (float y = -LevelBoundary; y <= LevelBoundary; y += GridSize)
        {
            FVector Location(x, y, StartLocation.Z);
            if (IsLocationNavigable(World, Location))
            {
                Nodes.Add(Location);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("Obstacle : %f,%f,%f"), Location.X, Location.Y, Location.Z);
            }
        }
    }

    NodeArr = Nodes;

    return Nodes;
}

bool APathFinder::IsLocationNavigable(UWorld* World, FVector Location)
{
    // 레이캐스트 등을 사용하여 Location이 이동 가능한지 확인
    FHitResult Hit;
    FVector Start = Location + FVector(0, 0, 100);
    FVector End = Location - FVector(0, 0, 100);
    FCollisionQueryParams Params;

    bool Result = !World->LineTraceSingleByChannel(Hit, Start, End, ECC_GameTraceChannel5, Params);

    if (Result)
        DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 10.f);
    else
        DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 10.f);

    return Result;
}

TMap<FVector, TArray<FVector>> APathFinder::GenerateEdges(const TArray<FVector>& Nodes, float GridSize)
{
    TMap<FVector, TArray<FVector>> Edges;

    for (const FVector& Node : Nodes)
    {
        TArray<FVector> Neighbors;
        for (const FVector& Offset : NeighborOffsets)
        {
            FVector Neighbor = Node + Offset * GridSize;
            if (Nodes.Contains(Neighbor))
            {
                Neighbors.Add(Neighbor);
            }
        }
        Edges.Add(Node, Neighbors);
    }

    EdgesMap = Edges;

    return Edges;
}

