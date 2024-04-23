// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWSquareWood.h"



ANWSquareWood::ANWSquareWood()
{
	PrimaryActorTick.bCanEverTick = false;
	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SquareWood"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("SquareWoodBox"));
	
	RootComponent = NormalWeapon;
	BoxComponent->SetupAttachment(NormalWeapon);

	BoxComponent->SetRelativeLocation(FVector(0.f, 0.f, 120.f));
	BoxComponent->SetRelativeScale3D(FVector(0.25f, 0.25f, 1.0f));



	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SQUREWOOD(TEXT("/Game/Mesh/SM_SquareWood.SM_SquareWood"));
	if (SM_SQUREWOOD.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_SQUREWOOD.Object);
	}


	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision")); // 충돌 프리셋은 변경될 수도 있다.
	BoxComponent->SetCollisionProfileName(TEXT("NoCollision"));

	m_fWeaponSTR		= 2.f;
	m_fWeaponDurability = 8.f;
	m_fWeaponRange		= 3.f;
	WeaponName = "SquareWood";
}

void ANWSquareWood::PlaceItem()
{
}