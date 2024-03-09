// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProData/ItemDataStructure.h"
#include "OneGameModeBase.generated.h"

/**
 * 
 */

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
    EMPLOYEE,
    FIREFIGHTER,
    GIRL,
    IDOL,
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
    //virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) ov

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


    // 캐릭터 선택 함수
    void ChoiceCharacter();

    void SpawnCharacter(int32 characterindex);

    // 아이템 생성 함수
    void SpawnItemBoxes(int32 itemboxindex, FName itemname, EItemClass itemclass, UTexture2D* texture, int count, FVector itemboxpos);

    // 좀비 생성 함수
    void SpawnZombies(int32 zombieindex, EZombie zombieaiconindex, FVector zombiepos, bool ispatrol);
};
