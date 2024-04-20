// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWMannequinArm.h"

ANWMannequinArm::ANWMannequinArm()
{
	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MannequinArm"));
	RootComponent = NormalWeapon;

	 ConstructorHelpers::FObjectFinder<UStaticMesh> SM_MANNEQUINARM(TEXT("/Game/Mesh/SM_MannequinArm.SM_MannequinArm"));
	if (SM_MANNEQUINARM.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_MANNEQUINARM.Object);
	}


	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));

	m_fWeaponSTR		= 2.f;
	m_fWeaponDurability = 6.f;
	m_fWeaponRange		= 2.f;

}
