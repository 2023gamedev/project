// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWMannequinLeg.h"

ANWMannequinLeg::ANWMannequinLeg()
{
	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MannequinLeg"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("MannequinLegBox"));

	RootComponent = NormalWeapon;
	BoxComponent->SetupAttachment(NormalWeapon);

	BoxComponent->SetRelativeLocation(FVector(2.f, 0.f, -40.f));
	BoxComponent->SetRelativeScale3D(FVector(0.15f, 0.25f, 0.75f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_MANNEQUINLEG(TEXT("/Game/Mesh/SM_MannequinLeg.SM_MannequinLeg"));
	if (SM_MANNEQUINLEG.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_MANNEQUINLEG.Object);
	}



	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	BoxComponent->SetCollisionProfileName(TEXT("NoCollision"));

	m_fWeaponSTR		= 2.f;
	m_fWeaponDurability = 6.f;
	m_fWeaponRange		= 2.f;

}
