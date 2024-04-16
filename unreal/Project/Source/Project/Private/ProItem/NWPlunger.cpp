// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWPlunger.h"

ANWPlunger::ANWPlunger()
{
	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NORMALWEAPON"));
	RootComponent = NormalWeapon;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_PLUNGER(TEXT("/Game/StorePropsCollection/StaticMeshes/OfficePapers/SM_Book4.SM_Book4"));
	if (SM_PLUNGER.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_PLUNGER.Object);
	}


	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));

	m_fWeaponSTR		= 2.f;
	m_fWeaponDurability = 6.f;
	m_fWeaponRange		= 3.f;

}
