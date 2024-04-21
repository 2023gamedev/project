// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWFryingPan.h"

ANWFryingPan::ANWFryingPan()
{
	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FryingPan"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("FryingPanBox"));

	RootComponent = NormalWeapon;
	BoxComponent->SetupAttachment(NormalWeapon);

	BoxComponent->SetRelativeLocation(FVector(0.f, 50.f, 120.f));
	BoxComponent->SetRelativeRotation(FRotator(0.f, 0.f, 90.f));
	BoxComponent->SetRelativeScale3D(FVector(0.75f, 0.25f, 0.75f));


	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_FRYINGPAN(TEXT("/Game/Mesh/SM_FryingPan.SM_FryingPan"));
	if (SM_FRYINGPAN.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_FRYINGPAN.Object);
	}



	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	BoxComponent->SetCollisionProfileName(TEXT("NoCollision"));


	m_fWeaponSTR = 4.f;
	m_fWeaponDurability = 10.f;
	m_fWeaponRange = 2.f;

}
