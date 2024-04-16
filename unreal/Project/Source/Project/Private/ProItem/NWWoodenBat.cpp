// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWWoodenBat.h"

ANWWoodenBat::ANWWoodenBat()
{
	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NORMALWEAPON"));
	RootComponent = NormalWeapon;

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_WODDENBAT(TEXT("/Game/StorePropsCollection/StaticMeshes/OfficePapers/SM_Book4.SM_Book4"));
	if (SM_WODDENBAT.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_WODDENBAT.Object);
	}

	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));

	m_fWeaponSTR		= 4.f;
	m_fWeaponDurability = 8.f;
	m_fWeaponRange		= 2.f;

}
