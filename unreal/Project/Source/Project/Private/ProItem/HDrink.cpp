// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/HDrink.h"

AHDrink::AHDrink()
{
	PrimaryActorTick.bCanEverTick = false;

	HealingItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Drink"));
	RootComponent = HealingItem;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_DRINK(TEXT("/Game/TMSportsEquipmentPack2/SportsEquipment/meshes/EnergyDrink01Can02/SM_EnergyDrink01Can02.SM_EnergyDrink01Can02"));
	if (SM_DRINK.Succeeded()) {
		HealingItem->SetStaticMesh(SM_DRINK.Object);
	}




	HealingItem->SetCollisionProfileName(TEXT("NoCollision"));

	m_fHealingSpeed		= 1.f;
	m_fHealingDuration  = 15.f;

	HName = "Drink";
}
