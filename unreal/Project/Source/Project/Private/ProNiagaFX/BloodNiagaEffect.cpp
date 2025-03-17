// Fill out your copyright notice in the Description page of Project Settings.


#include "ProNiagaFX/BloodNiagaEffect.h"
#include "Components/WidgetComponent.h"
#include "NiagaraFunctionLibrary.h"
//#include "ProZombie/BaseZombie.h"

// Sets default values
ABloodNiagaEffect::ABloodNiagaEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Blood's ProcMesh"));

	RootComponent = ProcMesh;

	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Blood FX generated");

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NE(TEXT("/Script/Niagara.NiagaraSystem'/Game/BloodEffect.BloodEffect'"));
	if (NE.Succeeded())
	{
		BloodFXSystem = NE.Object;
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Blood FX working fine");
	}
}

// Called when the game starts or when spawned
void ABloodNiagaEffect::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABloodNiagaEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	for (UNiagaraComponent* BloodFX : BloodFXComponents)
	{
		if (BloodFX)
		{
			//BloodFX->SetWorldLocation(GetActorLocation());
			if (ProcMesh) {
				SetActorLocation(ProcMesh->GetComponentLocation());
			}
		}
	}
	

	if (spawn_flag) {
		const float spawn_inteval = 0.5f;
		const float spawn_duration = 3.0f;
		const float destory_time = 5.0f;

		// spawn_inteval 간격으로 스폰
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &ABloodNiagaEffect::SpawnBloodEffect, spawn_inteval, blood_spawnloop, 0.0f);
		// spawn_duration 동안 스폰
		GetWorld()->GetTimerManager().SetTimer(StopSpawnTimerHandle, this, &ABloodNiagaEffect::StopSpawnBloodEffect, 1.0f, false, spawn_duration);
		// destory_time 후에 액터 삭제
		GetWorld()->GetTimerManager().SetTimer(EndTimerHandle, this, &ABloodNiagaEffect::EndBloodEffect, 1.0f, false, destory_time);

		spawn_flag = false;
	}
}

void ABloodNiagaEffect::EndPlay(EEndPlayReason::Type type)
{
	for (UNiagaraComponent* BloodFX : BloodFXComponents)
	{
		if (BloodFX)
		{
			BloodFX->Deactivate();  // 비활성화
			BloodFX->DestroyComponent();  // 컴포넌트 제거
		}
	}

	// 배열 비우기
	BloodFXComponents.Empty();

	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Blood FX ended");

	// 액터 삭제
	if (!IsPendingKillPending())	// 이미 삭제가 예약된 경우 중복 호출 방지
	{
		Destroy();
	}

	Super::EndPlay(type);	// 안 부르면 엔진이 정상적인 정리 작업을 수행하지 못해, 리소스가 제대로 해제되지 않거나(메모리 누수) 레벨 언로드가 지연되어 프레임 드랍과 같은 문제가 발생한다
}

void ABloodNiagaEffect::SpawnBloodEffect()
{
	//RootComponent = ProcMesh;

	//UE_LOG(LogTemp, Log, TEXT("BloodSpawn Location: %s"), *GetActorLocation().ToString());
	//UE_LOG(LogTemp, Log, TEXT("BloodSpawn Rotation: %s"), *GetActorRotation().ToString());

	//DrawDebugPoint(
	//	GetWorld(),
	//	GetActorLocation(),
	//	10.0f,
	//	FColor::Purple,
	//	false,
	//	20.0f,
	//	0
	//);

	//DrawDebugLine(
	//	GetWorld(),
	//	GetActorLocation(),
	//	GetActorLocation() + GetActorRotation().Vector() * 50.0f,
	//	FColor::Red,
	//	false,
	//	20.0f,
	//	0,
	//	1.0f
	//);

	UNiagaraComponent* NewBloodFX = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodFXSystem,
		FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z),
		FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw, GetActorRotation().Roll));

	if (NewBloodFX)
	{
		NewBloodFX->SetNiagaraVariableInt(FString("User.Blood_SpawnCount"), blood_spawncount);

		BloodFXComponents.Add(NewBloodFX);

		NewBloodFX->Activate();

		const int decrease_count = 20;

		blood_spawncount -= decrease_count;	// 시간이 지날 수록 생성되는 피 파티클 수 줄임

		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Blood FX played");
	}
}

void ABloodNiagaEffect::StopSpawnBloodEffect()
{
	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);

	GetWorld()->GetTimerManager().ClearTimer(StopSpawnTimerHandle);
}

void ABloodNiagaEffect::EndBloodEffect()
{
	GetWorld()->GetTimerManager().ClearTimer(EndTimerHandle);

	EndPlay(EEndPlayReason::Destroyed);
}