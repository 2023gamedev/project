// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWButchersKnife.h"

ANWButchersKnife::ANWButchersKnife()
{

	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButchersKnife"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("ButchersKnifeBox"));

	RootComponent = NormalWeapon;
	BoxComponent->SetupAttachment(NormalWeapon);

	BoxComponent->SetRelativeLocation(FVector(30.4f, -10.19f, 1.f));
	BoxComponent->SetRelativeRotation(FRotator(-90.f, 90.f, -120.f));
	BoxComponent->SetRelativeScale3D(FVector(0.03f, 0.25f, 0.5f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BUTCHERSKNIFE(TEXT("/Game/Mesh/SM_ButherKnife.SM_ButherKnife"));
	if (SM_BUTCHERSKNIFE.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_BUTCHERSKNIFE.Object);
	}

	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	BoxComponent->SetCollisionProfileName(TEXT("NoCollision"));


	m_fWeaponSTR = 4.f;
	m_fWeaponDurability = 7.f;
	m_fWeaponRange = 1.f;

}
