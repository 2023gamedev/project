// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWGolfClub.h"

ANWGolfClub::ANWGolfClub()
{
	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("NORMALWEAPON"));
	RootComponent = NormalWeapon;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_NORMALWEAPON(TEXT("/Game/Mesh/SM_GolfClub.SM_GolfClub"));
	if (SK_NORMALWEAPON.Succeeded()) {
		NormalWeapon->SetSkeletalMesh(SK_NORMALWEAPON.Object);
	}

	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));

	m_fWeaponSTR = 6.f;
	m_fWeaponDurability = 8.f;
	m_fWeaponRange = 3.f;

}
