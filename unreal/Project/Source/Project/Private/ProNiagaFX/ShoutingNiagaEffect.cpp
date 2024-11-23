// Fill out your copyright notice in the Description page of Project Settings.


#include "ProNiagaFX/ShoutingNiagaEffect.h"
#include "Components/WidgetComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "ProZombie/BaseZombie.h"

// Sets default values
AShoutingNiagaEffect::AShoutingNiagaEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
 
	RootComponent = Mesh;

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NE(TEXT("/Script/Niagara.NiagaraSystem'/Game/ShoutingEffect.ShoutingEffect'"));
	if (NE.Succeeded())
	{
		ShoutingFXSystem = NE.Object;
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
	}

}

// Called every frame
void AShoutingNiagaEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShoutingNiagaEffect::EndPlay(EEndPlayReason::Type type)
{
	ShoutingFXComponent->Deactivate();

	Destroy();
}