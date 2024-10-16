// Fill out your copyright notice in the Description page of Project Settings.


#include "ProGamemode/OneGameModeBase.h"
#include "ProCharacter/BaseCharacter.h"
#include "ProCharacter/EmployeeCharacter.h"
#include "ProCharacter/FireFighterCharacter.h"
#include "ProCharacter/GirlCharacter.h"
#include "ProCharacter/IdolCharacter.h"
#include "ProCharacter/PlayerCharacterController.h"
#include "ProZombie/BaseZombie.h"
#include "ProZombie/NormalZombie.h"
#include "ProZombie/ShoutingZombie.h"
#include "ProZombie/RunningZombie.h"
#include "ProZombie/RunningZombieAIController.h"
#include "ProZombie/ShoutingZombieAIController.h"
#include "ProZombie/ZombieAIController.h"
#include "Math/UnrealMathUtility.h"
#include "ProItem/ItemBoxActor.h"
#include "ProItem/InterActor.h"
#include "ProItem/CarActor.h"
#include "ProItem/RoofTopDoorActor.h"
#include "ClientSocket.h"
#include "EngineUtils.h"
#include "GameFramework/DefaultPawn.h"
#include "GameFramework/PlayerStart.h"
#include "ProGamemode/GameModeManager.h"
#include "ProGamemode/LobbyGameMode.h"
#include "ProGamemode/ProGameInstance.h"
#include "Math/UnrealMathUtility.h"
#include "NavigationSystem.h"

#include "NavMesh/RecastNavMesh.h"

TMap<uint32, ABaseZombie*> ZombieMap;

AOneGameModeBase::AOneGameModeBase()
{
    //DefaultPawnClass = ABaseCharacter::StaticClass();
    //PlayerControllerClass = APlayerCharacterController::StaticClass();


    // 변수들
    m_iItemBoxNumber = 0;

    m_iZombieNumber = 0;
    m_iShoutingZombieNumber = 0;
    m_iRunningZombieNumber = 0;

    // 플레이어를 선택한 것을 받아서 넘겨주려고 할 예정
    CharacterIconIndex = EPlayerCharacter::EMPLOYEE;

    ItemRandomLocationSetting();

    CarActorRandomLocationSetting();

    CarKeyRandomSetting();

    SpawnInterActorStaticClasses();
}

void AOneGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    
    SpawnInterItem(0, "CarActor");
    SpawnInterItem(1, "CarActor");
    SpawnInterItem(2, "CarActor");
    SpawnInterItem(3, "CarActor");
    SpawnInterItem(4, "CarActor");
    SpawnInterItem(5, "CarActor");
    SpawnInterItem(6, "CarActor");
    SpawnInterItem(7, "RoofTopDoorActor");


    ZombieMap.Empty(); // ZombieMap 초기화

    ABaseCharacter* DefaultPawn = nullptr;

    UWorld* World = GetWorld();
    if (World) {

        for (TActorIterator<ABaseCharacter> ActorItr(World); ActorItr; ++ActorItr) {
            UE_LOG(LogTemp, Error, TEXT("DefaultPawn Complete"));
            DefaultPawn = *ActorItr;
            if (DefaultPawn) {
                break;
            }
        }

        DefaultPawn->ThrowOnGround.BindUObject(this, &AOneGameModeBase::SpawnOnGroundItem);
    }

 
    // commit debug용 
    //TArray<FVector> OutVertices;
    //TArray<FVector> Outlines;
    //TArray<FVector> OutPoly;
    //ARecastNavMesh* NavMesh = Cast<ARecastNavMesh>(UNavigationSystemV1::GetCurrent(World)->MainNavData);
    //
    //
    //if (NavMesh)
    //{
    //    FRecastDebugGeometry NavMeshGeometry;
    //    NavMesh->GetDebugGeometry(NavMeshGeometry);
    //    //NavMesh->GetPathSegmentBoundaryEdges(NavMeshGeometry);       // 이거 Edge 받을 수 있는 거 처럼 생겼는데 인자 뭐 넘겨 줘야 되는 지 모르겠음;;
    //    //NavMesh->GetPolyEdges();
    //    
    //    Outlines.Append(NavMeshGeometry.NavMeshEdges);  // 사실상 안 담김
    //    OutPoly.Append(NavMeshGeometry.PolyEdges);      // 사실상 안 담김
    //
    //
    //
    //    int i = 0;
    //    // 데이터를 텍스트 파일로 저장하기 위한 문자열로 변환
    //    FString OutlineData;
    //    for (const FVector& Edge : Outlines)
    //    {
    //        i++;
    //        OutlineData += FString::Printf(TEXT("%f,%f,%f\n"), Edge.X, Edge.Y, Edge.Z);
    //    }
    //
    //    UE_LOG(LogTemp, Log, TEXT("Outlines :: %d!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"), i);
    //
    //    // 데이터를 텍스트 파일로 저장하기 위한 문자열로 변환
    //    FString OutPolyData;
    //    for (const FVector& Poly : OutPoly)
    //    {
    //        OutPolyData += FString::Printf(TEXT("%f,%f,%f\n"), Poly.X, Poly.Y, Poly.Z);
    //    }
    //
    //    // 저장할 파일 경로 설정 (프로젝트 디렉토리)
    //    FString FilePath = FPaths::ProjectDir() + TEXT("NavMeshEdges.txt");
    //    FString FilePathPol = FPaths::ProjectDir() + TEXT("NavMeshPolyes.txt");
    //
    //    // 파일에 데이터 저장
    //    FFileHelper::SaveStringToFile(OutlineData, *FilePath);
    //    FFileHelper::SaveStringToFile(OutPolyData, *FilePathPol);
    //
    //    OutVertices.Append(NavMeshGeometry.MeshVerts);
    //
    //    for (const FVector& Vertex : NavMeshGeometry.MeshVerts)
    //    {
    //        DrawDebugPoint(GetWorld(), Vertex, 10.0f, FColor::Red, false, -1.0f, 0);
    //    }
    //
    //    FString FilePathB2 = FPaths::ProjectDir() + TEXT("B2.txt");
    //    FString FilePathB1 = FPaths::ProjectDir() + TEXT("B1.txt");
    //    FString FilePathF1 = FPaths::ProjectDir() + TEXT("F1.txt");
    //    FString FilePathF2 = FPaths::ProjectDir() + TEXT("F2.txt");
    //    FString FilePathF3 = FPaths::ProjectDir() + TEXT("F3.txt");
    //
    //
    //    FString NodeDataB2;
    //    FString NodeDataB1;
    //    FString NodeDataF1;
    //    FString NodeDataF2;
    //    FString NodeDataF3;
    //
    //    for (const FVector& Node : OutVertices)
    //    {
    //        if (Node.Z < 800.f) {
    //            NodeDataB2 += FString::Printf(TEXT("%f,%f,%f\n"), Node.X, Node.Y, Node.Z);
    //        }
    //        else if (Node.Z < 1800.f) {
    //            NodeDataB1 += FString::Printf(TEXT("%f,%f,%f\n"), Node.X, Node.Y, Node.Z);
    //        }
    //        else if (Node.Z < 2500.f) {
    //            NodeDataF1 += FString::Printf(TEXT("%f,%f,%f\n"), Node.X, Node.Y, Node.Z);
    //        }
    //        else if (Node.Z < 3600.f) {
    //            NodeDataF2 += FString::Printf(TEXT("%f,%f,%f\n"), Node.X, Node.Y, Node.Z);
    //        }
    //        else {
    //            NodeDataF3 += FString::Printf(TEXT("%f,%f,%f\n"), Node.X, Node.Y, Node.Z);
    //        }
    //    }
    //
    //    FFileHelper::SaveStringToFile(NodeDataB2, *FilePathB2);
    //    FFileHelper::SaveStringToFile(NodeDataB1, *FilePathB1);
    //    FFileHelper::SaveStringToFile(NodeDataF1, *FilePathF1);
    //    FFileHelper::SaveStringToFile(NodeDataF2, *FilePathF2);
    //    FFileHelper::SaveStringToFile(NodeDataF3, *FilePathF3);
    //
    //}

    //==========================================
    {
        //// 현재 월드에서 네비게이션 시스템을 가져옵니다.
        //UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
        //if (NavSystem)
        //{
        //    // 네비게이션 데이터 인스턴스를 가져옵니다.
        //    ARecastNavMesh* NavData = Cast<ARecastNavMesh>(NavSystem->GetDefaultNavDataInstance());
        //    if (NavData)
        //    {
        //        // NavData를 통해 NavMesh 세부 정보에 접근할 수 있습니다.
        //        //const TArray<FNavMeshTile>& Tiles = NavData->GetNavMeshTiles();
        //        // Tiles를 통해 폴리곤 및 정점 정보를 가져옵니다.
        //        
        //        
        //        
        //        FBox Tiles = NavData->GetNavMeshTileBounds(0);
        //    }
        //}
    }
    //==========================================
}

void AOneGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);

    ChoiceCharacterBefore();
    ChoiceCharacter();
}


void AOneGameModeBase::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
}

void AOneGameModeBase::ChoiceCharacterBefore()
{
    // 현재 월드의 게임 인스턴스 가져오기
    UGameInstance* CurrentGameInstance = GetGameInstance();

    if (CurrentGameInstance) {
        UProGameInstance* GameInstance = Cast<UProGameInstance>(CurrentGameInstance);
        if (GameInstance) {
            int CharacterNumber = GameInstance->GetChoicedCharacterNumber();
            if (CharacterNumber == 0) {
                CharacterIconIndex = EPlayerCharacter::GIRL;
            }
            else if (CharacterNumber == 1) {
                CharacterIconIndex = EPlayerCharacter::EMPLOYEE;
            }
            else if (CharacterNumber == 2) {
                CharacterIconIndex = EPlayerCharacter::IDOL;
            }
            else if (CharacterNumber == 3) {
                CharacterIconIndex = EPlayerCharacter::FIREFIGHTER;
            }
        }
        else {
            // 게임 인스턴스가 유효하지 않은 경우에 대한 처리
        }
    }
    else {
        // 게임 인스턴스가 아직 생성되지 않은 경우에 대한 처리
    }
}


