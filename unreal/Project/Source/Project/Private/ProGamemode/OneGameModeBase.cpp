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


    //SpawnZombiesStaticClasses();

    ItemRandomLocationSetting();

    CarActorRandomLocationSetting();

    CarKeyRandomSetting();

    SpawnInterActorStaticClasses();
}

void AOneGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    // B1
    SpawnItemBoxes(0, "SquareWood", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenSquareWood.InvenSquareWood"), NULL, LOAD_None, NULL), 1, EItemFloor::FLOORB1);
    SpawnItemBoxes(1, "FireExtinguisher", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenFireEx.InvenFireEx"), NULL, LOAD_None, NULL), 1, EItemFloor::FLOORB1);
    SpawnItemBoxes(2, "Pipe", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenPipe.InvenPipe"), NULL, LOAD_None, NULL), 1, EItemFloor::FLOORB1);
    SpawnItemBoxes(3, "FireAxe", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenFireAxe.InvenFireAxe"), NULL, LOAD_None, NULL), 1, EItemFloor::FLOORB1);
    SpawnItemBoxes(4, "SashimiKnife", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenSasimiKnife.InvenSasimiKnife"), NULL, LOAD_None, NULL), 1, EItemFloor::FLOORB1);
    SpawnItemBoxes(5, "ButchersKnife", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenButherKnife.InvenButherKnife"), NULL, LOAD_None, NULL), 1, EItemFloor::FLOORB1);
    SpawnItemBoxes(6, "Bottle", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenBottle.InvenBottle"), NULL, LOAD_None, NULL), 1, EItemFloor::FLOORB1);
    SpawnItemBoxes(7, "DirtyClothes", EItemClass::BLEEDINGHEALINGITEM, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenDirtyCloth.InvenDirtyCloth"), NULL, LOAD_None, NULL), 3, EItemFloor::FLOORB1);
    SpawnItemBoxes(8, "Water", EItemClass::HEALINGITEM, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenWater.InvenWater"), NULL, LOAD_None, NULL), 2, EItemFloor::FLOORB1);
    SpawnItemBoxes(9, "Smoke", EItemClass::HEALINGITEM, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenSmoke.InvenSmoke"), NULL, LOAD_None, NULL), 2, EItemFloor::FLOORB1);

    SpawnItemBoxes(10, "Drink", EItemClass::HEALINGITEM, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenDrink.InvenDrink"), NULL, LOAD_None, NULL), 2, EItemFloor::FLOORB1);
    SpawnItemBoxes(11, "Snack", EItemClass::HEALINGITEM, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenSnack.InvenSnack"), NULL, LOAD_None, NULL), 2, EItemFloor::FLOORB1);
    SpawnItemBoxes(12, "CannedTuna", EItemClass::HEALINGITEM, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenCannedTuna.InvenCannedTuna"), NULL, LOAD_None, NULL), 2, EItemFloor::FLOORB1);
    SpawnItemBoxes(13, "Water", EItemClass::HEALINGITEM, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenWater.InvenWater"), NULL, LOAD_None, NULL), 2, EItemFloor::FLOORB1);
    SpawnItemBoxes(14, "Smoke", EItemClass::HEALINGITEM, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenSmoke.InvenSmoke"), NULL, LOAD_None, NULL), 2, EItemFloor::FLOORB1);
    SpawnItemBoxes(15, "Drink", EItemClass::HEALINGITEM, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenDrink.InvenDrink"), NULL, LOAD_None, NULL), 2, EItemFloor::FLOORB1);
    SpawnItemBoxes(16, "BagActor", EItemClass::BAGITEM, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenBag.InvenBag"), NULL, LOAD_None, NULL), 1, EItemFloor::FLOORB1); // 수정 필요
    SpawnItemBoxes(17, "BigBagActor", EItemClass::BAGITEM, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenBigBag.InvenBigBag"), NULL, LOAD_None, NULL), 1, EItemFloor::FLOORB1);

    SpawnItemBoxes(18, "CarKey1", EItemClass::KEYITEM, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenCarKey1.InvenCarKey1"), NULL, LOAD_None, NULL), 1, EItemFloor::FLOORB1);
    SpawnItemBoxes(19, "RoofKey1", EItemClass::KEYITEM, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenRoofKey1.InvenRoofKey1"), NULL, LOAD_None, NULL), 1, EItemFloor::FLOORB1);

    // F1
    SpawnItemBoxes(20, "SquareWood", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenSquareWood.InvenSquareWood"), NULL, LOAD_None, NULL), 1, EItemFloor::FLOOR1);
    SpawnItemBoxes(21, "FireExtinguisher", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenFireEx.InvenFireEx"), NULL, LOAD_None, NULL), 1, EItemFloor::FLOOR1);
    SpawnItemBoxes(22, "Pipe", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenPipe.InvenPipe"), NULL, LOAD_None, NULL), 1, EItemFloor::FLOOR1);
    SpawnItemBoxes(23, "FireAxe", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenFireAxe.InvenFireAxe"), NULL, LOAD_None, NULL), 1, EItemFloor::FLOOR1);
    SpawnItemBoxes(24, "MannequinArm", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvnenMannequinArm.InvnenMannequinArm"), NULL, LOAD_None, NULL), 1, EItemFloor::FLOOR1);
    SpawnItemBoxes(25, "MannequinLeg", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenMannequinLeg.InvenMannequinLeg"), NULL, LOAD_None, NULL), 1, EItemFloor::FLOOR1);
    SpawnItemBoxes(26, "Book", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/Invenbook.Invenbook"), NULL, LOAD_None, NULL), 1, EItemFloor::FLOOR1);
    SpawnItemBoxes(27, "Scissors", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenSissor.InvenSissor"), NULL, LOAD_None, NULL), 1, EItemFloor::FLOOR1);
    SpawnItemBoxes(28, "MagicStick", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenMagicStick.InvenMagicStick"), NULL, LOAD_None, NULL), 1, EItemFloor::FLOOR1);
    SpawnItemBoxes(29, "DirtyClothes", EItemClass::BLEEDINGHEALINGITEM, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenDirtyCloth.InvenDirtyCloth"), NULL, LOAD_None, NULL), 3, EItemFloor::FLOOR1);

    SpawnItemBoxes(30, "Bandage", EItemClass::BLEEDINGHEALINGITEM, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenBandage.InvenBandage"), NULL, LOAD_None, NULL), 3, EItemFloor::FLOOR1);
    SpawnItemBoxes(31, "Clothes", EItemClass::BLEEDINGHEALINGITEM, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenCloth.InvenCloth"), NULL, LOAD_None, NULL), 3, EItemFloor::FLOOR1);
    SpawnItemBoxes(32, "Gauze", EItemClass::BLEEDINGHEALINGITEM, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenGauze.InvenGauze"), NULL, LOAD_None, NULL), 3, EItemFloor::FLOOR1);
    SpawnItemBoxes(33, "Water", EItemClass::HEALINGITEM, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenWater.InvenWater"), NULL, LOAD_None, NULL), 2, EItemFloor::FLOOR1);
    SpawnItemBoxes(34, "Smoke", EItemClass::HEALINGITEM, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenSmoke.InvenSmoke"), NULL, LOAD_None, NULL), 2, EItemFloor::FLOOR1);
    SpawnItemBoxes(35, "Disinfectant", EItemClass::HEALINGITEM, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenDisinfectant.InvenDisinfectant"), NULL, LOAD_None, NULL), 2, EItemFloor::FLOOR1);
    SpawnItemBoxes(36, "Ointment", EItemClass::HEALINGITEM, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenOintment.InvenOintment"), NULL, LOAD_None, NULL), 2, EItemFloor::FLOOR1);
    SpawnItemBoxes(37, "BigBagActor", EItemClass::BAGITEM, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenBigBag.InvenBigBag"), NULL, LOAD_None, NULL), 1, EItemFloor::FLOOR1);

    SpawnItemBoxes(38, "CarKey2", EItemClass::KEYITEM, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenCarKey1.InvenCarKey1"), NULL, LOAD_None, NULL), 1, EItemFloor::FLOOR1);
    SpawnItemBoxes(39, "RoofKey2", EItemClass::KEYITEM, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenRoofKey2.InvenRoofKey2"), NULL, LOAD_None, NULL), 1, EItemFloor::FLOOR1);

    // F2
    SpawnItemBoxes(40, "SquareWood", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenSquareWood.InvenSquareWood"), NULL, LOAD_None, NULL), 1, EItemFloor::FLOOR2);
    SpawnItemBoxes(41, "FireExtinguisher", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenFireEx.InvenFireEx"), NULL, LOAD_None, NULL), 1, EItemFloor::FLOOR2);
    SpawnItemBoxes(42, "Pipe", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenPipe.InvenPipe"), NULL, LOAD_None, NULL), 2, EItemFloor::FLOOR2);
    SpawnItemBoxes(43, "FireAxe", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenFireAxe.InvenFireAxe"), NULL, LOAD_None, NULL), 1, EItemFloor::FLOOR2);
    SpawnItemBoxes(44, "Plunger", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenPlunger.InvenPlunger"), NULL, LOAD_None, NULL), 1, EItemFloor::FLOOR2);
    SpawnItemBoxes(45, "FryingPan", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenFryingPan.InvenFryingPan"), NULL, LOAD_None, NULL), 1, EItemFloor::FLOOR2);
    SpawnItemBoxes(46, "Iron", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenIron.InvenIron"), NULL, LOAD_None, NULL), 1, EItemFloor::FLOOR2);
    SpawnItemBoxes(47, "Shovels", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenShovel.InvenShovel"), NULL, LOAD_None, NULL), 1, EItemFloor::FLOOR2);
    SpawnItemBoxes(48, "GolfClub", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenGolfClub.InvenGolfClub"), NULL, LOAD_None, NULL), 1, EItemFloor::FLOOR2);
    SpawnItemBoxes(49, "WoodenBat", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenWoodenBat.InvenWoodenBat"), NULL, LOAD_None, NULL), 1, EItemFloor::FLOOR2);

    SpawnItemBoxes(50, "DirtyClothes", EItemClass::BLEEDINGHEALINGITEM, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenDirtyCloth.InvenDirtyCloth"), NULL, LOAD_None, NULL), 3, EItemFloor::FLOOR2);
    SpawnItemBoxes(51, "Clothes", EItemClass::BLEEDINGHEALINGITEM, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenCloth.InvenCloth"), NULL, LOAD_None, NULL), 3, EItemFloor::FLOOR2);
    SpawnItemBoxes(52, "Water", EItemClass::HEALINGITEM, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenWater.InvenWater"), NULL, LOAD_None, NULL), 2, EItemFloor::FLOOR2);
    SpawnItemBoxes(53, "Smoke", EItemClass::HEALINGITEM, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenSmoke.InvenSmoke"), NULL, LOAD_None, NULL), 2, EItemFloor::FLOOR2);
    SpawnItemBoxes(54, "Water", EItemClass::HEALINGITEM, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenWater.InvenWater"), NULL, LOAD_None, NULL), 2, EItemFloor::FLOOR2);
    SpawnItemBoxes(55, "Smoke", EItemClass::HEALINGITEM, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenSmoke.InvenSmoke"), NULL, LOAD_None, NULL), 2, EItemFloor::FLOOR2);
    SpawnItemBoxes(56, "BagActor", EItemClass::BAGITEM, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenBag.InvenBag"), NULL, LOAD_None, NULL), 1, EItemFloor::FLOOR2);
    SpawnItemBoxes(57, "BigBagActor", EItemClass::BAGITEM, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenBigBag.InvenBigBag"), NULL, LOAD_None, NULL), 1, EItemFloor::FLOOR2);

    SpawnItemBoxes(58, "CarKey3", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenCarKey2.InvenCarKey2"), NULL, LOAD_None, NULL), 10, EItemFloor::FLOOR2);
    SpawnItemBoxes(59, "CarKey4", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenCarKey2.InvenCarKey2"), NULL, LOAD_None, NULL), 20, EItemFloor::FLOOR2);

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
    TArray<FVector> OutVertices;
    ARecastNavMesh* NavMesh = Cast<ARecastNavMesh>(UNavigationSystemV1::GetCurrent(World)->MainNavData);
    if (NavMesh)
    {
        FRecastDebugGeometry NavMeshGeometry;
        NavMesh->GetDebugGeometry(NavMeshGeometry);
        //NavMesh->Get

        OutVertices.Append(NavMeshGeometry.MeshVerts);

        for (const FVector& Vertex : NavMeshGeometry.MeshVerts)
        {
            DrawDebugPoint(GetWorld(), Vertex, 10.0f, FColor::Red, false, -1.0f, 0);
        }

        FString FilePathB2 = FPaths::ProjectDir() + TEXT("B2.txt");
        FString FilePathB1 = FPaths::ProjectDir() + TEXT("B1.txt");
        FString FilePathF1 = FPaths::ProjectDir() + TEXT("F1.txt");
        FString FilePathF2 = FPaths::ProjectDir() + TEXT("F2.txt");
        FString FilePathF3 = FPaths::ProjectDir() + TEXT("F3.txt");


        FString NodeDataB2;
        FString NodeDataB1;
        FString NodeDataF1;
        FString NodeDataF2;
        FString NodeDataF3;

        for (const FVector& Node : OutVertices)
        {
            if (Node.Z < 800.f) {
                NodeDataB2 += FString::Printf(TEXT("%f,%f,%f\n"), Node.X, Node.Y, Node.Z);
            }
            else if (Node.Z < 1800.f) {
                NodeDataB1 += FString::Printf(TEXT("%f,%f,%f\n"), Node.X, Node.Y, Node.Z);
            }
            else if (Node.Z < 2500.f) {
                NodeDataF1 += FString::Printf(TEXT("%f,%f,%f\n"), Node.X, Node.Y, Node.Z);
            }
            else if (Node.Z < 3600.f) {
                NodeDataF2 += FString::Printf(TEXT("%f,%f,%f\n"), Node.X, Node.Y, Node.Z);
            }
            else {
                NodeDataF3 += FString::Printf(TEXT("%f,%f,%f\n"), Node.X, Node.Y, Node.Z);
            }
        }

        FFileHelper::SaveStringToFile(NodeDataB2, *FilePathB2);
        FFileHelper::SaveStringToFile(NodeDataB1, *FilePathB1);
        FFileHelper::SaveStringToFile(NodeDataF1, *FilePathF1);
        FFileHelper::SaveStringToFile(NodeDataF2, *FilePathF2);
        FFileHelper::SaveStringToFile(NodeDataF3, *FilePathF3);

    }

    //==========================================
    {
        // 현재 월드에서 네비게이션 시스템을 가져옵니다.
        UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
        if (NavSystem)
        {
            // 네비게이션 데이터 인스턴스를 가져옵니다.
            ARecastNavMesh* NavData = Cast<ARecastNavMesh>(NavSystem->GetDefaultNavDataInstance());
            if (NavData)
            {
                // NavData를 통해 NavMesh 세부 정보에 접근할 수 있습니다.
                //const TArray<FNavMeshTile>& Tiles = NavData->GetNavMeshTiles();
                // Tiles를 통해 폴리곤 및 정점 정보를 가져옵니다.
                
                
                
                FBox Tiles = NavData->GetNavMeshTileBounds(0);
            }
        }



    }
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

void AOneGameModeBase::SpawnCharacter(int32 characterindex)
{
    // 그 대신 이렇게 하면 PlayerStart 위치들을 따로 적어놔야 할것 같다. 코드상에서는 알 수 없으니까
    ABaseCharacter* DefaultPawn = nullptr;
    APlayerStart* PlayerStart = nullptr;

    DefaultPawn->SetPlayerId(characterindex);

    // 이름(Tag)으로 할 시
    FName PlayerStartTagName = "Start3";

    // 인덱스로 할 시
    // int index = 0;

    UWorld* World = GetWorld();
    if (World) {

        for (TActorIterator<APlayerStart> It(World); It; ++It) {
            APlayerStart* PS = *It;

            // 이름(Tag)으로 할 시
            if (PS && PS->PlayerStartTag == PlayerStartTagName) {

                PlayerStart = PS;
                break;
            }
            // 인덱스로 할 시
            //if (index == 3) {

            //    PlayerStart = PS;
            //    break;
            //}
            //index++;
        }
        for (TActorIterator<ABaseCharacter> ActorItr(World); ActorItr; ++ActorItr) {
            UE_LOG(LogTemp, Error, TEXT("DefaultPawn Complete"));
            DefaultPawn = *ActorItr;
            if (DefaultPawn) {
                break;
            }
        }

        if (DefaultPawn && PlayerStart) {
            UE_LOG(LogTemp, Error, TEXT("DefaultPawn && PlayerStart Complete"));
            DefaultPawn->SetActorLocation(PlayerStart->GetActorLocation());
        }
    }
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

//void AOneGameModeBase::SpawnZombiesStaticClasses()
//{
//
//    // 지하 1층
//    for (int i = 0; i < 10; ++i) {
//        ZombieClasses.Add(ANormalZombie::StaticClass());
//        ZombieAIClasses.Add(AZombieAIController::StaticClass());
//    }
//
//    // 지상 1층
//    for (int i = 10; i < 12; ++i) {
//        ZombieClasses.Add(AShoutingZombie::StaticClass());
//        ShoutingZombieAIClasses.Add(AShoutingZombieAIController::StaticClass());
//    }
//    for (int i = 12; i < 20; ++i) {
//        ZombieClasses.Add(ANormalZombie::StaticClass());
//        ZombieAIClasses.Add(AZombieAIController::StaticClass());
//    }
//
//    // 지상 2층
//    for (int i = 20; i < 23; ++i) {
//        ZombieClasses.Add(ARunningZombie::StaticClass());
//        RunningZombieAIClasses.Add(ARunningZombieAIController::StaticClass());
//    }
//    for (int i = 23; i < 30; ++i) {
//        ZombieClasses.Add(ANormalZombie::StaticClass());
//        ZombieAIClasses.Add(AZombieAIController::StaticClass());
//    }
//
//    // 지하 2층
//    for (int i = 30; i < 35; ++i) {
//        ZombieClasses.Add(ARunningZombie::StaticClass());
//        RunningZombieAIClasses.Add(ARunningZombieAIController::StaticClass());
//    }
//    for (int i = 35; i < 40; ++i) {
//        ZombieClasses.Add(ANormalZombie::StaticClass());
//        ZombieAIClasses.Add(AZombieAIController::StaticClass());
//    }
//}

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





void AOneGameModeBase::SpawnItemBoxes(int32 itemboxindex, FName itemname, EItemClass itemclass, UTexture2D* texture, int count, EItemFloor itemfloor)
{
    ItemBoxClasses.Add(AItemBoxActor::StaticClass());

    TSubclassOf<AItemBoxActor> SelectedItemBoxClass = ItemBoxClasses[itemboxindex];


    FVector itemboxpos = RandomItemBoxLocation(itemfloor);


    // 선택된 아이템 박스 클래스로 아이템 박스 생성
    AItemBoxActor* SpawnedItemBox = GetWorld()->SpawnActor<AItemBoxActor>(SelectedItemBoxClass, itemboxpos, FRotator::ZeroRotator);

    UE_LOG(LogTemp, Error, TEXT("ITEM___111"));
    if (SpawnedItemBox) {
        SpawnedItemBox->ItemName = itemname;
        SpawnedItemBox->ItemClassType = itemclass;
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
            BasePlayer->SetPlayerRun();
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





//void AOneGameModeBase::SpawnZombies(int32 zombieindex, EZombie zombieaiconindex, FVector zombiepos, FRotator zombieroatate, EZombiePatrol zombiepatrol, float patrolrange)
//{
//    TSubclassOf<ABaseZombie> SelectedZombieClass = ZombieClasses[zombieindex];
//
//
//    // 선택된 좀비 클래스로 좀비를 생성
//    ABaseZombie* SpawnedZombie = GetWorld()->SpawnActor<ABaseZombie>(SelectedZombieClass, zombiepos, zombieroatate);
//
//
//    UE_LOG(LogTemp, Error, TEXT("111111111111111111111"));
//    if (SpawnedZombie)
//    {
//        UE_LOG(LogTemp, Error, TEXT("2222222222222222222222222222222222222222"));
//        if (zombieaiconindex == EZombie::NORMAL) {
//
//            TSubclassOf<AZombieAIController> SelectedZombieAIClass = ZombieAIClasses[m_iZombieNumber];
//            AZombieAIController* AIZombieController = GetWorld()->SpawnActor<AZombieAIController>(SelectedZombieAIClass, FVector::ZeroVector, FRotator::ZeroRotator);
//
//            if (AIZombieController) {
//                SpawnedZombie->SpawnDefaultController();
//                AIZombieController->Possess(SpawnedZombie);
//                AIZombieController->SetStartLocationValue(zombiepos);
//
//                if (zombiepatrol == EZombiePatrol::PATROLX) {
//                    AIZombieController->SetPatrolLocationValue(FVector(zombiepos.X + patrolrange, zombiepos.Y, zombiepos.Z));
//                }
//                else if (zombiepatrol == EZombiePatrol::PATROLY) {
//                    AIZombieController->SetPatrolLocationValue(FVector(zombiepos.X, zombiepos.Y + patrolrange, zombiepos.Z));
//                }
//            }
//            else {
//                UE_LOG(LogTemp, Error, TEXT("SpawnedZombie is NULL"));
//            }
//
//            SpawnedZombie->SetZombieId(zombieindex);
//            m_iZombieNumber++;
//        }
//        else if (zombieaiconindex == EZombie::SHOUTING) {
//            UE_LOG(LogTemp, Error, TEXT("333333333333333333333333333333333333333333"));
//
//            TSubclassOf<AShoutingZombieAIController> SelectedShoutingZombieAIClass = ShoutingZombieAIClasses[m_iShoutingZombieNumber];
//            AShoutingZombieAIController* AIShoutingZombieController = GetWorld()->SpawnActor<AShoutingZombieAIController>(SelectedShoutingZombieAIClass, FVector::ZeroVector, FRotator::ZeroRotator);
//
//            if (AIShoutingZombieController) {
//                SpawnedZombie->SpawnDefaultController();
//                AIShoutingZombieController->Possess(SpawnedZombie);
//                AIShoutingZombieController->SetStartLocationValue(zombiepos);
//
//                if (zombiepatrol == EZombiePatrol::PATROLX) {
//                    AIShoutingZombieController->SetPatrolLocationValue(FVector(zombiepos.X + patrolrange, zombiepos.Y, zombiepos.Z));
//                }
//                else if (zombiepatrol == EZombiePatrol::PATROLY) {
//                    AIShoutingZombieController->SetPatrolLocationValue(FVector(zombiepos.X, zombiepos.Y + patrolrange, zombiepos.Z));
//                }
//            }
//            else {
//                UE_LOG(LogTemp, Error, TEXT("SpawnedZombie is NULL2"));
//            }
//
//            SpawnedZombie->SetZombieId(zombieindex);
//            m_iShoutingZombieNumber++;
//        }
//        else if (zombieaiconindex == EZombie::RUNNING) {
//            UE_LOG(LogTemp, Error, TEXT("4444444444444444444444444444444444444444444"));
//
//            TSubclassOf<ARunningZombieAIController> SelectedRunningZombieAIClass = RunningZombieAIClasses[m_iRunningZombieNumber];
//            ARunningZombieAIController* AIRunningZombieController = GetWorld()->SpawnActor<ARunningZombieAIController>(SelectedRunningZombieAIClass, FVector::ZeroVector, FRotator::ZeroRotator);
//
//            if (AIRunningZombieController) {
//                SpawnedZombie->SpawnDefaultController();
//                AIRunningZombieController->Possess(SpawnedZombie);
//                AIRunningZombieController->SetStartLocationValue(zombiepos);
//
//
//                if (zombiepatrol == EZombiePatrol::PATROLX) {
//                    AIRunningZombieController->SetPatrolLocationValue(FVector(zombiepos.X + patrolrange, zombiepos.Y, zombiepos.Z));
//                }
//                else if (zombiepatrol == EZombiePatrol::PATROLY) {
//                    AIRunningZombieController->SetPatrolLocationValue(FVector(zombiepos.X, zombiepos.Y + patrolrange, zombiepos.Z));
//                }
//            }
//            else {
//                UE_LOG(LogTemp, Error, TEXT("SpawnedZombie is NULL3"));
//            }
//
//            SpawnedZombie->SetZombieId(zombieindex);
//            m_iRunningZombieNumber++;
//        }
//
//        ZombieMap.Add(zombieindex, SpawnedZombie);
//    }
//}

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
    TSubclassOf<AZombieAIController> ZombieAIClass = nullptr;

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


    ABaseZombie** ZombiePtr = ZombieMap.Find(ZombieID);
    if (ZombiePtr && *ZombiePtr)
    {
        ABaseZombie* BaseZombie = *ZombiePtr;
        if (BaseZombie)
        {
            FVector OldLocation = BaseZombie->GetActorLocation();

            // 기존 캐릭터 위치 업데이트
            BaseZombie->SetActorLocation(NewLocation);
            BaseZombie->SetActorRotation(NewRotation);

            
            UE_LOG(LogTemp, Warning, TEXT("Updated Zombie ID: %d at new location"), ZombieID);
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

            AZombieAIController* AIZombieController = World->SpawnActor<AZombieAIController>(ZombieAIClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
            if (AIZombieController)
            {
                AIZombieController->Possess(NewZombie);
                UE_LOG(LogTemp, Warning, TEXT("Spawned and possessed new Zombie ID: %d"), ZombieID);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("Failed to spawn AI Controller for Zombie ID: %d"), ZombieID);
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to spawn new Zombie ID: %d"), ZombieID);
        }
        //UE_LOG(LogTemp, Warning, TEXT("No zombie found with ID: %d"), ZombieID);
    }
}

void AOneGameModeBase::UpdateZombieAttack(uint32 ZombieId)
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
            BaseZombie->Attack();
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