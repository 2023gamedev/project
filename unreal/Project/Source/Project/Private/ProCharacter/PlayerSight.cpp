// Fill out your copyright notice in the Description page of Project Settings.


#include "ProCharacter/PlayerSight.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

UPlayerSight::UPlayerSight()
{

	PrimaryComponentTick.bCanEverTick = true;

}


void UPlayerSight::BeginPlay()
{
	Super::BeginPlay();

	
}


void UPlayerSight::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * m_fMaxSightDistance;

	FCollisionShape Sphere = FCollisionShape::MakeSphere(m_fRadius);
	FHitResult HitResult;
	m_bHasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		Sphere
	);


	if (m_bHasHit) { // 충돌 시
		HitActor = HitResult.GetActor();
		if (HitActor == nullptr) {
		}
		else {
		}

	}
	else {
	}


}

