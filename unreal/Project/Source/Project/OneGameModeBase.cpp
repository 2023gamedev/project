// Fill out your copyright notice in the Description page of Project Settings.


#include "OneGameModeBase.h"
#include "BaseCharacter.h"
#include "BaseZombie.h"
#include "NormalZombie.h"
#include "ShoutingZombie.h"
#include "RunningZombie.h"
#include "RunningZombieAIController.h"
#include "ShoutingZombieAIController.h"
#include "ZombieAIController.h"
#include "Math/UnrealMathUtility.h"
#include "PlayerCharacterController.h"
#include "ItemBoxActor.h"



AOneGameModeBase::AOneGameModeBase()
{
	DefaultPawnClass = ABaseCharacter::StaticClass();
	PlayerControllerClass = APlayerCharacterController::StaticClass();

    
    // 변수들
    m_iItemBoxNumber = 0;
    
    m_iZombieNumber = 0;
    m_iShoutingZombieNumber = 0;
    m_iRunningZombieNumber = 0;

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

    SpawnItemBoxes(0, "SquareWood", FVector(2175.f, 2074.f, 30.0626f));
    SpawnItemBoxes(1, "SquareWood", FVector(2275.f, 2074.f, 30.0626f));
    SpawnItemBoxes(2, "SquareWood", FVector(2375.f, 2074.f, 30.0626f));

    // BeginPlay에서 SpawnZombies 호출
    SpawnZombies(0, EZombie::NORMAL, FVector(470.f,1320.f,88.f));
    SpawnZombies(1, EZombie::SHOUTING, FVector(470.f,1120.f,88.f));
    SpawnZombies(2, EZombie::RUNNING, FVector(470.f, 920.f, 88.f));

}

void AOneGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}


void AOneGameModeBase::SpawnItemBoxes(int32 itemboxindex, FString itemid, FVector itemboxpos) {

    TSubclassOf<AItemBoxActor> SelectedItemBoxClass = ItemBoxClasses[itemboxindex];

    // 선택된 아이템 박스 클래스로 아이템 박스 생성
    AItemBoxActor* SpawnedItemBox = GetWorld()->SpawnActor<AItemBoxActor>(SelectedItemBoxClass, itemboxpos, FRotator::ZeroRotator);
    
    UE_LOG(LogTemp, Error, TEXT("ITEM___111"));
    if (SpawnedItemBox) {
        UE_LOG(LogTemp, Error, TEXT("ITEM___222"));
        SpawnedItemBox->SetInBoxItemId(itemid);
    }
}

void AOneGameModeBase::SpawnZombies(int32 zombieindex, EZombie zombieaiconindex, FVector zombiepos)
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
            }
            else {
                UE_LOG(LogTemp, Error, TEXT("SpawnedZombie is NULL3"));
            }

            m_iRunningZombieNumber++;
        }

    }
}
