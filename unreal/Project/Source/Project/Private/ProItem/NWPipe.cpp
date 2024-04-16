// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWPipe.h"

ANWPipe::ANWPipe()
{
	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NORMALWEAPON"));
	RootComponent = NormalWeapon;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_PIPE(TEXT("/Game/StorePropsCollection/StaticMeshes/OfficePapers/SM_Book4.SM_Book4"));
	if (SM_PIPE.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_PIPE.Object);
	}


	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));

	m_fWeaponSTR		= 2.f;
	m_fWeaponDurability = 9.f;
	m_fWeaponRange		= 3.f;

}
