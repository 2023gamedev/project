// Fill out your copyright notice in the Description page of Project Settings.


#include "ProNiagaFX/ResurrectNiagaEffect.h"
#include "Components/WidgetComponent.h"
#include "NiagaraFunctionLibrary.h"


// Sets default values
AResurrectNiagaEffect::AResurrectNiagaEffect()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));

	RootComponent = Mesh;

	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Resurrect FX generated");

}

// Called when the game starts or when spawned
void AResurrectNiagaEffect::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AResurrectNiagaEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (spawn_flag) {
		SpawnResurrectEffect();

		const float destory_time = 20.0f;

		// destory_time 후에 액터까지 삭제
		GetWorld()->GetTimerManager().SetTimer(EndTimerHandle, this, &AResurrectNiagaEffect::EndResurrectEffect, 1.0f, false, destory_time);

		spawn_flag = false;
	}

}

void AResurrectNiagaEffect::EndPlay(EEndPlayReason::Type type)
{
	if (ResurrectFXComponent)
	{
		ResurrectFXComponent->Deactivate();  // 비활성화
		ResurrectFXComponent->DestroyComponent();  // 컴포넌트 제거
	}

	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Resurrect FX ended (->detroy)");

	// 액터 삭제
	if (!IsPendingKillPending())	// 이미 삭제가 예약된 경우 중복 호출 방지
	{
		Destroy();
	}

	Super::EndPlay(type);	// 안 부르면 엔진이 정상적인 정리 작업을 수행하지 못해, 리소스가 제대로 해제되지 않거나(메모리 누수) 레벨 언로드가 지연되어 프레임 드랍과 같은 문제가 발생한다
}

void AResurrectNiagaEffect::SpawnResurrectEffect()
{
	//UE_LOG(LogTemp, Log, TEXT("ResurrectFXSpawn Location: %s"), *GetActorLocation().ToString());
	//UE_LOG(LogTemp, Log, TEXT("ResurrectFXSpawn Rotation: %s"), *GetActorRotation().ToString());
	//
	//DrawDebugPoint(
	//	GetWorld(),
	//	GetActorLocation(),
	//	10.0f,
	//	FColor::Purple,
	//	false,
	//	20.0f,
	//	0
	//);
	//
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

	if (ResurrectFXSystem) {
		float SpawnOffset_Z = -50.0f;
		ResurrectFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ResurrectFXSystem, FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + SpawnOffset_Z));
	}
	else {
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Resurrect FX System is nullptr!");
		UE_LOG(LogTemp, Error, TEXT("Resurrect FX System is nullptr!"));
	}

	if (ResurrectFXComponent)
	{
		ResurrectFXComponent->Activate();

		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Resurrect FX played");
	}
}

void AResurrectNiagaEffect::EndResurrectEffect()
{
	GetWorld()->GetTimerManager().ClearTimer(EndTimerHandle);

	EndPlay(EEndPlayReason::Destroyed);
}