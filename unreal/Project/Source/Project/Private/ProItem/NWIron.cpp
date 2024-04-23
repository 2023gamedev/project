// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWIron.h"

ANWIron::ANWIron()
{
	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Iron"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("IronBox"));

	RootComponent = NormalWeapon;
	BoxComponent->SetupAttachment(NormalWeapon);

	BoxComponent->SetRelativeLocation(FVector(0.f, 0.f, -15.f));
	BoxComponent->SetRelativeScale3D(FVector(0.4f, 0.2f, 0.25f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_IRON(TEXT("/Game/Mesh/SM_Iron.SM_Iron"));
	if (SM_IRON.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_IRON.Object);
	}


	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	BoxComponent->SetCollisionProfileName(TEXT("NoCollision"));

	m_fWeaponSTR			= 4.f;
	m_fWeaponDurability		= 6.f;
	m_fWeaponRange			= 1.f;
	WeaponName = "Iron";
}
