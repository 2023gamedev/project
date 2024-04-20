// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWWoodenBat.h"

ANWWoodenBat::ANWWoodenBat()
{
	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WoodenBat"));
	RootComponent = NormalWeapon;

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_WODDENBAT(TEXT("/Game/Safe_House/meshes/SM_woodenbat.SM_woodenbat"));
	if (SM_WODDENBAT.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_WODDENBAT.Object);
	}

	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));

	m_fWeaponSTR		= 4.f;
	m_fWeaponDurability = 8.f;
	m_fWeaponRange		= 2.f;

}
