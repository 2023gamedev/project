// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWFireExtinguisher.h"

ANWFireExtinguisher::ANWFireExtinguisher()
{

	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FireExtinguisher"));
	RootComponent = NormalWeapon;

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_FIREEXTINGUISHER(TEXT("/Game/Showcase/Meshes/SM_FireExtinguisher.SM_FireExtinguisher"));
	if (SM_FIREEXTINGUISHER.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_FIREEXTINGUISHER.Object);
	}





	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));

	m_fWeaponSTR = 3.f;
	m_fWeaponDurability = 10.f;
	m_fWeaponRange = 2.f;

}
