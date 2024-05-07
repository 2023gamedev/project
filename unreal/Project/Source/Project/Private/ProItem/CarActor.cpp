// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/CarActor.h"
#include "ProCharacter/BaseCharacter.h"
#include "EngineUtils.h"
#include "ProGamemode/ProGameInstance.h"

// Sets default values
ACarActor::ACarActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("CTRIGGER"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CARACTOR"));

	RootComponent = Trigger;
	StaticMesh->SetupAttachment(RootComponent);

	Trigger->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_CAR(TEXT("/Game/VehicleVarietyPack/Meshes/SM_Hatchback.SM_Hatchback"));
	if (SM_CAR.Succeeded()) {
		StaticMesh->SetStaticMesh(SM_CAR.Object);
	}


	Trigger->SetCollisionProfileName(TEXT("ItemBoxPS"));
	StaticMesh->SetRelativeLocation(FVector(0.f, 0.f, -60.f));

	InterActorName = "CarActor";
	bIsUnlock = false;
}

void ACarActor::UnLock()
{
	bIsUnlock = true;

	GetWorldTimerManager().SetTimer(ClearTimer, this, &ACarActor::ClearAddScore, 10.0f, false);
}

void ACarActor::ClearAddScore()
{
    // 일정 범위 내에 있는 플레이어를 검색합니다.
    FVector CarLocation = GetActorLocation();
    const float ScoreRadius = 500.0f; // 점수를 주고 싶은 범위의 반경
	int32 Score = 0;
	TArray<ABaseCharacter*> PlayersInRange;

    const int32 ScorePerPlayer = 600; // 각 플레이어당 부여할 점수
    for (TActorIterator<ABaseCharacter> It(GetWorld()); It; ++It)
    {
        ABaseCharacter* PlayerCharacter = *It;
        if (PlayerCharacter)
        {
            FVector PlayerLocation = PlayerCharacter->GetActorLocation();
            float DistanceSquared = FVector::DistSquared(CarLocation, PlayerLocation);
            if (DistanceSquared <= FMath::Square(ScoreRadius))
            {
				PlayersInRange.Add(PlayerCharacter);
				Score += ScorePerPlayer;
            }
        }
    }


	for (ABaseCharacter* Player : PlayersInRange)
	{
		Player->AddScore(Score);
	}

}

// Called when the game starts or when spawned
void ACarActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACarActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}