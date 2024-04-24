// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWShovels.h"

ANWShovels::ANWShovels()
{
	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Shovels"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("ShovelsBox"));

	RootComponent = NormalWeapon;
	BoxComponent->SetupAttachment(NormalWeapon);

	BoxComponent->SetRelativeLocation(FVector(0.f, 0.f, -20.f));
	BoxComponent->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.5f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SHOVELS(TEXT("/Game/Safe_House/meshes/SM_shovel.SM_shovel"));
	if (SM_SHOVELS.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_SHOVELS.Object);
	}


	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	BoxComponent->SetCollisionProfileName(TEXT("NoCollision"));

	m_fWeaponSTR		= 6.f;
	m_fWeaponDurability = 10.f;
	m_fWeaponRange		= 3.f;
	WeaponName = "Shovels";
}
