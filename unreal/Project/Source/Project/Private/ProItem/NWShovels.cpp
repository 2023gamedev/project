// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWShovels.h"

ANWShovels::ANWShovels()
{
	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NORMALWEAPON"));
	RootComponent = NormalWeapon;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SHOVELS(TEXT("/Game/StorePropsCollection/StaticMeshes/OfficePapers/SM_Book4.SM_Book4"));
	if (SM_SHOVELS.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_SHOVELS.Object);
	}


	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));

	m_fWeaponSTR		= 6.f;
	m_fWeaponDurability = 10.f;
	m_fWeaponRange		= 3.f;

}
