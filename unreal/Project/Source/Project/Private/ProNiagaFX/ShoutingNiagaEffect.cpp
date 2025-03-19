// Fill out your copyright notice in the Description page of Project Settings.


#include "ProNiagaFX/ShoutingNiagaEffect.h"

#include "Components/WidgetComponent.h"
#include "NiagaraFunctionLibrary.h"
//#include "ProZombie/BaseZombie.h"

// Sets default values
AShoutingNiagaEffect::AShoutingNiagaEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
 
	RootComponent = Mesh;

	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Shouting FX generated");	// 게임 초기화 전에 부르면 메모리 엑세스 오류 일으킴 (최소 BeginPlay 이후에 불러야함)
	//UE_LOG(LogTemp, Log, TEXT("Shouting FX generated"));

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NE(TEXT("/Script/Niagara.NiagaraSystem'/Game/ShoutingEffect.ShoutingEffect'"));
	if (NE.Succeeded())
	{
		ShoutingFXSystem = NE.Object;
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Found Shouting FX!");
		//UE_LOG(LogTemp, Log, TEXT("Found Shouting FX!"));
	}
}

// Called when the game starts or when spawned
void AShoutingNiagaEffect::BeginPlay()
{
	Super::BeginPlay();
	
	if (ShoutingFXSystem) {
		float SpawnOffset_Z = 5.0f;
		ShoutingFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ShoutingFXSystem, FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + SpawnOffset_Z));
	}
	else {
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Shouting FX System is nullptr!");
		UE_LOG(LogTemp, Error, TEXT("Shouting FX System is nullptr!"));
	}

	if (ShoutingFXComponent)
	{
		ShoutingFXComponent->Activate();
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Shouting FX played");
	}

}

// Called every frame
void AShoutingNiagaEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShoutingNiagaEffect::EndPlay(EEndPlayReason::Type type)
{
	if (ShoutingFXComponent) {
		ShoutingFXComponent->Deactivate(); // 비활성화
		ShoutingFXComponent->DestroyComponent();  // 컴포넌트 제거
	}

	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Shouting FX ended");

	// 액터 삭제
	if (!IsPendingKillPending())	// 이미 삭제가 예약된 경우 중복 호출 방지
	{
		Destroy();
	}

	Super::EndPlay(type);	// 안 부르면 엔진이 정상적인 정리 작업을 수행하지 못해, 리소스가 제대로 해제되지 않거나(메모리 누수) 레벨 언로드가 지연되어 프레임 드랍과 같은 문제가 발생한다
}