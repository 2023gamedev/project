// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWSashimiKnife.h"

ANWSashimiKnife::ANWSashimiKnife()
{
	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SashimiKnife"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("SashimiKnifeBox"));
	PlaneComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneComponent"));

	RootComponent = NormalWeapon;
	BoxComponent->SetupAttachment(NormalWeapon);
	PlaneComponent->SetupAttachment(BoxComponent);

	BoxComponent->SetRelativeLocation(FVector(22.f, -23.f, 0.f));
	BoxComponent->SetRelativeRotation(FRotator(-90.f, -194.f, 144.f));
	BoxComponent->SetRelativeScale3D(FVector(0.01f, 0.15f, 0.52f));
	
	PlaneComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	PlaneComponent->SetRelativeRotation(FRotator(0.f, 90.f, 90.f));
	PlaneComponent->SetRelativeScale3D(FVector(9.565617f, 2.208863f, 1.0f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SASIMIKNIFE(TEXT("/Game/Mesh/SM_SasimiKnife.SM_SasimiKnife"));
	if (SM_SASIMIKNIFE.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_SASIMIKNIFE.Object);
	}
	ConstructorHelpers::FObjectFinder<UStaticMesh> PLANE(TEXT("/Engine/BasicShapes/Plane.Plane"));
	if (PLANE.Succeeded()) {
		PlaneComponent->SetStaticMesh(PLANE.Object);
	}

	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	BoxComponent->SetCollisionProfileName(TEXT("NoCollision"));
	PlaneComponent->SetCollisionProfileName(TEXT("NoCollision"));

	PlaneComponent->SetHiddenInGame(true);

	m_fWeaponSTR		= 3.f;
	m_fWeaponDurability = WeaponDurability::SashimiKnife;
	m_fWeaponDurability_Max = m_fWeaponDurability;
	m_fWeaponRange		= 1.f;
	WeaponName = "SashimiKnife";
}