void AOneGameModeBase::ChoiceCharacter()
{
    if (CharacterIconIndex == EPlayerCharacter::EMPLOYEE) {
        PlayerCharacterClasses.Add(AEmployeeCharacter::StaticClass());
    }
    else if (CharacterIconIndex == EPlayerCharacter::FIREFIGHTER) {
        PlayerCharacterClasses.Add(AFireFighterCharacter::StaticClass());
    }
    else if (CharacterIconIndex == EPlayerCharacter::GIRL) {
        PlayerCharacterClasses.Add(AGirlCharacter::StaticClass());
    }
    else if (CharacterIconIndex == EPlayerCharacter::IDOL) {
        PlayerCharacterClasses.Add(AIdolCharacter::StaticClass());
    }


    PlayerCharacterControllerClasses.Add(APlayerCharacterController::StaticClass());

    DefaultPawnClass = PlayerCharacterClasses[0];
    PlayerControllerClass = PlayerCharacterControllerClasses[0];

}


void AOneGameModeBase::ItemRandomLocationSetting()
{
    // 0~19은 B1 20~39는 F1 40~59는 F2
// B1
    ItemRandomLocationStruct[0].sLocation = FVector(2330.f, 3200.f, 975.0626f);
    ItemRandomLocationStruct[1].sLocation = FVector(1880.f, 3870.f, 975.0626f);
    ItemRandomLocationStruct[2].sLocation = FVector(400.f, 3650.f, 975.0626f);

    ItemRandomLocationStruct[3].sLocation = FVector(1000.f, 2800.f, 975.0626f);
    ItemRandomLocationStruct[4].sLocation = FVector(2200.f, 3300.f, 975.0626f);
    ItemRandomLocationStruct[5].sLocation = FVector(1770.f, 2170.f, 975.0626f);

    ItemRandomLocationStruct[6].sLocation = FVector(440.f, 2310.f, 975.0626f);
    ItemRandomLocationStruct[7].sLocation = FVector(2300.f, 1800.f, 975.0626f);
    ItemRandomLocationStruct[8].sLocation = FVector(820.f, 1650.f, 975.0626f);

    ItemRandomLocationStruct[9].sLocation = FVector(120.f, 1200.f, 975.0626f);
    ItemRandomLocationStruct[10].sLocation = FVector(270.f, 2000.f, 975.0626f);
    ItemRandomLocationStruct[11].sLocation = FVector(1870.f, 800.f, 975.0626f);

    ItemRandomLocationStruct[12].sLocation = FVector(1200.f, 100.f, 975.0626f);
    ItemRandomLocationStruct[13].sLocation = FVector(30.f, 540.f, 975.0626f);
    ItemRandomLocationStruct[14].sLocation = FVector(500.f, -238.f, 975.0626f);

    ItemRandomLocationStruct[15].sLocation = FVector(1200.f, -976.f, 975.0626f);
    ItemRandomLocationStruct[16].sLocation = FVector(480.f, 800.f, 975.0626f);
    ItemRandomLocationStruct[17].sLocation = FVector(2330.f, 400.f, 975.0626f);

    ItemRandomLocationStruct[18].sLocation = FVector(730.f, 2130.f, 975.0626f);
    ItemRandomLocationStruct[19].sLocation = FVector(30.f, 3540.f, 975.0626f);


    // F1
    ItemRandomLocationStruct[20].sLocation = FVector(1200.f, 70.f, 1929.0626f);
    ItemRandomLocationStruct[21].sLocation = FVector(180.f, 520.f, 1929.0626f);
    ItemRandomLocationStruct[22].sLocation = FVector(1200.f, -817.f, 1929.0626f);

    ItemRandomLocationStruct[23].sLocation = FVector(800.f, -817.f, 1929.0626f);
    ItemRandomLocationStruct[24].sLocation = FVector(165.f, 960.f, 1929.0626f);
    ItemRandomLocationStruct[25].sLocation = FVector(1980.f, 1100.f, 1929.0626f);

    ItemRandomLocationStruct[26].sLocation = FVector(1390.f, 800.f, 1929.0626f);
    ItemRandomLocationStruct[27].sLocation = FVector(400.f, 1680.f, 1929.0626f);
    ItemRandomLocationStruct[28].sLocation = FVector(1100.f, 1260.f, 1929.0626f);

    ItemRandomLocationStruct[29].sLocation = FVector(70.f, 2390.f, 1929.0626f);
    ItemRandomLocationStruct[30].sLocation = FVector(550.f, 2520.f, 1929.0626f);
    ItemRandomLocationStruct[31].sLocation = FVector(200.f, 2800.f, 1929.0626f);

    ItemRandomLocationStruct[32].sLocation = FVector(1600.f, 3050.f, 1929.0626f);
    ItemRandomLocationStruct[33].sLocation = FVector(1530.f, 3900.f, 1929.0626f);
    ItemRandomLocationStruct[34].sLocation = FVector(2340.f, 3850.f, 1929.0626f);

    ItemRandomLocationStruct[35].sLocation = FVector(290.f, 3020.f, 1929.0626f);
    ItemRandomLocationStruct[36].sLocation = FVector(90.f, 3770.f, 1929.0626f);
    ItemRandomLocationStruct[37].sLocation = FVector(1050.f, 3350.f, 1929.0626f);

    ItemRandomLocationStruct[38].sLocation = FVector(50.f, 1740.f, 1929.0626f);
    ItemRandomLocationStruct[39].sLocation = FVector(2280.f, 2620.f, 1929.0626f);


    // F2                                                              
    ItemRandomLocationStruct[40].sLocation = FVector(870.f, -926.f, 2888.0626f);
    ItemRandomLocationStruct[41].sLocation = FVector(1260.f, -467.f, 2888.0626f);
    ItemRandomLocationStruct[42].sLocation = FVector(2170.f, 60.f, 2888.0626f);

    ItemRandomLocationStruct[43].sLocation = FVector(460.f, 800.f, 2888.0626f);
    ItemRandomLocationStruct[44].sLocation = FVector(1100.f, 600.f, 2888.0626f);
    ItemRandomLocationStruct[45].sLocation = FVector(1600.f, 1140.f, 2888.0626f);

    ItemRandomLocationStruct[46].sLocation = FVector(1200.f, 1300.f, 2888.0626f);
    ItemRandomLocationStruct[47].sLocation = FVector(1980.f, 1600.f, 2888.0626f);
    ItemRandomLocationStruct[48].sLocation = FVector(550.f, 1770.f, 2888.0626f);

    ItemRandomLocationStruct[49].sLocation = FVector(170.f, 1120.f, 2888.0626f);
    ItemRandomLocationStruct[50].sLocation = FVector(1240.f, 2270.f, 2888.0626f);
    ItemRandomLocationStruct[51].sLocation = FVector(1410.f, 2650.f, 2888.0626f);

    ItemRandomLocationStruct[52].sLocation = FVector(410.f, 3030.f, 2888.0626f);
    ItemRandomLocationStruct[53].sLocation = FVector(801.f, 3780.f, 2888.0626f);
    ItemRandomLocationStruct[54].sLocation = FVector(2001.f, 3470.f, 2888.0626f);

    ItemRandomLocationStruct[55].sLocation = FVector(1730.f, 1200.f, 2888.0626f);
    ItemRandomLocationStruct[56].sLocation = FVector(260.f, 3780.f, 2888.0626f);
    ItemRandomLocationStruct[57].sLocation = FVector(2310.f, 2040.f, 2888.0626f);

    ItemRandomLocationStruct[58].sLocation = FVector(869.f, 3120.f, 2888.0626f);
    ItemRandomLocationStruct[59].sLocation = FVector(1140.f, 1460.f, 2888.0626f);

    for (int i = 0; i < 60; ++i) {
        ItemRandomLocationStruct[i].bIsSeatLocation = false;
    }

}

