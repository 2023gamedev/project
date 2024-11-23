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

	if (spawn_flag) {
		UNiagaraComponent* NewBloodFX = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BloodFXSystem,
			FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z),
			FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw, GetActorRotation().Roll));

		if (NewBloodFX)
		{
			NewBloodFX->SetNiagaraVariableInt(FString("User.Blood_SpawnCount"), blood_spawncount);

			BloodFXComponents.Add(NewBloodFX);

			NewBloodFX->Activate();

			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Blood FX played");
		}

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

	Destroy();
}

void ABloodNiagaEffect::SpawnBloodEffect()
{

}
