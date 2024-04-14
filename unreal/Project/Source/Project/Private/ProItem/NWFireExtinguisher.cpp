// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWFireExtinguisher.h"

ANWFireExtinguisher::ANWFireExtinguisher()
{

	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("NORMALWEAPON"));
	RootComponent = NormalWeapon;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_NORMALWEAPON(TEXT("/Game/Showcase/Meshes/SM_Fire_Extinguisher_01a.SM_Fire_Extinguisher_01a"));
	if (SK_NORMALWEAPON.Succeeded()) {
		NormalWeapon->SetSkeletalMesh(SK_NORMALWEAPON.Object);
	}

	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));

	m_fWeaponSTR = 3.f;
	m_fWeaponDurability = 10.f;
	m_fWeaponRange = 2.f;

}