FVector AOneGameModeBase::RandomItemBoxLocation(EItemFloor itemfloor)
{
    int32 RandomNumber = 0;

    int32 MinIndex = 0;
    int32 MaxIndex = 0;


    if (itemfloor == EItemFloor::FLOORB1) {
        MinIndex = 0;
        MaxIndex = 19;
    }
    else if (itemfloor == EItemFloor::FLOOR1) {
        MinIndex = 20;
        MaxIndex = 39;
    }
    else if (itemfloor == EItemFloor::FLOOR2) {
        MinIndex = 40;
        MaxIndex = 59;
    }

    while (true) {
        RandomNumber = FMath::RandRange(MinIndex, MaxIndex);

        if (!ItemRandomLocationStruct[RandomNumber].bIsSeatLocation) {
            ItemRandomLocationStruct[RandomNumber].bIsSeatLocation = true;
            return ItemRandomLocationStruct[RandomNumber].sLocation;
        }
    }

    return FVector::ZeroVector;
}

int32 AOneGameModeBase::RandomCarActorLocation()
{
    int32 RandomNumber = 0;

    while (true) {
        RandomNumber = FMath::RandRange(0, 6);

        if (!CarActorRandomLocationStruct[RandomNumber].bIsSeatLocation) {
            CarActorRandomLocationStruct[RandomNumber].bIsSeatLocation = true;
            return RandomNumber;
        }
    }


    return -1;
}

int32 AOneGameModeBase::RandomCarKey()
{
    int32 RandomNumber = 0;

    while (true) {
        RandomNumber = FMath::RandRange(0, 6);

        if (!CarKeyRandom[RandomNumber].bIsSeatCarKey) {
            CarKeyRandom[RandomNumber].bIsSeatCarKey = true;
            return RandomNumber;
        }
    }

    return -1;
}





