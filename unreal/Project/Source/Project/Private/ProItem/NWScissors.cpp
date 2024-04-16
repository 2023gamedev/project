// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWScissors.h"

ANWScissors::ANWScissors()
{
	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NORMALWEAPON"));
	RootComponent = NormalWeapon;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SCISSORS(TEXT("/Game/StorePropsCollection/StaticMeshes/OfficePapers/SM_Book4.SM_Book4"));
	if (SM_SCISSORS.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_SCISSORS.Object);
	}



	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));

	m_fWeaponSTR		= 4.f;
	m_fWeaponDurability = 5.f;
	m_fWeaponRange		= 1.f;

}
