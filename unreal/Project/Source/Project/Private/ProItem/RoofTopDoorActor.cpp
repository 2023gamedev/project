// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/RoofTopDoorActor.h"
#include "ProCharacter/BaseCharacter.h"
#include "EngineUtils.h"


// Sets default values
ARoofTopDoorActor::ARoofTopDoorActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("RTDTRIGGER"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ROOFTOPDOOR"));

	RootComponent = Trigger;
	StaticMesh->SetupAttachment(RootComponent);

	Trigger->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_ROOFTOPDOOR(TEXT("/Game/Mesh/SM_RoofTopDoor.SM_RoofTopDoor"));
	if (SM_ROOFTOPDOOR.Succeeded()) {
		StaticMesh->SetStaticMesh(SM_ROOFTOPDOOR.Object);
	}


	Trigger->SetCollisionProfileName(TEXT("ItemBoxPS"));
	StaticMesh->SetRelativeLocation(FVector(0.f, -70.f, -150.f));

	InterActorName = "RoofTopDoorActor";

	bIsDisableUnlock = false;
	bIsUnlockKey1 = false;
	bIsUnlockKey2 = false;
}

// Called when the game starts or when spawned
void ARoofTopDoorActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ARoofTopDoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARoofTopDoorActor::StartDisableUnlock()
{
	GetWorldTimerManager().SetTimer(ClearTimer, this, &ARoofTopDoorActor::DisableUnlock, 300.0f, false);
}

void ARoofTopDoorActor::DisableUnlock()
{
	bIsDisableUnlock = true;
}

void ARoofTopDoorActor::UnlockKey1()
{
	if (!bIsDisableUnlock) {
		bIsUnlockKey1 = true;

		if (bIsUnlockKey2 == true) {
			UnlockDoor();
		}
	}

}

void ARoofTopDoorActor::UnlockKey2()
{
	if (!bIsDisableUnlock) {
		bIsUnlockKey2 = true;

		if (bIsUnlockKey1 == true) {
			UnlockDoor();
		}
	}
}

void ARoofTopDoorActor::UnlockDoor()
{
	GetWorldTimerManager().SetTimer(ClearTimer, this, &ARoofTopDoorActor::ClearAddScore, 10.0f, false);
}

void ARoofTopDoorActor::ClearAddScore()
{
	// ���� ���� ���� �ִ� �÷��̾ �˻��մϴ�.
	FVector CarLocation = GetActorLocation();
	const float ScoreRadius = 500.0f; // ������ �ְ� ���� ������ �ݰ�
	int32 Score = 0;
	TArray<ABaseCharacter*> PlayersInRange;


	const int32 ScorePerPlayer = 1000; // �� �÷��̾�� �ο��� ����
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
		// ������ �ο��ϴ� �ڵ� �߰�
		Player->AddScore(Score);
	}

}
