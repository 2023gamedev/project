// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWWoodenBat.h"

ANWWoodenBat::ANWWoodenBat()
{
	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WoodenBat"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("WoodenBatBox"));

	RootComponent = NormalWeapon;
	BoxComponent->SetupAttachment(NormalWeapon);

	BoxComponent->SetRelativeLocation(FVector(0.f, -20.f, 0.f));
	BoxComponent->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));
	BoxComponent->SetRelativeScale3D(FVector(0.25f, 0.2f, 0.75f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_WODDENBAT(TEXT("/Game/Safe_House/meshes/SM_woodenbat.SM_woodenbat"));
	if (SM_WODDENBAT.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_WODDENBAT.Object);
	}

	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	BoxComponent->SetCollisionProfileName(TEXT("NoCollision"));

	m_fWeaponSTR		= 4.f;
	m_fWeaponDurability = 8.f;
	m_fWeaponRange		= 2.f;

}
