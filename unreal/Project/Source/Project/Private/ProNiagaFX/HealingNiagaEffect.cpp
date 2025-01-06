// Fill out your copyright notice in the Description page of Project Settings.


#include "ProNiagaFX/HealingNiagaEffect.h"
#include "Components/WidgetComponent.h"
#include "NiagaraFunctionLibrary.h"
//#include "ProCharacter/BaseCharacter.h"
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
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Healing FX System found");
	}
}

// Called when the game starts or when spawned
void AHealingNiagaEffect::BeginPlay()
{
	Super::BeginPlay();
	
	HealingFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HealingFXSystem, FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z) + spawn_offset);
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

	if (OwnerChar && HealingFXComponent) {
		HealingFXComponent->SetWorldLocation(OwnerChar->GetActorLocation() + spawn_offset);
	}

}

void AHealingNiagaEffect::EndPlay(EEndPlayReason::Type type)
{
	if(HealingFXComponent)
		HealingFXComponent->Deactivate();

	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Healing FX ended");

	//Destroy();
	Super::EndPlay(type);	// 안 부르면 엔진이 정상적인 정리 작업을 수행하지 못해, 리소스가 제대로 해제되지 않거나(메모리 누수) 레벨 언로드가 지연되어 프레임 드랍과 같은 문제가 발생한다
}

