// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWScissors.h"

ANWScissors::ANWScissors()
{
	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("NORMALWEAPON"));
	RootComponent = NormalWeapon;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_NORMALWEAPON(TEXT("/Game/StorePropsCollection/StaticMeshes/OfficeSmallProps/SM_Scissors.SM_Scissors"));
	if (SK_NORMALWEAPON.Succeeded()) {
		NormalWeapon->SetSkeletalMesh(SK_NORMALWEAPON.Object);
	}

	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));

	m_fWeaponSTR		= 4.f;
	m_fWeaponDurability = 5.f;
	m_fWeaponRange		= 1.f;

}
