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

UCLASS()
class PROJECT_API AOneGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AOneGameModeBase();

	virtual void PostLogin(APlayerController* NewPlayer) override;

    virtual void BeginPlay() override;
    
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

    UPROPERTY(VisibleAnywhere, Category = "Zombies")
    float m_fPatrolRange;


    UPROPERTY(VisibleAnywhere, Category ="PlayerCharacter")
    EPlayerCharacter CharacterIconIndex;


    struct SItemRandomLocation
    {
        FVector sLocation;                  // 위치가 어딘가
        bool    bIsSeatLocation = false;    // 위치에 누가 이미 차지하고 있는가 true 
    };


    void ChoiceCharacterBefore();

    // 캐릭터 선택 함수
    void ChoiceCharacter();

    void SpawnCharacter(int32 characterindex);

    // 아이템 박스 위치 정해놓고 랜덤으로 정해서 생성
    FVector RandomItemBoxLocation(EItemFloor itemfloor);  // 제작 예정

    // 아이템 랜덤 위치 정해놓는 배열
    SItemRandomLocation ItemRandomLocationStruct[60];

    // 아이템 생성 함수
    void SpawnItemBoxes(int32 itemboxindex, FName itemname, EItemClass itemclass, UTexture2D* texture, int count, EItemFloor itemfloor);

    // 좀비 생성 함수
    void SpawnZombies(int32 zombieindex, EZombie zombieaiconindex, FVector zombiepos, bool ispatrol);

    void UpdateZombie(uint32 ZombieID, FVector NewLocation, FRotator NewRotation);

    int32 GetItemBoxNumber() { return m_iItemBoxNumber; }

    void SpawnOnGroundItem(FName itemname, EItemClass itemclass, UTexture2D* texture, int count);

    void UpdateOtherPlayer(uint32 PlayerID, FVector NewLocation, FRotator NewRotation);

};
