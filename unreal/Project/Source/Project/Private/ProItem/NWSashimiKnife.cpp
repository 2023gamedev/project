// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWSashimiKnife.h"

ANWSashimiKnife::ANWSashimiKnife()
{
	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SashimiKnife"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("SashimiKnifeBox"));

	RootComponent = NormalWeapon;
	BoxComponent->SetupAttachment(NormalWeapon);
	BoxComponent->SetRelativeLocation(FVector(22.f, -23.f, 0.f));
	BoxComponent->SetRelativeRotation(FRotator(-90.f, -194.f, 144.f));
	BoxComponent->SetRelativeScale3D(FVector(0.01f, 0.15f, 0.52f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SASIMIKNIFE(TEXT("/Game/Mesh/SM_SasimiKnife.SM_SasimiKnife"));
	if (SM_SASIMIKNIFE.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_SASIMIKNIFE.Object);
	}



	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	BoxComponent->SetCollisionProfileName(TEXT("NoCollision"));

	m_fWeaponSTR		= 3.f;
	m_fWeaponDurability = 9.f;
	m_fWeaponRange		= 1.f;
	WeaponName = "SashimiKnife";
}
