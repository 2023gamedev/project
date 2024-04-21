// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWFireAxe.h"

ANWFireAxe::ANWFireAxe()
{

	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FireAxe"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("FireAxeBox"));

	RootComponent = NormalWeapon;
	BoxComponent->SetupAttachment(NormalWeapon);

	BoxComponent->SetRelativeLocation(FVector(0.f, 0.f, 18.f));
	BoxComponent->SetRelativeScale3D(FVector(0.03f, 0.25f, 0.2f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_FIREAXE(TEXT("/Game/Mesh/SM_FireAxe.SM_FireAxe"));
	if (SM_FIREAXE.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_FIREAXE.Object);
	}




	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	BoxComponent->SetCollisionProfileName(TEXT("NoCollision"));


	m_fWeaponSTR = 5.f;
	m_fWeaponDurability = 10.f;
	m_fWeaponRange = 2.f;

}
