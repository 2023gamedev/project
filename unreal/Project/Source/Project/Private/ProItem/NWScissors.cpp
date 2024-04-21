// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWScissors.h"

ANWScissors::ANWScissors()
{
	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Scissors"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("ScissorsBox"));

	RootComponent = NormalWeapon;
	BoxComponent->SetupAttachment(NormalWeapon);

	BoxComponent->SetRelativeLocation(FVector(0.f, -20.f, 120.f));
	BoxComponent->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));
	BoxComponent->SetRelativeScale3D(FVector(0.15f, 0.15f, 0.25f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SCISSORS(TEXT("/Game/StorePropsCollection/StaticMeshes/OfficeSmallProps/SM_Scissors.SM_Scissors"));
	if (SM_SCISSORS.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_SCISSORS.Object);
	}



	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	BoxComponent->SetCollisionProfileName(TEXT("NoCollision"));

	m_fWeaponSTR		= 4.f;
	m_fWeaponDurability = 5.f;
	m_fWeaponRange		= 1.f;

}
