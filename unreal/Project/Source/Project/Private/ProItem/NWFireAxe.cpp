// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWFireAxe.h"

ANWFireAxe::ANWFireAxe()
{

	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NORMALWEAPON"));
	RootComponent = NormalWeapon;

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_FIREAXE(TEXT("/Game/StorePropsCollection/StaticMeshes/OfficePapers/SM_Book4.SM_Book4"));
	if (SM_FIREAXE.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_FIREAXE.Object);
	}





	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));

	m_fWeaponSTR = 5.f;
	m_fWeaponDurability = 10.f;
	m_fWeaponRange = 2.f;

}
