// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/HWater.h"

AHWater::AHWater()
{
	PrimaryActorTick.bCanEverTick = false;

	HealingItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Water"));
	RootComponent = HealingItem;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_WATER(TEXT("/Game/TMSportsEquipmentPack2/SportsEquipment/meshes/EnergyDrink01Can02/SM_EnergyDrink01Can02.SM_EnergyDrink01Can02"));
	if (SM_WATER.Succeeded()) {
		HealingItem->SetStaticMesh(SM_WATER.Object);
	}


	HealingItem->SetCollisionProfileName(TEXT("NoCollision"));


	m_fHealingSpeed		= 1.f;
	m_fHealingDuration  = 10.f;

	HName = "Water";
}