void AOneGameModeBase::SpawnItemBoxes(uint32 itemboxindex, FName itemname, uint32 itemclass, UTexture2D* texture, int count, uint32 itemfloor, FVector itempos)
{
    ItemBoxClasses.Add(AItemBoxActor::StaticClass());

    TSubclassOf<AItemBoxActor> SelectedItemBoxClass = ItemBoxClasses[itemboxindex];

    EItemClass iclass;

    if (itemclass == 0) {
        iclass = EItemClass::NORMALWEAPON;
    }
    else if (itemclass == 1) {
        iclass = EItemClass::THROWINGWEAPON;
    }
    else if (itemclass == 2) {
        iclass = EItemClass::HEALINGITEM;
    }
    else if (itemclass == 3) {
        iclass = EItemClass::BLEEDINGHEALINGITEM;
    }
    else if (itemclass == 4) {
        iclass = EItemClass::KEYITEM;
    }
    else if (itemclass == 5) {
        iclass = EItemClass::BAGITEM;
    }
    else if (itemclass == 6) {
        iclass = EItemClass::NONE;
    }

    // 선택된 아이템 박스 클래스로 아이템 박스 생성
    AItemBoxActor* SpawnedItemBox = GetWorld()->SpawnActor<AItemBoxActor>(SelectedItemBoxClass, itempos, FRotator::ZeroRotator);

    UE_LOG(LogTemp, Error, TEXT("ITEM___111"));
    if (SpawnedItemBox) {
        SpawnedItemBox->ItemName = itemname;
        SpawnedItemBox->ItemClassType = iclass;
        SpawnedItemBox->Texture = texture;
        SpawnedItemBox->Count = count;
        SpawnedItemBox->ItemBoxId = itemboxindex;
    }
    m_iItemBoxNumber++;
}

void AOneGameModeBase::SpawnOnGroundItem(FName itemname, EItemClass itemclass, UTexture2D* texture, int count)
{
    ABaseCharacter* DefaultPawn = nullptr;


    UWorld* World = GetWorld();
    if (World) {
        for (TActorIterator<ABaseCharacter> ActorItr(World); ActorItr; ++ActorItr) {
            DefaultPawn = *ActorItr;
            if (DefaultPawn) {
                break;
            }
        }
    }


    FVector DropPos = DefaultPawn->GetActorForwardVector() * 100.f;

    ItemBoxClasses.Add(AItemBoxActor::StaticClass());
    TSubclassOf<AItemBoxActor> SelectedItemBoxClass = ItemBoxClasses[GetItemBoxNumber()];
    FVector itemboxpos = DefaultPawn->GetActorLocation() + FVector(DropPos.X, DropPos.Y, -60.149886f);

    AItemBoxActor* SpawnedItemBox = GetWorld()->SpawnActor<AItemBoxActor>(SelectedItemBoxClass, itemboxpos, FRotator::ZeroRotator);

    if (SpawnedItemBox) {
        SpawnedItemBox->ItemName = itemname;
        SpawnedItemBox->ItemClassType = itemclass;
        SpawnedItemBox->Texture = texture;
        SpawnedItemBox->Count = count;
    }

    m_iItemBoxNumber++;
}

void AOneGameModeBase::CarActorRandomLocationSetting()
{
    CarActorRandomLocationStruct[0].sLocation = FVector(1772.f, 978.f, 60.f);
    CarActorRandomLocationStruct[1].sLocation = FVector(1662.f, 1678.f, 60.f);
    CarActorRandomLocationStruct[2].sLocation = FVector(722.f, 978.f, 60.f);
    CarActorRandomLocationStruct[3].sLocation = FVector(1142.f, 2068.f, 60.f);
    CarActorRandomLocationStruct[4].sLocation = FVector(632.f, 2688.f, 60.f);
    CarActorRandomLocationStruct[5].sLocation = FVector(1492.f, 2680.f, 60.f);
    CarActorRandomLocationStruct[6].sLocation = FVector(1122.f, 3468.f, 60.f);

    CarActorRandomLocationStruct[0].sRotation = FRotator(0.f, -30.f, 0.f);
    CarActorRandomLocationStruct[1].sRotation = FRotator(0.f, 30.f, 0.f);
    CarActorRandomLocationStruct[2].sRotation = FRotator(0.f, -50.f, 0.f);
    CarActorRandomLocationStruct[3].sRotation = FRotator(0.f, 30.f, 0.f);
    CarActorRandomLocationStruct[4].sRotation = FRotator(0.f, -30.f, 0.f);
    CarActorRandomLocationStruct[5].sRotation = FRotator(0.f, -130.f, 0.f);
    CarActorRandomLocationStruct[6].sRotation = FRotator(0.f, -70.f, 0.f);

    for (int i = 0; i < 7; ++i) {
        CarActorRandomLocationStruct[i].bIsSeatLocation = false;
    }

}

