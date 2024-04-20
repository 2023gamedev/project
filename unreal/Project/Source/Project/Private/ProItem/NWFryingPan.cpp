// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWFryingPan.h"

ANWFryingPan::ANWFryingPan()
{
	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FryingPan"));
	RootComponent = NormalWeapon;

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_FRYINGPAN(TEXT("/Game/Mesh/SM_FryingPan.SM_FryingPan"));
	if (SM_FRYINGPAN.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_FRYINGPAN.Object);
	}




	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));

	m_fWeaponSTR = 4.f;
	m_fWeaponDurability = 10.f;
	m_fWeaponRange = 2.f;

}
