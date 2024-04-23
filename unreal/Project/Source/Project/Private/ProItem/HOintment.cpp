// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/HOintment.h"

AHOintment::AHOintment()
{
	PrimaryActorTick.bCanEverTick = false;

	HealingItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ointment"));
	RootComponent = HealingItem;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_OINTMENT(TEXT("/Game/TMSportsEquipmentPack2/SportsEquipment/meshes/EnergyDrink01Can02/SM_EnergyDrink01Can02.SM_EnergyDrink01Can02"));
	if (SM_OINTMENT.Succeeded()) {
		HealingItem->SetStaticMesh(SM_OINTMENT.Object);
	}





	HealingItem->SetCollisionProfileName(TEXT("NoCollision"));


	m_fHealingSpeed		= 2.f;
	m_fHealingDuration  = 15.f;

	HName = "Ointment";
}
