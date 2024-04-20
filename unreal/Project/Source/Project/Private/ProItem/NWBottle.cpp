// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWBottle.h"

ANWBottle::ANWBottle()
{

	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bottle"));
	RootComponent = NormalWeapon;

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BOTTLE (TEXT("/Game/Mesh/SM_Bottle.SM_Bottle"));
	if (SM_BOTTLE.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_BOTTLE.Object);
	}

	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));

	m_fWeaponSTR = 2.f;
	m_fWeaponDurability = 3.f;
	m_fWeaponRange = 1.f;

}
