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
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Found Healing FX!");
	}
}

// Called when the game starts or when spawned
void AHealingNiagaEffect::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AHealingNiagaEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (spawn_flag) {
		spawn_flag = false;

		if (HealingFXSystem) {
			HealingFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HealingFXSystem, FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z) + spawn_offset);
		
			if (materialType == 1) {
				UMaterialInterface* HealingMat = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Mesh/healing.healing"));

				if (HealingMat && HealingFXComponent)
				{
					HealingFXComponent->SetVariableMaterial(TEXT("HealingMaterialParam"), HealingMat);
				}
			}
			if (materialType == 2) {
				UMaterialInterface* HealingMat = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Mesh/zombiehealing.zombiehealing"));

				if (HealingMat && HealingFXComponent)
				{
					HealingFXComponent->SetVariableMaterial(TEXT("HealingMaterialParam"), HealingMat);
				}
			}
		}
		else {
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Healing FX System is nullptr!");
			UE_LOG(LogTemp, Error, TEXT("Healing FX System is nullptr!"));
		}

		if (HealingFXComponent)
		{
			HealingFXComponent->Activate();
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Healing FX played");
		}
	}


	// 캐릭터 위치 따라 이펙트 생성 위치 갱신
	if (OwnerChar && HealingFXComponent) {
		HealingFXComponent->SetWorldLocation(OwnerChar->GetActorLocation() + spawn_offset);
	}

}

void AHealingNiagaEffect::EndPlay(EEndPlayReason::Type type)
{
	if (HealingFXComponent) {
		HealingFXComponent->Deactivate(); // 비활성화
		HealingFXComponent->DestroyComponent();  // 컴포넌트 제거
	}

	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Healing FX ended");

	// 액터 삭제
	if (!IsPendingKillPending())	// 이미 삭제가 예약된 경우 중복 호출 방지
	{
		Destroy();
	}

	Super::EndPlay(type);	// 안 부르면 엔진이 정상적인 정리 작업을 수행하지 못해, 리소스가 제대로 해제되지 않거나(메모리 누수) 레벨 언로드가 지연되어 프레임 드랍과 같은 문제가 발생한다
}

