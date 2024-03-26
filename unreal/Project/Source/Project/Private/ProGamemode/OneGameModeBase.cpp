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
#include "EngineUtils.h"
#include "GameFramework/DefaultPawn.h"
#include "GameFramework/PlayerStart.h"
#include "ProGamemode/GameModeManager.h"
#include "ProGamemode/LobbyGameMode.h"
#include "ProGamemode/ProGameInstance.h"
#include "Math/UnrealMathUtility.h"

AOneGameModeBase::AOneGameModeBase()
{
	//DefaultPawnClass = ABaseCharacter::StaticClass();
	//PlayerControllerClass = APlayerCharacterController::StaticClass();

    
    // 변수들
    m_iItemBoxNumber            = 0;
    
    m_iZombieNumber             = 0;
    m_iShoutingZombieNumber     = 0;
    m_iRunningZombieNumber      = 0;
    m_fPatrolRange              = 500.f;

    // 플레이어를 선택한 것을 받아서 넘겨주려고 할 예정
    CharacterIconIndex = EPlayerCharacter::EMPLOYEE;

    // 좀비는 계속 부활하기에 더 생성하거나 삭제할 이유가 없어 이 곳에서 Add가능하다.
    ZombieClasses.Add(ANormalZombie::StaticClass());
    ZombieAIClasses.Add(AZombieAIController::StaticClass());

    ZombieClasses.Add(AShoutingZombie::StaticClass());
    ShoutingZombieAIClasses.Add(AShoutingZombieAIController::StaticClass());

    ZombieClasses.Add(ARunningZombie::StaticClass());
    RunningZombieAIClasses.Add(ARunningZombieAIController::StaticClass());


    // 0~19은 B1 20~39는 F1 40~59는 F2
// B1
    ItemRandomLocationStruct[0].sLocation = FVector(2175.f, 2074.f, 30.0626f);
    ItemRandomLocationStruct[1].sLocation = FVector(2275.f, 2074.f, 30.0626f);
    ItemRandomLocationStruct[2].sLocation = FVector(2375.f, 2074.f, 30.0626f);

    ItemRandomLocationStruct[3].sLocation = FVector(2175.f, 2774.f, 30.0626f);
    ItemRandomLocationStruct[4].sLocation = FVector(2275.f, 2974.f, 30.0626f);
    ItemRandomLocationStruct[5].sLocation = FVector(2375.f, 3174.f, 30.0626f);

    ItemRandomLocationStruct[6].sLocation = FVector(2175.f, 2574.f, 30.0626f);
    ItemRandomLocationStruct[7].sLocation = FVector(2275.f, 2874.f, 30.0626f);
    ItemRandomLocationStruct[8].sLocation = FVector(2375.f, 1074.f, 30.0626f);

    ItemRandomLocationStruct[9].sLocation = FVector(2105.f, 1374.f, 30.0626f);
    ItemRandomLocationStruct[10].sLocation = FVector(2075.f, 1574.f, 30.0626f);
    ItemRandomLocationStruct[11].sLocation = FVector(2175.f, 3574.f, 30.0626f);

    ItemRandomLocationStruct[12].sLocation = FVector(2105.f, 1374.f, 30.0626f);
    ItemRandomLocationStruct[13].sLocation = FVector(2075.f, 1574.f, 30.0626f);
    ItemRandomLocationStruct[14].sLocation = FVector(2175.f, 3574.f, 30.0626f);

    ItemRandomLocationStruct[15].sLocation = FVector(2105.f, 1374.f, 30.0626f);
    ItemRandomLocationStruct[16].sLocation = FVector(2075.f, 1574.f, 30.0626f);
    ItemRandomLocationStruct[17].sLocation = FVector(2175.f, 3574.f, 30.0626f);

    ItemRandomLocationStruct[18].sLocation = FVector(2975.f, 2074.f, 30.0626f);
    ItemRandomLocationStruct[19].sLocation = FVector(2675.f, 2074.f, 30.0626f);


    // F1
    ItemRandomLocationStruct[20].sLocation = FVector(2175.f, 2074.f, 300.0626f);
    ItemRandomLocationStruct[21].sLocation = FVector(2275.f, 2074.f, 300.0626f);
    ItemRandomLocationStruct[22].sLocation = FVector(2375.f, 2074.f, 300.0626f);

    ItemRandomLocationStruct[23].sLocation = FVector(2175.f, 2774.f, 300.0626f);
    ItemRandomLocationStruct[24].sLocation = FVector(2275.f, 2974.f, 300.0626f);
    ItemRandomLocationStruct[25].sLocation = FVector(2375.f, 3174.f, 300.0626f);

    ItemRandomLocationStruct[26].sLocation = FVector(2175.f, 2574.f, 300.0626f);
    ItemRandomLocationStruct[27].sLocation = FVector(2275.f, 2874.f, 300.0626f);
    ItemRandomLocationStruct[28].sLocation = FVector(2375.f, 1074.f, 300.0626f);

    ItemRandomLocationStruct[29].sLocation = FVector(2105.f, 1374.f, 300.0626f);
    ItemRandomLocationStruct[30].sLocation = FVector(2075.f, 1574.f, 300.0626f);
    ItemRandomLocationStruct[31].sLocation = FVector(2175.f, 3574.f, 300.0626f);

    ItemRandomLocationStruct[32].sLocation = FVector(2105.f, 1374.f, 300.0626f);
    ItemRandomLocationStruct[33].sLocation = FVector(2075.f, 1574.f, 300.0626f);
    ItemRandomLocationStruct[34].sLocation = FVector(2175.f, 3574.f, 300.0626f);

    ItemRandomLocationStruct[35].sLocation = FVector(2105.f, 1374.f, 300.0626f);
    ItemRandomLocationStruct[36].sLocation = FVector(2075.f, 1574.f, 300.0626f);
    ItemRandomLocationStruct[37].sLocation = FVector(2175.f, 3574.f, 300.0626f);

    ItemRandomLocationStruct[38].sLocation = FVector(2975.f, 2074.f, 300.0626f);
    ItemRandomLocationStruct[39].sLocation = FVector(2675.f, 2074.f, 300.0626f);

    // F2
    ItemRandomLocationStruct[40].sLocation = FVector(2175.f, 2074.f, 600.0626f);
    ItemRandomLocationStruct[41].sLocation = FVector(2275.f, 2074.f, 600.0626f);
    ItemRandomLocationStruct[42].sLocation = FVector(2375.f, 2074.f, 600.0626f);

    ItemRandomLocationStruct[43].sLocation = FVector(2175.f, 2774.f, 600.0626f);
    ItemRandomLocationStruct[44].sLocation = FVector(2275.f, 2974.f, 600.0626f);
    ItemRandomLocationStruct[45].sLocation = FVector(2375.f, 3174.f, 600.0626f);

    ItemRandomLocationStruct[46].sLocation = FVector(2175.f, 2574.f, 600.0626f);
    ItemRandomLocationStruct[47].sLocation = FVector(2275.f, 2874.f, 600.0626f);
    ItemRandomLocationStruct[48].sLocation = FVector(2375.f, 1074.f, 600.0626f);

    ItemRandomLocationStruct[49].sLocation = FVector(2105.f, 1374.f, 600.0626f);
    ItemRandomLocationStruct[50].sLocation = FVector(2075.f, 1574.f, 600.0626f);
    ItemRandomLocationStruct[51].sLocation = FVector(2175.f, 3574.f, 600.0626f);

    ItemRandomLocationStruct[52].sLocation = FVector(2105.f, 1374.f, 600.0626f);
    ItemRandomLocationStruct[53].sLocation = FVector(2075.f, 1574.f, 600.0626f);
    ItemRandomLocationStruct[54].sLocation = FVector(2175.f, 3574.f, 600.0626f);

    ItemRandomLocationStruct[55].sLocation = FVector(2105.f, 1374.f, 600.0626f);
    ItemRandomLocationStruct[56].sLocation = FVector(2075.f, 1574.f, 600.0626f);
    ItemRandomLocationStruct[57].sLocation = FVector(2175.f, 3574.f, 600.0626f);

    ItemRandomLocationStruct[58].sLocation = FVector(2975.f, 2074.f, 600.0626f);
    ItemRandomLocationStruct[59].sLocation = FVector(2675.f, 2074.f, 600.0626f);

    for (int i = 0; i < 60; ++i) {
        ItemRandomLocationStruct[i].bIsSeatLocation = false;
    }

}

