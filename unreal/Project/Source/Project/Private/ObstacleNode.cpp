// Fill out your copyright notice in the Description page of Project Settings.


#include "ObstacleNode.h"

AObstacleNode::AObstacleNode()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AObstacleNode::BeginPlay()
{
    Super::BeginPlay();
}

void AObstacleNode::GenerateNodesAndEdges()
{
    int floor;

    if (Activate_Floor_B2) {
        floor = 1;
        StartLocation.Z = B2_Z;
        GenerateEdges(GenerateNodes(GetWorld(), _GridSize), _GridSize, floor);
    }

    if (Activate_Floor_B1) {
        floor = 2;
        StartLocation.Z = B1_Z;
        GenerateEdges(GenerateNodes(GetWorld(), _GridSize), _GridSize, floor);
    }

    if (Activate_Floor_F1) {
        floor = 3;
        StartLocation.Z = F1_Z;
        GenerateEdges(GenerateNodes(GetWorld(), _GridSize), _GridSize, floor);
    }
   
    if (Activate_Floor_F2) {
        floor = 4;
        StartLocation.Z = F2_Z;
        GenerateEdges(GenerateNodes(GetWorld(), _GridSize), _GridSize, floor);
    }
    
    UE_LOG(LogTemp, Warning, TEXT("GenerateEdges_DebugTest"));
}

void AObstacleNode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (Activate == true && Activated == false) {
        Activate = false;
        Activated = true;

        GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AObstacleNode::GenerateNodesAndEdges, 1.0f, false);
    }
}

TArray<FVector> AObstacleNode::GenerateNodes(UWorld* World, float GridSize)
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
                //Nodes.Add(Location);
                UE_LOG(LogTemp, Warning, TEXT("Obstacle : %f,%f,%f"), Location.X, Location.Y, Location.Z);
            }
        }
    }

    NodeArr = Nodes;


    if (SaveNodeFile == false)
        return Nodes;

    // Nodes를 파일로 저장
    //FString FilePath = FPaths::ProjectDir() + TEXT("ObstacleNodes.txt");
    /*FString NodeData;

    for (const FVector& Node : Nodes)
    {
        if (Node.X > mapminX && Node.X < mapmaxX && Node.Y > mapminY && Node.Y < mapmaxY) {
            NodeData += FString::Printf(TEXT("%f,%f,%f\n"), Node.X, Node.Y, Node.Z);
        }

        //NodeData += FString::Printf(TEXT("%f,%f,%f\n"), Node.X, Node.Y, Node.Z);
    }*/

    //FFileHelper::SaveStringToFile(NodeData, *FilePath);

    return Nodes;
}

bool AObstacleNode::IsLocationNavigable(UWorld* World, FVector Location)
{
    // 레이캐스트 등을 사용하여 Location이 이동 가능한지 확인
    FHitResult Hit;
    FVector Start = Location + FVector(0, 0, 10);
    FVector End = Location - FVector(0, 0, 100);
    FCollisionQueryParams Params;

    bool Result = !World->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);

    if (DrawNodes) {
        if (Result) {
            if (Location.X > mapminX && Location.X < mapmaxX && Location.Y > mapminY && Location.Y < mapmaxY) {
                DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, DrawNode_LifeTime);
            }

        }
        else {
            DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, DrawNode_LifeTime);
        }
    }

    return Result;
}

// floor: 1 - B2 / 2 - B1 / 3 - F1 / 4 - F2
TMap<FVector, TArray<FEdgeData>> AObstacleNode::GenerateEdges(const TArray<FVector>& Nodes, float GridSize, int floor)
{
    TMap<FVector, TArray<FEdgeData>> Edges;

    for (const FVector& Node : Nodes)
    {
        if (Node.X > mapminX && Node.X < mapmaxX && Node.Y > mapminY && Node.Y < mapmaxY) {
            TArray<FEdgeData> EdgeDatas;
            for (const FVector& Offset : NeighborOffsets)
            {
                bool bDiagonal = (FMath::Abs(Offset.X) == 1 && FMath::Abs(Offset.Y) == 1);
                FVector Neighbor = Node + Offset * GridSize;

                FEdgeData EdgeData;
                EdgeData.Location = Neighbor;
                EdgeData.Weight = bDiagonal ? sqrt(2.f) : 1.f;

                if (Neighbor.X > mapminX && Neighbor.X <  mapmaxX && Neighbor.Y >mapminY && Neighbor.Y < mapmaxY) {
                    if (Nodes.Contains(Neighbor)) {
                        EdgeDatas.Add(EdgeData);
                    }
                }
            }
            Edges.Add(Node, EdgeDatas);
        }
    }

    EdgesMap = Edges;


    if (SaveNodeFile == false)
        return Edges;

    // 엣지 데이터를 문자열로 변환
    FString EdgeData;
    for (const auto& Edge : Edges)
    {
        const FVector& Node = Edge.Key;
        const TArray<FEdgeData>& EdgeDatas = Edge.Value;

        EdgeData += FString::Printf(TEXT("Node: %f, %f, %f\n"), Node.X, Node.Y, Node.Z);
        for (const FEdgeData& EdgeDataEntry : EdgeDatas)
        {
            EdgeData += FString::Printf(TEXT("Neighbor: %f, %f, %f\n"),
                EdgeDataEntry.Location.X,
                EdgeDataEntry.Location.Y,
                EdgeDataEntry.Location.Z);
            EdgeData += FString::Printf(TEXT("Weight: %f\n"), EdgeDataEntry.Weight);
        }
    }

    // 파일로 저장
    FString Floor = TEXT("");
    switch (floor) {
    case 1: Floor = TEXT("EdgesB2.txt"); break;
    case 2: Floor = TEXT("EdgesB1.txt"); break;
    case 3: Floor = TEXT("EdgesF1.txt"); break;
    case 4: Floor = TEXT("EdgesF2.txt"); break;
    }
    FString FilePath = FPaths::ProjectDir() + Floor;
    FFileHelper::SaveStringToFile(EdgeData, *FilePath);

    return Edges;
}

// 해야할 일
// TMap에 두번째 인자인 TArray<FVector>부분 FVector 아닌 cost값(거리)인 float 값 추가하기
// 그것을 txt로 뽑고 그것을 이용해서 서버에서 받아서 AStar 이것들을 이용하도록 수정하기