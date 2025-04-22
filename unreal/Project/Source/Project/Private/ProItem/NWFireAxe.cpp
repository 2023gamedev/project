// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWFireAxe.h"

ANWFireAxe::ANWFireAxe()
{

	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FireAxe"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("FireAxeBox"));
	PlaneComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneComponent"));
	
	RootComponent = NormalWeapon;
	BoxComponent->SetupAttachment(NormalWeapon);
	PlaneComponent->SetupAttachment(BoxComponent); 
	
	BoxComponent->SetRelativeLocation(FVector(0.f, 4.733299f, 38.292949f));
	BoxComponent->SetRelativeScale3D(FVector(0.03f, 0.457955f, 0.249752f));

	PlaneComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	PlaneComponent->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	PlaneComponent->SetRelativeScale3D(FVector(5.37f, 0.645938f, 1.0f));
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_FIREAXE(TEXT("/Game/Mesh/SM_FireAxe.SM_FireAxe"));
	if (SM_FIREAXE.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_FIREAXE.Object);
	}
	ConstructorHelpers::FObjectFinder<UStaticMesh> PLANE(TEXT("/Engine/BasicShapes/Plane.Plane"));
	if (PLANE.Succeeded()) {
		PlaneComponent->SetStaticMesh(PLANE.Object);
	}

	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	BoxComponent->SetCollisionProfileName(TEXT("NoCollision"));
	PlaneComponent->SetCollisionProfileName(TEXT("NoCollision"));

	PlaneComponent->SetHiddenInGame(true);

	m_fWeaponSTR = 5.f;
	m_fWeaponDurability = WeaponDurability::FireAxe;
	m_fWeaponDurability_Max = m_fWeaponDurability;
	m_fWeaponRange = 2.5f;
	WeaponName = "FireAxe";
}
