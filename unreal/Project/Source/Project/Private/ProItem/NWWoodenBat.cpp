// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWWoodenBat.h"

ANWWoodenBat::ANWWoodenBat()
{
	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("NORMALWEAPON"));
	RootComponent = NormalWeapon;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_NORMALWEAPON(TEXT("/Game/Safe_House/meshes/SM_woodenbat.SM_woodenbat"));
	if (SK_NORMALWEAPON.Succeeded()) {
		NormalWeapon->SetSkeletalMesh(SK_NORMALWEAPON.Object);
	}

	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));

	m_fWeaponSTR		= 4.f;
	m_fWeaponDurability = 8.f;
	m_fWeaponRange		= 2.f;

}
