// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWGolfClub.h"

ANWGolfClub::ANWGolfClub()
{
	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GolfClub"));
	RootComponent = NormalWeapon;

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_GOLFCLUB(TEXT("/Game/Mesh/SM_GolfClub.SM_GolfClub"));
	if (SM_GOLFCLUB.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_GOLFCLUB.Object);
	}


	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));

	m_fWeaponSTR = 6.f;
	m_fWeaponDurability = 8.f;
	m_fWeaponRange = 3.f;

}
