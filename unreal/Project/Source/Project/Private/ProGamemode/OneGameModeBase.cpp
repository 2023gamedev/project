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
#include "ClientSocket.h"
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

    // 아이템박스는 도중에 생성되거나 없어질 수 있기에 여기다 쓰면 안된다.
    //ItemBoxClasses.Add(AItemBoxActor::StaticClass());
    //ItemBoxClasses.Add(AItemBoxActor::StaticClass());
    //ItemBoxClasses.Add(AItemBoxActor::StaticClass());

    // 좀비는 계속 부활하기에 더 생성하거나 삭제할 이유가 없어 이 곳에서 Add가능하다.
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

    //SpawnCharacter(0);

 

    // BeginPlay에서 SpawnZombies 호출
    //SpawnZombies(0, EZombie::NORMAL, FVector(400.f,1320.f, 90.212492f), true);
    //SpawnZombies(1, EZombie::SHOUTING, FVector(470.f,1120.f, 90.212492f), true);
    //SpawnZombies(2, EZombie::RUNNING, FVector(540.f, 920.f, 90.212492f), true);

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

    DefaultPawn->SetPlayerId(99);

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
    ItemBoxClasses.Add(AItemBoxActor::StaticClass());

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

    // 매개변수들을 디버깅하기 위해 출력
    //FString ItemNameString = itemname.ToString();
    //FString ItemClassString = UEnum::GetValueAsString(itemclass); // EItemClass의 문자열로 변환
    //FString TextureName = texture ? texture->GetName() : TEXT("NULL");
    //FString DefaultPawnLocation = DefaultPawn->GetActorLocation().ToString();

    //FString DebugMessage = FString::Printf(TEXT("Item Name: %s, Item Class: %s, Texture: %s, Count: %d, Location %s, index %d"), *ItemNameString, *ItemClassString, *TextureName, count, *DefaultPawnLocation, GetItemBoxNumber());
    //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, DebugMessage);

    


    FVector DropPos = DefaultPawn->GetActorForwardVector() * 100.f;
    SpawnItemBoxes(GetItemBoxNumber(), itemname, itemclass, texture, count, DefaultPawn->GetActorLocation() + FVector(DropPos.X, DropPos.Y, -60.149886f) );
}

void AOneGameModeBase::UpdateOtherPlayer(uint32 PlayerID, FVector NewLocation, FRotator NewRotation)
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

            return; // 위치를 업데이트 했으므로 함수 종료
        }
    }
    
    //기존 캐릭터를 찾지 못한 경우에만 새 캐릭터 스폰
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    ABaseCharacter* NewCharacter = World->SpawnActor<ABaseCharacter>(DefaultPawnClass, NewLocation, NewRotation, SpawnParams);

    if (NewCharacter)
    {
        // 새 캐릭터에 PlayerId 설정
        NewCharacter->SetPlayerId(PlayerID);
    }
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

