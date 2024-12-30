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
	UE_LOG(LogTemp, Log, TEXT("Shouting FX generated"));

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NE(TEXT("/Script/Niagara.NiagaraSystem'/Game/ShoutingEffect.ShoutingEffect'"));
	if (NE.Succeeded())
	{
		ShoutingFXSystem = NE.Object;
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Shouting FX System found");
		UE_LOG(LogTemp, Log, TEXT("Shouting FX System found"));
	}
}

// Called when the game starts or when spawned
void AShoutingNiagaEffect::BeginPlay()
{
	Super::BeginPlay();
	
	ShoutingFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ShoutingFXSystem, FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z));
	if (ShoutingFXComponent)
	{
		ShoutingFXComponent->Activate();
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Shouting FX played");
	}

}

// Called every frame
void AShoutingNiagaEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShoutingNiagaEffect::EndPlay(EEndPlayReason::Type type)
{
	if (ShoutingFXComponent)
		ShoutingFXComponent->Deactivate();

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Shouting FX ended");

	Destroy();
}