void AOneGameModeBase::BeginPlay()
{
    Super::BeginPlay();
                                                                                                                                                                    //테스트용으로 수 다르게 표시 나중에 변경
    SpawnItemBoxes(0, "SquareWood", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Engine/EngineResources/AICON-Red.AICON-Red"), NULL, LOAD_None, NULL), 1,  EItemFloor::FLOORB1);
    SpawnItemBoxes(1, "SquareWood", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Engine/EngineResources/AICON-Red.AICON-Red"), NULL, LOAD_None, NULL), 10, EItemFloor::FLOOR1);
    SpawnItemBoxes(2, "SquareWood", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Engine/EngineResources/AICON-Red.AICON-Red"), NULL, LOAD_None, NULL), 20, EItemFloor::FLOOR2);

    SpawnCharacter(0);


    // BeginPlay에서 SpawnZombies 호출
    SpawnZombies(0, EZombie::NORMAL, FVector(400.f,1320.f, 90.212492f), true);
    SpawnZombies(1, EZombie::SHOUTING, FVector(470.f,1120.f, 90.212492f), true);
    SpawnZombies(2, EZombie::RUNNING, FVector(540.f, 920.f, 90.212492f), true);

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



void AOneGameModeBase::SpawnZombies(int32 zombieindex, EZombie zombieaiconindex, FVector zombiepos, bool ispatrol)
{

    TSubclassOf<ABaseZombie> SelectedZombieClass = ZombieClasses[zombieindex];


    // 선택된 좀비 클래스로 좀비를 생성
    ABaseZombie* SpawnedZombie = GetWorld()->SpawnActor<ABaseZombie>(SelectedZombieClass, zombiepos, FRotator::ZeroRotator);
 

    UE_LOG(LogTemp, Error, TEXT("111111111111111111111"));
    if (SpawnedZombie)
    {
        UE_LOG(LogTemp, Error, TEXT("2222222222222222222222222222222222222222"));
        if (zombieaiconindex == EZombie::NORMAL) {

            TSubclassOf<AZombieAIController> SelectedZombieAIClass = ZombieAIClasses[m_iZombieNumber];
            AZombieAIController* AIZombieController = GetWorld()->SpawnActor<AZombieAIController>(SelectedZombieAIClass, FVector::ZeroVector, FRotator::ZeroRotator);

            if (AIZombieController) {
                SpawnedZombie->SpawnDefaultController();
                AIZombieController->Possess(SpawnedZombie);
                AIZombieController->SetStartLocationValue(zombiepos);

                if (ispatrol) {
                    AIZombieController->SetPatrolLocationValue(FVector(zombiepos.X, zombiepos.Y + m_fPatrolRange, zombiepos.Z));
                }
                
            }
            else {
                UE_LOG(LogTemp, Error, TEXT("SpawnedZombie is NULL"));
            }

            m_iZombieNumber++;
        }
        else if (zombieaiconindex == EZombie::SHOUTING) {
            UE_LOG(LogTemp, Error, TEXT("333333333333333333333333333333333333333333"));

            TSubclassOf<AShoutingZombieAIController> SelectedShoutingZombieAIClass = ShoutingZombieAIClasses[m_iShoutingZombieNumber];
            AShoutingZombieAIController* AIShoutingZombieController = GetWorld()->SpawnActor<AShoutingZombieAIController>(SelectedShoutingZombieAIClass, FVector::ZeroVector, FRotator::ZeroRotator);
            
            if (AIShoutingZombieController) {
                SpawnedZombie->SpawnDefaultController();
                AIShoutingZombieController->Possess(SpawnedZombie);
                AIShoutingZombieController->SetStartLocationValue(zombiepos);

                if (ispatrol) {
                    AIShoutingZombieController->SetPatrolLocationValue(FVector(zombiepos.X, zombiepos.Y + m_fPatrolRange, zombiepos.Z));
                }
            }
            else {
            UE_LOG(LogTemp, Error, TEXT("SpawnedZombie is NULL2"));
            }

            m_iShoutingZombieNumber++;
        }
        else if (zombieaiconindex == EZombie::RUNNING) {
            UE_LOG(LogTemp, Error, TEXT("4444444444444444444444444444444444444444444"));

            TSubclassOf<ARunningZombieAIController> SelectedRunningZombieAIClass = RunningZombieAIClasses[m_iRunningZombieNumber];
            ARunningZombieAIController* AIRunningZombieController = GetWorld()->SpawnActor<ARunningZombieAIController>(SelectedRunningZombieAIClass, FVector::ZeroVector, FRotator::ZeroRotator);

            if (AIRunningZombieController) {
                SpawnedZombie->SpawnDefaultController();
                AIRunningZombieController->Possess(SpawnedZombie);
                AIRunningZombieController->SetStartLocationValue(zombiepos);


                if (ispatrol) {
                    AIRunningZombieController->SetPatrolLocationValue(FVector(zombiepos.X, zombiepos.Y + m_fPatrolRange, zombiepos.Z));
                }
            }
            else {
                UE_LOG(LogTemp, Error, TEXT("SpawnedZombie is NULL3"));
            }

            m_iRunningZombieNumber++;
        }

    }
}

