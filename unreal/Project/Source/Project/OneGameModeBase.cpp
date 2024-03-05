// Fill out your copyright notice in the Description page of Project Settings.


#include "OneGameModeBase.h"
#include "BaseCharacter.h"
#include "EmployeeCharacter.h"
#include "FireFighterCharacter.h"
#include "GirlCharacter.h"
#include "IdolCharacter.h"
#include "PlayerCharacterController.h"
#include "BaseZombie.h"
#include "NormalZombie.h"
#include "ShoutingZombie.h"
#include "RunningZombie.h"
#include "RunningZombieAIController.h"
#include "ShoutingZombieAIController.h"
#include "ZombieAIController.h"
#include "Math/UnrealMathUtility.h"
#include "ItemBoxActor.h"


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

    SpawnCharacter(0, FVector(2215.f, 2282.f, 90.212492f));


    // BeginPlay에서 SpawnZombies 호출
    SpawnZombies(0, EZombie::NORMAL, FVector(400.f,1320.f, 90.212492f), true);
    SpawnZombies(1, EZombie::SHOUTING, FVector(470.f,1120.f, 90.212492f), true);
    SpawnZombies(2, EZombie::RUNNING, FVector(540.f, 920.f, 90.212492f), true);

}

void AOneGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

//void AOneGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
//{
//    Super::InitGame(MapName, Options, ErrorMessage);
//
//    // Create a local player and assign a player controller
//    ULocalPlayer* LocalPlayer = UGameplayStatics::CreateLocalPlayer(GetWorld(), 0, true);
//    if (LocalPlayer)
//    {
//        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
//        if (PlayerController)
//        {
//            // Set the player controller's input mode, viewport, etc.
//            // 이곳에서 로컬 플레이어의 컨트롤러 설정을 수행합니다.
//            // 예를 들어 입력 모드 설정, 뷰포트 설정 등이 여기에 포함될 수 있습니다.
//        }
//        else
//        {
//            ErrorMessage = "Failed to get player controller!";
//        }
//    }
//    else
//    {
//        ErrorMessage = "Failed to create local player!";
//    }
//}

void AOneGameModeBase::ChoiceCharacter()
{
        if (CharacterIconIndex == EPlayerCharacter::EMPLOYEE) {
            //DefaultPawnClass = AEmployeeCharacter::StaticClass();
            PlayerCharacterClasses.Add(AEmployeeCharacter::StaticClass());
        }
        else if (CharacterIconIndex == EPlayerCharacter::FIREFIGHTER) {
            //DefaultPawnClass = AFireFighterCharacter::StaticClass();
            PlayerCharacterClasses.Add(AFireFighterCharacter::StaticClass());
        }
        else if (CharacterIconIndex == EPlayerCharacter::GIRL) {
            //DefaultPawnClass = AGirlCharacter::StaticClass();
            PlayerCharacterClasses.Add(AGirlCharacter::StaticClass());
        }
        else if (CharacterIconIndex == EPlayerCharacter::IDOL) {
            //DefaultPawnClass = AIdolCharacter::StaticClass();
            PlayerCharacterClasses.Add(AIdolCharacter::StaticClass());
        }


        //PlayerControllerClass = APlayerCharacterController::StaticClass();
        PlayerCharacterControllerClasses.Add(APlayerCharacterController::StaticClass());

        DefaultPawnClass = PlayerCharacterClasses[0];
        PlayerControllerClass = PlayerCharacterControllerClasses[0];

}

void AOneGameModeBase::SpawnCharacter(int32 characterindex, FVector characterpos)
{
    //TSubclassOf<ABaseCharacter> SelectedCharacterClass = PlayerCharacterClasses[characterindex];
    //ABaseCharacter* SpawnedCharacter = GetWorld()->SpawnActor<ABaseCharacter>(SelectedCharacterClass, characterpos, FRotator::ZeroRotator);
    //TSubclassOf<APlayerCharacterController> SelectedPlayerConstrollerClass = PlayerCharacterControllerClasses[0]; // 0을 변수를 바꿀수도 있다.
    //APlayerCharacterController* AICharacterController = GetWorld()->SpawnActor<APlayerCharacterController>(SelectedPlayerConstrollerClass, characterpos, FRotator::ZeroRotator);

    //SpawnedCharacter->SpawnDefaultController();
    //AICharacterController->Possess(SpawnedCharacter);
   
    //if (AICharacterController->GetPawn() == nullptr) {
    //    UE_LOG(LogTemp, Error, TEXT("Possess No!!!!"));
    //}
    //else {
    //    FString PossessedPawnName = AICharacterController->GetPawn()->GetName();
    //    UE_LOG(LogTemp, Warning, TEXT("Controller is possessing: %s"), *PossessedPawnName);
    //}

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
