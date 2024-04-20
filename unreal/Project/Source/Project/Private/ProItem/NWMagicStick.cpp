// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWMagicStick.h"

ANWMagicStick::ANWMagicStick()
{
	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MagicStick"));
	RootComponent = NormalWeapon;

	 ConstructorHelpers::FObjectFinder<UStaticMesh> SM_MAGICSTICK(TEXT("/Game/Safe_House/meshes/SM_magicstick.SM_magicstick"));
	if (SM_MAGICSTICK.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_MAGICSTICK.Object);
	}



	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));

	m_fWeaponSTR		= 2.f;
	m_fWeaponDurability = 5.f;
	m_fWeaponRange		= 2.f;

}
