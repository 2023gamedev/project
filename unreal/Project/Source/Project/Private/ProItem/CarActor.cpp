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

	RectLight_1 = CreateDefaultSubobject<URectLightComponent>(TEXT("RectLight_1"));
	RectLight_2 = CreateDefaultSubobject<URectLightComponent>(TEXT("RectLight_2"));
	RectLight_1_f = CreateDefaultSubobject<URectLightComponent>(TEXT("RectLight_1_f"));
	RectLight_2_f = CreateDefaultSubobject<URectLightComponent>(TEXT("RectLight_2_f"));

	RootComponent = Trigger;
	StaticMesh->SetupAttachment(RootComponent);
	RectLight_1->SetupAttachment(RootComponent);
	RectLight_2->SetupAttachment(RootComponent);
	RectLight_1_f->SetupAttachment(RootComponent);
	RectLight_2_f->SetupAttachment(RootComponent);

	Trigger->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_CAR(TEXT("/Game/VehicleVarietyPack/Meshes/SM_Hatchback.SM_Hatchback"));
	if (SM_CAR.Succeeded()) {
		StaticMesh->SetStaticMesh(SM_CAR.Object);
	}

	Trigger->SetCollisionProfileName(TEXT("ItemBoxPS"));
	StaticMesh->SetRelativeLocation(FVector(0.f, 0.f, -60.f));

	// RectLight 설정
	RectLight_1->SetRelativeLocation(FVector(190.052262f, 49.871861f, 6.323853f));
	RectLight_1->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
	
	RectLight_1->SetIntensity(5000.0f);					// 밝기 설정
	RectLight_1->SetLightColor(FLinearColor::Yellow);	// 색상 설정
	RectLight_1->SetSourceWidth(30.0f);					// 너비 설정
	RectLight_1->SetSourceHeight(15.0f);				// 높이 설정
	RectLight_1->SetBarnDoorAngle(45.f);				// 반도어 각도 설정

	RectLight_1_f->SetRelativeLocation(FVector(190.052262f, 49.871861f, 6.323853f));
			   
	RectLight_1_f->SetIntensity(5000.0f);					// 밝기 설정
	RectLight_1_f->SetLightColor(FLinearColor::Yellow);	// 색상 설정
	RectLight_1_f->SetSourceWidth(30.0f);					// 너비 설정
	RectLight_1_f->SetSourceHeight(15.0f);				// 높이 설정
	RectLight_1_f->SetBarnDoorAngle(45.f);				// 반도어 각도 설정
	RectLight_1_f->SetAttenuationRadius(200.f);			// 빛의 감쇠 범위

	RectLight_2->SetRelativeLocation(FVector(190.052262f, -48.966053f, 6.323853f));
	RectLight_2->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));

	RectLight_2->SetIntensity(5000.0f);					// 밝기 설정
	RectLight_2->SetLightColor(FLinearColor::Yellow);	// 색상 설정
	RectLight_2->SetSourceWidth(30.0f);					// 너비 설정
	RectLight_2->SetSourceHeight(15.0f);				// 높이 설정
	RectLight_2->SetBarnDoorAngle(45.f);				// 반도어 각도 설정

	RectLight_2_f->SetRelativeLocation(FVector(190.052262f, -48.966053f, 6.323853f));
			  
	RectLight_2_f->SetIntensity(5000.0f);					// 밝기 설정
	RectLight_2_f->SetLightColor(FLinearColor::Yellow);	// 색상 설정
	RectLight_2_f->SetSourceWidth(30.0f);					// 너비 설정
	RectLight_2_f->SetSourceHeight(15.0f);				// 높이 설정
	RectLight_2_f->SetBarnDoorAngle(45.f);				// 반도어 각도 설정
	RectLight_2_f->SetAttenuationRadius(200.f);			// 빛의 감쇠 범위

	RectLight_1->SetHiddenInGame(true);
	RectLight_1_f->SetHiddenInGame(true);
	RectLight_2->SetHiddenInGame(true);
	RectLight_2_f->SetHiddenInGame(true);

	InterActorName = "CarActor";
	bIsUnlock = false;
}

void ACarActor::UnLock()
{
	bIsUnlock = true;

	RectLight_1->SetHiddenInGame(false);
	RectLight_1_f->SetHiddenInGame(false);
	RectLight_2->SetHiddenInGame(false);
	RectLight_2_f->SetHiddenInGame(false);

	//GetWorldTimerManager().SetTimer(ClearTimer, this, &ACarActor::ClearAddScore, 10.0f, false);
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

void ACarActor::ChangeColorToRed()
{
	// 원래는 바꿀 머테리얼만 읽어와서 기존 스태틱메시에 해당 머테리얼을 교체하는 게 메모리 상으로 좋지만, 그럼 귀찮아서 일단 이렇게 구현...
	UStaticMesh* SM_CAR_RED = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("/Game/VehicleVarietyPack/Meshes/SM_Hatchback_RED.SM_Hatchback_RED")));
	
	if (SM_CAR_RED) {
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Purple, FString::Printf(TEXT("Car #%d changed color to RED!!!"), CarID));

		StaticMesh->SetStaticMesh(SM_CAR_RED);
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