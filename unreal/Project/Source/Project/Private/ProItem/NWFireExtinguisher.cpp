// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWFireExtinguisher.h"

ANWFireExtinguisher::ANWFireExtinguisher()
{

	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FireExtinguisher"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("FireExtinguisherBox"));

	RootComponent = NormalWeapon;
	BoxComponent->SetupAttachment(NormalWeapon);

	BoxComponent->SetRelativeLocation(FVector(0.f, 0.f, 30.f));
	BoxComponent->SetRelativeScale3D(FVector(0.25f, 0.25f, 1.0f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_FIREEXTINGUISHER(TEXT("/Game/Showcase/Meshes/SM_FireExtinguisher.SM_FireExtinguisher"));
	if (SM_FIREEXTINGUISHER.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_FIREEXTINGUISHER.Object);
	}



	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	BoxComponent->SetCollisionProfileName(TEXT("NoCollision"));

	m_fWeaponSTR = 4.5f;
	m_fWeaponDurability = 10.f;
	m_fWeaponRange = 2.f;
	WeaponName = "FireExtinguisher";
}
