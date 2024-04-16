// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWSashimiKnife.h"

ANWSashimiKnife::ANWSashimiKnife()
{
	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NORMALWEAPON"));
	RootComponent = NormalWeapon;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SASIMIKNIFE(TEXT("/Game/StorePropsCollection/StaticMeshes/OfficePapers/SM_Book4.SM_Book4"));
	if (SM_SASIMIKNIFE.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_SASIMIKNIFE.Object);
	}



	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));

	m_fWeaponSTR		= 3.f;
	m_fWeaponDurability = 9.f;
	m_fWeaponRange		= 1.f;

}
