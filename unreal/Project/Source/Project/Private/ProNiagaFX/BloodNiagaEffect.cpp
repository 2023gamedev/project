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

	UE_LOG(LogTemp, Log, TEXT("BloodFX Actor Initialized"));
}

// Called when the game starts or when spawned
void ABloodNiagaEffect::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("BloodFX BeginPlay"));
}

// Called every frame
void ABloodNiagaEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (spawn_flag) {
		BloodFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodFXSystem,
			FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z),
			FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw, GetActorRotation().Roll));
		UE_LOG(LogTemp, Log, TEXT("BloodFX SpawnSystemAtLocation"));

		if (BloodFXComponent)
		{
			BloodFXComponent->SetNiagaraVariableInt(FString("User.Blood_SpawnCount"), blood_spawncount);
			UE_LOG(LogTemp, Log, TEXT("BloodFX SetNiagaraVariableInt SpawnCount Set - %d"), blood_spawncount);

			BloodFXComponent->Activate();
			UE_LOG(LogTemp, Log, TEXT("BloodFX Activate"));

			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Blood FX played");
		}

		spawn_flag = false;
	}
}


void ABloodNiagaEffect::EndPlay(EEndPlayReason::Type type)
{
	if (BloodFXComponent)
		BloodFXComponent->Deactivate();

	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Blood FX ended");

	Destroy();
}
