// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWFryingPan.h"

ANWFryingPan::ANWFryingPan()
{
	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("NORMALWEAPON"));
	RootComponent = NormalWeapon;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_NORMALWEAPON(TEXT("/Game/Mesh/SM_SquareWood.SM_SquareWood"));
	if (SK_NORMALWEAPON.Succeeded()) {
		NormalWeapon->SetSkeletalMesh(SK_NORMALWEAPON.Object);
	}

	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));

	m_fWeaponSTR = 4.f;
	m_fWeaponDurability = 10.f;
	m_fWeaponRange = 2.f;

}
