// Fill out your copyright notice in the Description page of Project Settings.


#include "ProNiagaFX/BloodNiagaEffect.h"
#include "Components/WidgetComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "ProZombie/BaseZombie.h"

// Sets default values
ABloodNiagaEffect::ABloodNiagaEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));

	RootComponent = Mesh;

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
	
	BloodFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodFXSystem, 
		FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z), 
		FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw, GetActorRotation().Roll));

	if (BloodFXComponent)
	{
		BloodFXComponent->Activate();
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Blood FX played");
	}

}

// Called every frame
void ABloodNiagaEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//BloodFXComponent->SetWorldLocation(OwnerChar->GetActorLocation());

}


void ABloodNiagaEffect::EndPlay(EEndPlayReason::Type type)
{
	if(BloodFXComponent)
		BloodFXComponent->Deactivate();

	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Blood FX ended");

	Destroy();
}
