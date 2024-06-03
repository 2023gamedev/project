// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProData/ItemDataStructure.h"
#include "OneGameModeBase.generated.h"

/**
 *
 */

 // Main Gamemode Ŭ����


UENUM(BlueprintType)
enum class EZombie
{
    NORMAL,
    SHOUTING,
    RUNNING,
};

// Zombie�� ��Ʈ���� ��������� �� ���ΰ�
UENUM(BlueprintType)
enum class EZombiePatrol
{
    NOTPATROL,
    PATROLX,
    PATROLY,
};

// ĳ���� ������ ���� enum Ŭ����
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
    FLOORB1, // ���� 1��
    FLOOR1,  // 1��
    FLOOR2,  // 2��
};



class ABaseCharacter;
class ABaseZombie;
class APlayerCharacterController;
class AZombieAIController;
class AShoutingZombieAIController;
class ARunningZombieAIController;
class AItemBoxActor;
class AInterActor;

UCLASS()
class PROJECT_API AOneGameModeBase : public AGameModeBase
{
    GENERATED_BODY()
public:
    AOneGameModeBase();

    virtual void PostLogin(APlayerController* NewPlayer) override;

    virtual void BeginPlay() override;

    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;


    // ������ �ڽ�
    UPROPERTY(EditDefaultsOnly, Category = "ItemBox")
    TArray<TSubclassOf<AItemBoxActor>> ItemBoxClasses;

    UPROPERTY(VisibleAnywhere, Category = "ItemBox")
    int32 m_iItemBoxNumber;


    // ĳ���͸� ������ �迭
    UPROPERTY(EditDefaultsOnly, Category = "PlayerCharacter")
    TArray<TSubclassOf<ABaseCharacter>> PlayerCharacterClasses;

    UPROPERTY(EditDefaultsOnly, Category = "PlayerCharacter")
    TArray<TSubclassOf<APlayerCharacterController>> PlayerCharacterControllerClasses;

    UPROPERTY(EditDefaultsOnly, Category = "OtherCharacter")
    TArray<TSubclassOf<ABaseCharacter>> OtherCharacterClasses;

    // �پ��� ������ ���� Ŭ������ ������ �迭
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

    // InterActor �迭 
    UPROPERTY(EditDefaultsOnly, Category = "Inter")
    TArray<TSubclassOf<AInterActor>> InterActorClasses;



    UPROPERTY(VisibleAnywhere, Category = "PlayerCharacter")
    EPlayerCharacter CharacterIconIndex;


    struct SItemRandomLocation
    {
        FVector sLocation;                  // ��ġ�� ���
        bool    bIsSeatLocation = false;    // ��ġ�� ���� �̹� �����ϰ� �ִ°� true 
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

    // ĳ���� ���� �Լ�
    void ChoiceCharacter();

    void SpawnCharacter(int32 characterindex);

    // ������ �ڽ� ��ġ ���س��� �������� ���ؼ� ����
    FVector RandomItemBoxLocation(EItemFloor itemfloor);

    // ī ���� ��ġ ���س��� �������� ���ؼ� ����
    int32 RandomCarActorLocation();

    int32 RandomCarKey();

    // ������ ���� ��ġ ���س��� �迭
    SItemRandomLocation ItemRandomLocationStruct[60];

    // Car ���� ��ġ ���س��� �迭
    SCarActorRandomLocation CarActorRandomLocationStruct[7];

    SCarKeyRandom CarKeyRandom[7];

    // ������ ���� �Լ�
    void ItemRandomLocationSetting();
    void SpawnItemBoxes(int32 itemboxindex, FName itemname, EItemClass itemclass, UTexture2D* texture, int count, EItemFloor itemfloor);

    // ���� ���� �Լ�
    void SpawnZombiesStaticClasses();
    void SpawnZombies(int32 zombieindex, EZombie zombieaiconindex, FVector zombiepos, FRotator zombieroatate, EZombiePatrol zombiepatrol, float patrolrange);

    void UpdateZombie(uint32 ZombieID, FVector NewLocation, FRotator NewRotation);

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
    
    void UpdatePlayerAttack(bool battack);

public:

    uint32 my_class_num;
};