void AOneGameModeBase::CarKeyRandomSetting()
{
    CarKeyRandom[0].CarKeyName = "CarKey1";
    CarKeyRandom[1].CarKeyName = "CarKey2";
    CarKeyRandom[2].CarKeyName = "CarKey3";
    CarKeyRandom[3].CarKeyName = "CarKey4";
    CarKeyRandom[4].CarKeyName = "None";
    CarKeyRandom[5].CarKeyName = "None";
    CarKeyRandom[6].CarKeyName = "None";

    for (int i = 0; i < 7; ++i) {
        CarKeyRandom[i].bIsSeatCarKey = false;
    }
}

void AOneGameModeBase::SpawnInterItem(int32 InterActorindex, FName InterName)
{

    TSubclassOf<AInterActor> SelectedInterActorClass = InterActorClasses[InterActorindex];


    FVector Location;
    FRotator Rotation;
    if (InterName == "CarActor") {
        int RandomValue = RandomCarActorLocation();
        int Randomkey = RandomCarKey();
        Location = CarActorRandomLocationStruct[RandomValue].sLocation;
        Rotation = CarActorRandomLocationStruct[RandomValue].sRotation;

        FName CarKey = CarKeyRandom[Randomkey].CarKeyName;

        // 선택된 아이템 박스 클래스로 아이템 박스 생성
        ACarActor* SpawnedCarActor = GetWorld()->SpawnActor<ACarActor>(SelectedInterActorClass, Location, Rotation);
        SpawnedCarActor->CarKeyName = CarKey;
    }
    else if (InterName == "RoofTopDoorActor") {

        ARoofTopDoorActor* SpawnedRoofTopDoor = GetWorld()->SpawnActor<ARoofTopDoorActor>(SelectedInterActorClass, FVector(2410.f, 200.f, 3940.f), FRotator::ZeroRotator);
    }


}

void AOneGameModeBase::SpawnInterActorStaticClasses()
{
    InterActorClasses.Add(ACarActor::StaticClass());
    InterActorClasses.Add(ACarActor::StaticClass());
    InterActorClasses.Add(ACarActor::StaticClass());
    InterActorClasses.Add(ACarActor::StaticClass());
    InterActorClasses.Add(ACarActor::StaticClass());
    InterActorClasses.Add(ACarActor::StaticClass());
    InterActorClasses.Add(ACarActor::StaticClass());
    InterActorClasses.Add(ARoofTopDoorActor::StaticClass());

}

void AOneGameModeBase::UpdateOtherPlayer(uint32 PlayerID, FVector NewLocation, FRotator NewRotation, uint32 charactertype, float hp)
{
    UWorld* World = GetWorld();

    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("UpdateOtherPlayer: GetWorld() returned nullptr"));
        return;
    }

    // 캐릭터 검색
    for (TActorIterator<ABaseCharacter> It(World); It; ++It)
    {
        ABaseCharacter* BasePlayer = *It;
        if (BasePlayer && BasePlayer->GetPlayerId() == PlayerID)
        {
            FVector OldLocation = BasePlayer->GetActorLocation();

            // 기존 캐릭터 위치 업데이트
            BasePlayer->SetActorLocation(NewLocation);
            BasePlayer->SetActorRotation(NewRotation);

            BasePlayer->UpdatePlayerData(NewLocation);

            BasePlayer->SetHP(hp);

            return;
        }
    }

    // 캐릭터 타입에 따라 스폰할 클래스를 결정
    TSubclassOf<ABaseCharacter> CharacterClass = nullptr;

    if (charactertype == 1) {
        CharacterClass = AEmployeeCharacter::StaticClass();
        //OtherCharacterClasses.Add(AEmployeeCharacter::StaticClass());
    }
    else if (charactertype == 3) {
        CharacterClass = AFireFighterCharacter::StaticClass();
        //OtherCharacterClasses.Add(AFireFighterCharacter::StaticClass());
    }
    else if (charactertype == 0) {
        CharacterClass = AGirlCharacter::StaticClass();
        //OtherCharacterClasses.Add(AGirlCharacter::StaticClass());
    }
    else if (charactertype == 2) {
        CharacterClass = AIdolCharacter::StaticClass();
        //OtherCharacterClasses.Add(AIdolCharacter::StaticClass());
    }

    if (CharacterClass)
    {
        //기존 캐릭터를 찾지 못한 경우에만 새 캐릭터 스폰
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        ABaseCharacter* NewCharacter = World->SpawnActor<ABaseCharacter>(CharacterClass, NewLocation, NewRotation, SpawnParams);

        if (NewCharacter)
        {
            // 새 캐릭터에 PlayerId 설정
            NewCharacter->SetPlayerId(PlayerID);
            NewCharacter->SetHP(hp);  // 초기 HP 설정
        }
    }
}

