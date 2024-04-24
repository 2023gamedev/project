// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/HSmoke.h"

AHSmoke::AHSmoke()
{
	PrimaryActorTick.bCanEverTick = false;

	HealingItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Smoke"));
	RootComponent = HealingItem;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SMOKE(TEXT("/Game/TMSportsEquipmentPack2/SportsEquipment/meshes/EnergyDrink01Can02/SM_EnergyDrink01Can02.SM_EnergyDrink01Can02"));
	if (SM_SMOKE.Succeeded()) {
		HealingItem->SetStaticMesh(SM_SMOKE.Object);
	}


	HealingItem->SetCollisionProfileName(TEXT("NoCollision"));


	m_fHealingSpeed		= 1.f;
	m_fHealingDuration  = 5.f;

	HName = "Smoke";
}
