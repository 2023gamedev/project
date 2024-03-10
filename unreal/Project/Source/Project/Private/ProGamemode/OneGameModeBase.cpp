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
    ChoiceCharacter();

    ItemBoxClasses.Add(AItemBoxActor::StaticClass());
    ItemBoxClasses.Add(AItemBoxActor::StaticClass());
    ItemBoxClasses.Add(AItemBoxActor::StaticClass());

    ZombieClasses.Add(ANormalZombie::StaticClass());
    ZombieAIClasses.Add(AZombieAIController::StaticClass());

    ZombieClasses.Add(AShoutingZombie::StaticClass());
    ShoutingZombieAIClasses.Add(AShoutingZombieAIController::StaticClass());

    ZombieClasses.Add(ARunningZombie::StaticClass());
    RunningZombieAIClasses.Add(ARunningZombieAIController::StaticClass());


    

}

void AOneGameModeBase::BeginPlay()
{
    Super::BeginPlay();
                                                                                                                                                                    //테스트용으로 수 다르게 표시 나중에 변경
    SpawnItemBoxes(0, "SquareWood", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Engine/EngineResources/AICON-Red.AICON-Red"), NULL, LOAD_None, NULL), 1, FVector(2175.f, 2074.f, 30.0626f));
    SpawnItemBoxes(1, "SquareWood", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Engine/EngineResources/AICON-Red.AICON-Red"), NULL, LOAD_None, NULL), 10, FVector(2275.f, 2074.f, 30.0626f));
    SpawnItemBoxes(2, "SquareWood", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Engine/EngineResources/AICON-Red.AICON-Red"), NULL, LOAD_None, NULL), 20, FVector(2375.f, 2074.f, 30.0626f));

    SpawnCharacter(0);


    // BeginPlay에서 SpawnZombies 호출
    SpawnZombies(0, EZombie::NORMAL, FVector(400.f,1320.f, 90.212492f), true);
    SpawnZombies(1, EZombie::SHOUTING, FVector(470.f,1120.f, 90.212492f), true);
    SpawnZombies(2, EZombie::RUNNING, FVector(540.f, 920.f, 90.212492f), true);

}

void AOneGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
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

void AOneGameModeBase::SpawnItemBoxes(int32 itemboxindex, FName itemname, EItemClass itemclass, UTexture2D* texture, int count, FVector itemboxpos)
{
        TSubclassOf<AItemBoxActor> SelectedItemBoxClass = ItemBoxClasses[itemboxindex];
    
        // 선택된 아이템 박스 클래스로 아이템 박스 생성
        AItemBoxActor* SpawnedItemBox = GetWorld()->SpawnActor<AItemBoxActor>(SelectedItemBoxClass, itemboxpos, FRotator::ZeroRotator);
        
        UE_LOG(LogTemp, Error, TEXT("ITEM___111"));
        if (SpawnedItemBox) {
            SpawnedItemBox->ItemName = itemname;
            SpawnedItemBox->ItemClassType = itemclass;
            SpawnedItemBox->Texture = texture;
            SpawnedItemBox->Count = count;
        }
}

//void AOneGameModeBase::SpawnItemBoxes(int32 itemboxindex, FString itemid, FVector itemboxpos) {
//
//    TSubclassOf<AItemBoxActor> SelectedItemBoxClass = ItemBoxClasses[itemboxindex];
//
//    // 선택된 아이템 박스 클래스로 아이템 박스 생성
//    AItemBoxActor* SpawnedItemBox = GetWorld()->SpawnActor<AItemBoxActor>(SelectedItemBoxClass, itemboxpos, FRotator::ZeroRotator);
//    
//    UE_LOG(LogTemp, Error, TEXT("ITEM___111"));
//    if (SpawnedItemBox) {
//        UE_LOG(LogTemp, Error, TEXT("ITEM___222"));
//        //SpawnedItemBox->SetInBoxItemId(itemid);
//    }
//}

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
