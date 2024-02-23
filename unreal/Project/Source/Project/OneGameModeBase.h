// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "OneGameModeBase.generated.h"

/**
 * 
 */



class ABaseZombie;
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


    // 아이템 박스
    UPROPERTY(EditDefaultsOnly, Category = "ItemBox")
    TArray<TSubclassOf<AItemBoxActor>> ItemBoxClasses;

    UPROPERTY(VisibleAnywhere, Category = "ItemBox")
    int32 m_iItemBoxNumber;


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


    enum class EZombie
    {
        NORMAL,
        SHOUTING,
        RUNNING,
    };


    // 아이템 생성 함수
    void SpawnItemBoxes(int32 itemboxindex, FString itemid, FVector itemboxpos);

    // 좀비 생성 함수
    void SpawnZombies(int32 zombieindex, EZombie zombieaiconindex, FVector zombiepos, bool ispatrol);
};
