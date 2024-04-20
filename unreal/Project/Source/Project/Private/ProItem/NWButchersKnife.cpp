// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWButchersKnife.h"

ANWButchersKnife::ANWButchersKnife()
{

	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButchersKnife"));
	RootComponent = NormalWeapon;

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BUTCHERSKNIFE(TEXT("/Game/Mesh/SM_ButherKnife.SM_ButherKnife"));
	if (SM_BUTCHERSKNIFE.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_BUTCHERSKNIFE.Object);
	}

	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));

	m_fWeaponSTR = 4.f;
	m_fWeaponDurability = 7.f;
	m_fWeaponRange = 1.f;

}
