// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProData/ItemDataStructure.h"
#include "OneGameModeBase.generated.h"

/**
 *
 */

 // Main Gamemode 클래스


UENUM(BlueprintType)
enum class EZombie
{
    NORMAL,
    SHOUTING,
    RUNNING,
};

// Zombie는 패트롤을 어느쪽으로 할 것인가
UENUM(BlueprintType)
enum class EZombiePatrol
{
    NOTPATROL,
    PATROLX,
    PATROLY,
};

// 캐릭터 선택을 위한 enum 클래스
UENUM(BlueprintType)
enum class EPlayerCharacter
{
    GIRL,
    EMPLOYEE,
    IDOL,
    FIREFIGHTER,
};

UENUM(BlueprintType)
enum class EItemFloor
{
    FLOORB1, // 지하 1층
    FLOOR1,  // 1층
    FLOOR2,  // 2층
};



class ABaseCharacter;
class ABaseZombie;
class APlayerCharacterController;
class AZombieAIController;
class AShoutingZombieAIController;
class ARunningZombieAIController;
class AItemBoxActor;
class AInterActor;

extern TMap<uint32, ABaseZombie*> ZombieMap;

UCLASS()
class PROJECT_API AOneGameModeBase : public AGameModeBase
{
    GENERATED_BODY()
public:
    AOneGameModeBase();

    virtual void PostLogin(APlayerController* NewPlayer) override;

    virtual void BeginPlay() override;
    virtual void BeginDestroy() override;

    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;


    // 아이템 박스
    UPROPERTY(EditDefaultsOnly, Category = "ItemBox")
    TArray<TSubclassOf<AItemBoxActor>> ItemBoxClasses;

    UPROPERTY(VisibleAnywhere, Category = "ItemBox")
    int32 m_iItemBoxNumber;


    // 캐릭터를 저장할 배열
    UPROPERTY(EditDefaultsOnly, Category = "PlayerCharacter")
    TArray<TSubclassOf<ABaseCharacter>> PlayerCharacterClasses;

    UPROPERTY(EditDefaultsOnly, Category = "PlayerCharacter")
    TArray<TSubclassOf<APlayerCharacterController>> PlayerCharacterControllerClasses;

    UPROPERTY(EditDefaultsOnly, Category = "OtherCharacter")
    TArray<TSubclassOf<ABaseCharacter>> OtherCharacterClasses;

    // 다양한 종류의 좀비 클래스를 저장할 배열
    UPROPERTY(EditDefaultsOnly, Category = "Zombies")
    TArray<TSubclassOf<ABaseZombie>> ZombieClasses;


    UPROPERTY(EditDefaultsOnly, Category = "Zombies")
    TArray<TSubclassOf<AZombieAIController>> ZombieAIClasses;

    UPROPERTY(EditDefaultsOnly, Category = "Zombies")
    TArray<TSubclassOf<AShoutingZombieAIController>> ShoutingZombieAIClasses;

    UPROPERTY(EditDefaultsOnly, Category = "Zombies")
    TArray<TSubclassOf<ARunningZombieAIController>> RunningZombieAIClasses;

    UPROPERTY(VisibleAnywhere, Category = "Zombies")
    int32 m_iZombieNumber;

    UPROPERTY(VisibleAnywhere, Category = "Zombies")
    int32 m_iShoutingZombieNumber;

    UPROPERTY(VisibleAnywhere, Category = "Zombies")
    int32 m_iRunningZombieNumber;

    // InterActor 배열 
    UPROPERTY(EditDefaultsOnly, Category = "Inter")
    TArray<TSubclassOf<AInterActor>> InterActorClasses;



    UPROPERTY(VisibleAnywhere, Category = "PlayerCharacter")
    EPlayerCharacter CharacterIconIndex;


    struct SItemRandomLocation
    {
        FVector sLocation;                  // 위치가 어딘가
        bool    bIsSeatLocation = false;    // 위치에 누가 이미 차지하고 있는가 true 
    };

    struct SCarActorRandomLocation
    {
        FVector sLocation;
        FRotator sRotation;
        bool bIsSeatLocation = false;
    };

    struct SCarKeyRandom
    {
        FName CarKeyName;
        bool bIsSeatCarKey = false;
    };

    void ChoiceCharacterBefore();

    // 캐릭터 선택 함수
    void ChoiceCharacter();

    void SpawnCharacter(int32 characterindex);

    // 아이템 박스 위치 정해놓고 랜덤으로 정해서 생성
    FVector RandomItemBoxLocation(EItemFloor itemfloor);

    // 카 액터 위치 정해놓고 랜덤으로 정해서 생성
    int32 RandomCarActorLocation();

    int32 RandomCarKey();

    // 아이템 랜덤 위치 정해놓는 배열
    SItemRandomLocation ItemRandomLocationStruct[60];

    // Car 랜덤 위치 정해놓는 배열
    SCarActorRandomLocation CarActorRandomLocationStruct[7];

    SCarKeyRandom CarKeyRandom[7];

    // 아이템 생성 함수
    void ItemRandomLocationSetting();
    void SpawnItemBoxes(int32 itemboxindex, FName itemname, EItemClass itemclass, UTexture2D* texture, int count, EItemFloor itemfloor);

    // 좀비 생성 함수
    void SpawnZombiesStaticClasses();
    //void SpawnZombies(int32 zombieindex, EZombie zombieaiconindex, FVector zombiepos, FRotator zombieroatate, EZombiePatrol zombiepatrol, float patrolrange);

    void UpdateZombie(uint32 ZombieID, uint32 ZombieType, FVector NewLocation, FRotator NewRotation);

    void UpdateZombieAttack(uint32 ZombieId);

    void UpdateZombieHP(uint32 ZombieId, uint32 HP);

    void DestroyItem(uint32 ItemId);

    int32 GetItemBoxNumber() { return m_iItemBoxNumber; }

    void SpawnOnGroundItem(FName itemname, EItemClass itemclass, UTexture2D* texture, int count);

    void CarActorRandomLocationSetting();

    void CarKeyRandomSetting();

    void SpawnInterItem(int32 InterActorindex, FName InterName);
    void SpawnInterActorStaticClasses();


    void UpdateOtherPlayer(uint32 PlayerID, FVector NewLocation, FRotator NewRotation, uint32 charactertype, float hp);

    void UpdatePlayerAttack(uint32 PlayerID, bool battack);

    void UpdateEquipItem(uint32 PlayerID, const FString& Itemname, uint32 itemtype);

    void UpdatePlayerRun(uint32 PlayerID, bool b_run);

    void UpdatePlayerJump(uint32 PlayerID);

public:

    uint32 my_class_num;
};
