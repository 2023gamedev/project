// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWPlunger.h"

ANWPlunger::ANWPlunger()
{
	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plunger"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("PlungerBox"));

	RootComponent = NormalWeapon;
	BoxComponent->SetupAttachment(NormalWeapon);

	BoxComponent->SetRelativeLocation(FVector(0.f, 0.f, -40.f));
	BoxComponent->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.5f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_PLUNGER(TEXT("/Game/Mesh/SM_Plunger.SM_Plunger"));
	if (SM_PLUNGER.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_PLUNGER.Object);
	}


	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	BoxComponent->SetCollisionProfileName(TEXT("NoCollision"));

	m_fWeaponSTR		= 2.f;
	m_fWeaponDurability = 7.f;
	m_fWeaponRange		= 3.f;
	WeaponName = "Plunger";
}
