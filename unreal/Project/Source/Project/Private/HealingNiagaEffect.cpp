// Fill out your copyright notice in the Description page of Project Settings.


#include "HealingNiagaEffect.h"
#include "Components/WidgetComponent.h"
#include "NiagaraFunctionLibrary.h"


// Sets default values
AHealingNiagaEffect::AHealingNiagaEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));

	RootComponent = Mesh;


	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NE(TEXT("NiagaraSystem'2024_project/unreal/Project/Content/HealingEffect'"));
	if (NE.Succeeded())
	{
		HealingFXSystem = NE.Object;
	}
}

// Called when the game starts or when spawned
void AHealingNiagaEffect::BeginPlay()
{
	Super::BeginPlay();

	HealingFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HealingFXSystem, FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 60.0f));
	if (HealingFXComponent)
	{
		HealingFXComponent->Activate();
	}
}


void AHealingNiagaEffect::Interaction()
{

	HealingFXComponent->Deactivate();

}

