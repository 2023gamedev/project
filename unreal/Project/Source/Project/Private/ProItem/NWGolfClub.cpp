// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWGolfClub.h"

ANWGolfClub::ANWGolfClub()
{
	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GolfClub"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("GolfClubBox"));

	RootComponent = NormalWeapon;
	BoxComponent->SetupAttachment(NormalWeapon);

	BoxComponent->SetRelativeLocation(FVector(0.f, 90.f, 0.f));
	BoxComponent->SetRelativeRotation(FRotator(0.f, 0.f, 90.f));
	BoxComponent->SetRelativeScale3D(FVector(0.25f, 0.1f, 0.75f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_GOLFCLUB(TEXT("/Game/Mesh/SM_GolfClub.SM_GolfClub"));
	if (SM_GOLFCLUB.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_GOLFCLUB.Object);
	}


	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	BoxComponent->SetCollisionProfileName(TEXT("NoCollision"));

	m_fWeaponSTR = 5.f;
	m_fWeaponDurability = WeaponDurability::GolfClub;
	m_fWeaponDurability_Max = m_fWeaponDurability;
	m_fWeaponRange = 3.f;
	WeaponName = "GolfClub";
}