void AOneGameModeBase::UpdatePlayerAttack(uint32 PlayerID, bool battack)
{
    UWorld* World = GetWorld();

    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("UpdateOtherPlayer: GetWorld() returned nullptr"));
        return;
    }

    // 캐릭터 검색
    for (TActorIterator<ABaseCharacter> It(World); It; ++It)
    {
        ABaseCharacter* BasePlayer = *It;
        if (BasePlayer && BasePlayer->GetPlayerId() == PlayerID)
        {
            BasePlayer->SetAttack(battack);
            //UE_LOG(LogTemp, Warning, TEXT("real update attack: %d, %d"), PlayerID, b_attack);

            return;
        }
    }
}

void AOneGameModeBase::UpdateEquipItem(uint32 PlayerID, const FString& Itemname, uint32 itemtype)
{
    UWorld* World = GetWorld();

    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("UpdateOtherPlayer: GetWorld() returned nullptr"));
        return;
    }

    // 캐릭터 검색
    for (TActorIterator<ABaseCharacter> It(World); It; ++It)
    {
        ABaseCharacter* BasePlayer = *It;
        if (BasePlayer && BasePlayer->GetPlayerId() == PlayerID)
        {
            if (Itemname != "") {
                if (itemtype == 0) {
                    BasePlayer->OtherSpawnBleedingHealingItem(Itemname);
                }
                else if (itemtype == 1) {
                    BasePlayer->OtherSpawnHealingItem(Itemname);
                }
                else if (itemtype == 2) {
                    BasePlayer->OtherSpawnThrowWeapon(Itemname);
                }
                else if (itemtype == 3) {
                    BasePlayer->OtherSpawnKeyItem(Itemname);
                }
                else if (itemtype == 4) {
                    BasePlayer->OtherSpawnNormalWeapon(Itemname);
                }
                UE_LOG(LogTemp, Warning, TEXT("real update equip: %d"), PlayerID);
            }
            return;
        }
    }
}

void AOneGameModeBase::UpdatePlayerRun(uint32 PlayerID, bool b_run)
{
    UWorld* World = GetWorld();

    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("UpdateOtherPlayer: GetWorld() returned nullptr"));
        return;
    }

    // 캐릭터 검색
    for (TActorIterator<ABaseCharacter> It(World); It; ++It)
    {
        ABaseCharacter* BasePlayer = *It;
        if (BasePlayer && BasePlayer->GetPlayerId() == PlayerID)
        {
            BasePlayer->Other_Run(b_run);
            UE_LOG(LogTemp, Warning, TEXT("real update run: %d, %d"), PlayerID, b_run);

            return;
        }
    }
}

void AOneGameModeBase::UpdatePlayerJump(uint32 PlayerID)
{
    UWorld* World = GetWorld();

    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("UpdateOtherPlayer: GetWorld() returned nullptr"));
        return;
    }

    // 캐릭터 검색
    for (TActorIterator<ABaseCharacter> It(World); It; ++It)
    {
        ABaseCharacter* BasePlayer = *It;
        if (BasePlayer && BasePlayer->GetPlayerId() == PlayerID)
        {
            BasePlayer->SetPlayerJump();
            UE_LOG(LogTemp, Warning, TEXT("real update jump: %d"), PlayerID);

            return;
        }
    }
}


