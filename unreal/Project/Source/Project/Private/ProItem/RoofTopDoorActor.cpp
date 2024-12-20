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
	StartDisableUnlock();
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
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("ARoofTopDoorActor::DisableUnlock()")));
	//TArray<ABaseCharacter*> Players;



	//// 5분지날 시 옥상문 잠기는 Text
	//FText KText = FText::FromString(TEXT("헬기가 떠났습니다."));
	//FText KMissionText1 = FText::FromString(TEXT("실패"));
	//FText KMissionText2 = FText::FromString(TEXT(""));
	//for (TActorIterator<ABaseCharacter> It(GetWorld()); It; ++It)
	//{
	//	ABaseCharacter* PlayerCharacter = *It;
	//	if (PlayerCharacter)
	//	{
	//		PlayerCharacter->ShowActionText(KText, FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f)), 5.f);

	//		// 5분 지날 시 옥상문 잠기는 Text

	//		PlayerCharacter->ShowMissionText(KMissionText1, FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f)), 1);


	//		PlayerCharacter->ShowMissionText(KMissionText2, FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f)), 2);
	//	}
	//}
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
	//GetWorldTimerManager().SetTimer(ClearTimer, this, &ARoofTopDoorActor::ClearAddScore, 10.0f, false);
}

void ARoofTopDoorActor::ClearAddScore()
{
	// 일정 범위 내에 있는 플레이어를 검색합니다.
	FVector CarLocation = GetActorLocation();
	const float ScoreRadius = 500.0f; // 점수를 주고 싶은 범위의 반경
	int32 Score = 0;
	TArray<ABaseCharacter*> PlayersInRange;


	const int32 ScorePerPlayer = 1000; // 각 플레이어당 부여할 점수
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
		// 점수를 부여하는 코드 추가
		Player->AddScore(Score);
	}

}
