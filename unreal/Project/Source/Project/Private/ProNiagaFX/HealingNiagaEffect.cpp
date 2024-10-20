// Fill out your copyright notice in the Description page of Project Settings.


#include "ProNiagaFX/HealingNiagaEffect.h"
#include "Components/WidgetComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "ProCharacter/BaseCharacter.h"
//#include "Engine.h"
//#include "EngineUtils.h"

// Sets default values
AHealingNiagaEffect::AHealingNiagaEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));

	RootComponent = Mesh;

	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Healing FX generated");

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NE(TEXT("/Script/Niagara.NiagaraSystem'/Game/HealingEffect.HealingEffect'"));
	if (NE.Succeeded())
	{
		HealingFXSystem = NE.Object;
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Healing FX working fine");
	}
}

// Called when the game starts or when spawned
void AHealingNiagaEffect::BeginPlay()
{
	Super::BeginPlay();
	
	HealingFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HealingFXSystem, FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z));
	if (HealingFXComponent)
	{
		HealingFXComponent->Activate();
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Healing FX played");
	}

}

// Called every frame
void AHealingNiagaEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HealingFXComponent->SetWorldLocation(OwnerChar->GetActorLocation());

}

void AHealingNiagaEffect::EndPlay(EEndPlayReason::Type type)
{
	HealingFXComponent->Deactivate();

	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Healing FX ended");

	Destroy();
}

