// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/HSnack.h"

AHSnack::AHSnack()
{
	PrimaryActorTick.bCanEverTick = false;

	HealingItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Snack"));
	RootComponent = HealingItem;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SNACK(TEXT("/Game/TMSportsEquipmentPack2/SportsEquipment/meshes/EnergyDrink01Can02/SM_EnergyDrink01Can02.SM_EnergyDrink01Can02"));
	if (SM_SNACK.Succeeded()) {
		HealingItem->SetStaticMesh(SM_SNACK.Object);
	}




	HealingItem->SetCollisionProfileName(TEXT("NoCollision"));


	m_fHealingSpeed		= 2.f;
	m_fHealingDuration  = 10.f;

	HName = "Snack";
}
