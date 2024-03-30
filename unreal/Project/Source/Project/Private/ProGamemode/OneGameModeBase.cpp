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

    
    // ������
    m_iItemBoxNumber            = 0;
    
    m_iZombieNumber             = 0;
    m_iShoutingZombieNumber     = 0;
    m_iRunningZombieNumber      = 0;
    m_fPatrolRange              = 500.f;

    // �÷��̾ ������ ���� �޾Ƽ� �Ѱ��ַ��� �� ����
    CharacterIconIndex = EPlayerCharacter::EMPLOYEE;
    ChoiceCharacter();

    // �����۹ڽ��� ���߿� �����ǰų� ������ �� �ֱ⿡ ����� ���� �ȵȴ�.
    //ItemBoxClasses.Add(AItemBoxActor::StaticClass());
    //ItemBoxClasses.Add(AItemBoxActor::StaticClass());
    //ItemBoxClasses.Add(AItemBoxActor::StaticClass());

    // ����� ��� ��Ȱ�ϱ⿡ �� �����ϰų� ������ ������ ���� �� ������ Add�����ϴ�.
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
                                                                                                                                                                    //�׽�Ʈ������ �� �ٸ��� ǥ�� ���߿� ����
    SpawnItemBoxes(0, "SquareWood", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Engine/EngineResources/AICON-Red.AICON-Red"), NULL, LOAD_None, NULL), 1, FVector(2175.f, 2074.f, 30.0626f));
    SpawnItemBoxes(1, "SquareWood", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Engine/EngineResources/AICON-Red.AICON-Red"), NULL, LOAD_None, NULL), 10, FVector(2275.f, 2074.f, 30.0626f));
    SpawnItemBoxes(2, "SquareWood", EItemClass::NORMALWEAPON, LoadObject<UTexture2D>(NULL, TEXT("/Engine/EngineResources/AICON-Red.AICON-Red"), NULL, LOAD_None, NULL), 20, FVector(2375.f, 2074.f, 30.0626f));

    //SpawnCharacter(0);

 

    // BeginPlay���� SpawnZombies ȣ��
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
    // �� ��� �̷��� �ϸ� PlayerStart ��ġ���� ���� ������� �Ұ� ����. �ڵ�󿡼��� �� �� �����ϱ�
    ABaseCharacter* DefaultPawn = nullptr;
    APlayerStart* PlayerStart = nullptr;

    DefaultPawn->SetPlayerId(99);

    // �̸�(Tag)���� �� ��
    FName PlayerStartTagName = "Start3";

    // �ε����� �� ��
    // int index = 0;

    UWorld* World = GetWorld();
    if (World) {

        for (TActorIterator<APlayerStart> It(World); It; ++It) { 
            APlayerStart* PS = *It;

            // �̸�(Tag)���� �� ��
            if (PS && PS->PlayerStartTag == PlayerStartTagName) { 
 
                PlayerStart = PS;
                break;
            }
            // �ε����� �� ��
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

	// ���õ� ������ �ڽ� Ŭ������ ������ �ڽ� ����
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

    // �Ű��������� ������ϱ� ���� ���
    //FString ItemNameString = itemname.ToString();
    //FString ItemClassString = UEnum::GetValueAsString(itemclass); // EItemClass�� ���ڿ��� ��ȯ
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

    // ĳ���� �˻�
    for (TActorIterator<ABaseCharacter> It(World); It; ++It)
    {
        ABaseCharacter* BasePlayer = *It;
        if (BasePlayer && BasePlayer->GetPlayerId() == PlayerID)
        {
            FVector OldLocation = BasePlayer->GetActorLocation();

            // ���� ĳ���� ��ġ ������Ʈ
            BasePlayer->SetActorLocation(NewLocation);
            BasePlayer->SetActorRotation(NewRotation);

            BasePlayer->UpdatePlayerData(NewLocation);

            return; // ��ġ�� ������Ʈ �����Ƿ� �Լ� ����
        }
    }
    
    //���� ĳ���͸� ã�� ���� ��쿡�� �� ĳ���� ����
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    ABaseCharacter* NewCharacter = World->SpawnActor<ABaseCharacter>(DefaultPawnClass, NewLocation, NewRotation, SpawnParams);

    if (NewCharacter)
    {
        // �� ĳ���Ϳ� PlayerId ����
        NewCharacter->SetPlayerId(PlayerID);
    }
}



void AOneGameModeBase::SpawnZombies(int32 zombieindex, EZombie zombieaiconindex, FVector zombiepos, bool ispatrol)
{

    TSubclassOf<ABaseZombie> SelectedZombieClass = ZombieClasses[zombieindex];


    // ���õ� ���� Ŭ������ ���� ����
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

