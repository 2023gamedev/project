// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWBottle.h"

ANWBottle::ANWBottle()
{

	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bottle"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BottleBox"));

	RootComponent = NormalWeapon;
	BoxComponent->SetupAttachment(NormalWeapon);

	BoxComponent->SetRelativeLocation(FVector(0.f, 0.f, -20.f));
	BoxComponent->SetRelativeScale3D(FVector(0.15f, 0.15f, 0.3f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BOTTLE (TEXT("/Game/Mesh/SM_Bottle.SM_Bottle"));
	if (SM_BOTTLE.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_BOTTLE.Object);
	}

	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	BoxComponent->SetCollisionProfileName(TEXT("NoCollision"));

	m_fWeaponSTR = 2.f;
	m_fWeaponDurability = 3.f;
	m_fWeaponRange = 1.f;

}
