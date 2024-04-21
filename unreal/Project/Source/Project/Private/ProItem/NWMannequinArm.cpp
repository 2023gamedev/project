// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWMannequinArm.h"

ANWMannequinArm::ANWMannequinArm()
{
	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MannequinArm"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("MannequinArmBox"));

	RootComponent = NormalWeapon;
	BoxComponent->SetupAttachment(NormalWeapon);

	BoxComponent->SetRelativeLocation(FVector(-22.98f, -2.f, -19.28f));
	BoxComponent->SetRelativeRotation(FRotator(50.f, 180.f, 180.f));
	BoxComponent->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.5f));

	 ConstructorHelpers::FObjectFinder<UStaticMesh> SM_MANNEQUINARM(TEXT("/Game/Mesh/SM_MannequinArm.SM_MannequinArm"));
	if (SM_MANNEQUINARM.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_MANNEQUINARM.Object);
	}


	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	BoxComponent->SetCollisionProfileName(TEXT("NoCollision"));

	m_fWeaponSTR		= 2.f;
	m_fWeaponDurability = 6.f;
	m_fWeaponRange		= 2.f;

}
