// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWMagicStick.h"

ANWMagicStick::ANWMagicStick()
{
	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MagicStick"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("MagicStickBox"));

	RootComponent = NormalWeapon;
	BoxComponent->SetupAttachment(NormalWeapon);

	BoxComponent->SetRelativeLocation(FVector(10.f, 0.f, 0.f));
	BoxComponent->SetRelativeRotation(FRotator(-90.f, 0.f, 90.f));
	BoxComponent->SetRelativeScale3D(FVector(0.15f, 0.2f, 0.5f));

	 ConstructorHelpers::FObjectFinder<UStaticMesh> SM_MAGICSTICK(TEXT("/Game/Safe_House/meshes/SM_magicstick.SM_magicstick"));
	if (SM_MAGICSTICK.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_MAGICSTICK.Object);
	}



	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	BoxComponent->SetCollisionProfileName(TEXT("NoCollision"));

	m_fWeaponSTR		= 2.f;
	m_fWeaponDurability = 5.f;
	m_fWeaponRange		= 2.f;

}
