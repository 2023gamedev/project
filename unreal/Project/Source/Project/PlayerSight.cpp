// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSight.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UPlayerSight::UPlayerSight()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerSight::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlayerSight::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * m_fMaxSightDistance;
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(m_fRadius);
	FHitResult HitResult;
	m_bHasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel1,
		Sphere
	);

	//UE_LOG(LogTemp, Warning, TEXT("xxxxxxxxxxxxxxxxxxxxx"));

	if (m_bHasHit) { // 충돌 시
		HitActor = HitResult.GetActor();
		if (HitActor == nullptr) {
			//UE_LOG(LogTemp, Warning, TEXT("xxxxxxxxxxxxxxxxxxxxx"));
		}
		else {
			//UE_LOG(LogTemp, Warning, TEXT("oooooooooooooooooooooooo"));
		}
		UE_LOG(LogTemp, Warning, TEXT("oooooooooooooooooooooooo"));
		//UE_LOG(LogTemp, Warning, TEXT("Hit Actor : %s"), *HitActor->GetActorNameOrLabel());
	}
	else {
		// 충돌 안함
	}


}