void AOneGameModeBase::UpdateZombie(uint32 ZombieID, uint32 ZombieType, FVector NewLocation, FRotator NewRotation)
{
    UWorld* World = GetWorld();

    //UE_LOG(LogTemp, Warning, TEXT("Call zombie ID: %d"), ZombieID);

    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("UpdateZombie: GetWorld() returned nullptr"));
        return;
    }

    TSubclassOf<ABaseZombie> ZombieClass = nullptr;
    TSubclassOf<AAIController> ZombieAIClass = nullptr;

    if (ZombieType == 0) {
        ZombieClass = ANormalZombie::StaticClass();
        ZombieAIClass = AZombieAIController::StaticClass();
    }
    else if (ZombieType == 1) {
        ZombieClass = AShoutingZombie::StaticClass();
        ZombieAIClass = AShoutingZombieAIController::StaticClass();
    }
    else if (ZombieType == 2) {
        ZombieClass = ARunningZombie::StaticClass();
        ZombieAIClass = ARunningZombieAIController::StaticClass();
    }

    // 0번 좀비가 제대로 함수 호출을 하긴 하는지 확인
    UE_LOG(LogTemp, Warning, TEXT("Updated Zombie ID: %d at new location"), ZombieID);

    ABaseZombie** ZombiePtr = ZombieMap.Find(ZombieID);
    if (!ZombiePtr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Could not find Zombie ID: %d"), ZombieID);
    }
    if (ZombiePtr && *ZombiePtr && IsValid(*ZombiePtr))
    {
        ABaseZombie* BaseZombie = *ZombiePtr;
        if (BaseZombie)
        {

            // 기존 캐릭터 위치 업데이트
            BaseZombie->SetActorLocation(NewLocation);
            BaseZombie->SetActorRotation(NewRotation);

            //좀비 애니메이션 업데이트용
            BaseZombie->UpdateZombieData(NewLocation);

            
            //UE_LOG(LogTemp, Warning, TEXT("Updated Zombie ID: %d at new location"), ZombieID);
        }
    }
    else
    {
        // 기존 좀비가 없으면 새로운 좀비 생성
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        ABaseZombie* NewZombie = World->SpawnActor<ABaseZombie>(ZombieClass, NewLocation, NewRotation, SpawnParams);

        if (NewZombie)
        {
            // 새 좀비에 ZombieID 설정 및 ZombieMap에 추가
            NewZombie->SetZombieId(ZombieID);
            ZombieMap.Add(ZombieID, NewZombie);

            UE_LOG(LogTemp, Warning, TEXT("Spawned new Zombie ID: %d"), ZombieID);

            if (ZombieType == 0) {
                AZombieAIController* AIZombieController = World->SpawnActor<AZombieAIController>(ZombieAIClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
                if (AIZombieController)
                {
                    AIZombieController->Possess(NewZombie);
                    AIZombieController->OwnerZombie = Cast<ANormalZombie>(NewZombie);

                    UE_LOG(LogTemp, Warning, TEXT("Spawned and possessed new Zombie ID: %d"), ZombieID);
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("Failed to spawn AI Controller for Zombie ID: %d"), ZombieID);
                }
            }
            else if (ZombieType == 1) {
                AShoutingZombieAIController* AIShoutingZombieController = World->SpawnActor<AShoutingZombieAIController>(ZombieAIClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
                if (AIShoutingZombieController)
                {
                    AIShoutingZombieController->Possess(NewZombie);
                    AIShoutingZombieController->OwnerZombie = Cast<AShoutingZombie>(NewZombie);

                    UE_LOG(LogTemp, Warning, TEXT("Spawned and possessed new Zombie ID: %d"), ZombieID);
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("Failed to spawn AI Controller for Zombie ID: %d"), ZombieID);
                }
            }
            else if (ZombieType == 2) {
                ARunningZombieAIController* AIRunningZombieController = World->SpawnActor<ARunningZombieAIController>(ZombieAIClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
                if (AIRunningZombieController)
                {
                    AIRunningZombieController->Possess(NewZombie);
                    AIRunningZombieController->OwnerZombie = Cast<ARunningZombie>(NewZombie);

                    UE_LOG(LogTemp, Warning, TEXT("Spawned and possessed new Zombie ID: %d"), ZombieID);
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("Failed to spawn AI Controller for Zombie ID: %d"), ZombieID);
                }
            }


        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to spawn new Zombie ID: %d"), ZombieID);
        }
        //UE_LOG(LogTemp, Warning, TEXT("No zombie found with ID: %d"), ZombieID);
    }
}


void AOneGameModeBase::UpdateZombieAttack(uint32 ZombieId, uint32 PlayerId)
{
    UWorld* World = GetWorld();

    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("UpdateZombieAttack: GetWorld() returned nullptr"));
        return;
    }

    // ZombieMap에서 ZombieID로 좀비를 찾음
    ABaseZombie** ZombiePtr = ZombieMap.Find(ZombieId);
    if (ZombiePtr && *ZombiePtr)
    {
        ABaseZombie* BaseZombie = *ZombiePtr;
        if (BaseZombie)
        {
            // 좀비의 공격 상태 업데이트
            BaseZombie->Attack(PlayerId);
            UE_LOG(LogTemp, Warning, TEXT("Updated Zombie ID: %d attack"), ZombieId);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No zombie found with ID: %d"), ZombieId);
    }
}

void AOneGameModeBase::UpdateZombieHP(uint32 ZombieId, uint32 HP)
{
    UWorld* World = GetWorld();

    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("UpdateZombieHP: GetWorld() returned nullptr"));
        return;
    }

    // ZombieMap에서 ZombieID로 좀비를 찾음
    ABaseZombie** ZombiePtr = ZombieMap.Find(ZombieId);
    if (ZombiePtr && *ZombiePtr)
    {
        ABaseZombie* BaseZombie = *ZombiePtr;
        if (BaseZombie)
        {
            //좀비의 체력상태 업데이트
            BaseZombie->SetHP(HP);
            UE_LOG(LogTemp, Warning, TEXT("Updated Zombie ID: %d HP state to: %d"), ZombieId, HP);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No zombie found with ID: %d"), ZombieId);
    }
}

void AOneGameModeBase::DestroyItem(uint32 ItemId)
{
    for (TActorIterator<AItemBoxActor> It(GetWorld()); It; ++It)
    {
        AItemBoxActor* ItemBox = *It;
        if (ItemBox && ItemBox->ItemBoxId == ItemId)
        {
            ItemBox->Destroy();
            break;
        }
    }
}

void AOneGameModeBase::BeginDestroy()
{
    Super::BeginDestroy();

    // 게임 종료 시 모든 좀비 객체와 맵 초기화
    for (auto& Elem : ZombieMap)
    {
        if (Elem.Value)
        {
            Elem.Value->Destroy();
        }
    }
    ZombieMap.Empty();
}