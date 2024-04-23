// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/HDisinfectant.h"

AHDisinfectant::AHDisinfectant()
{
	PrimaryActorTick.bCanEverTick = false;

	HealingItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Disinfectant"));
	RootComponent = HealingItem;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_DISINFECTANT(TEXT("/Game/TMSportsEquipmentPack2/SportsEquipment/meshes/EnergyDrink01Can02/SM_EnergyDrink01Can02.SM_EnergyDrink01Can02"));
	if (SM_DISINFECTANT.Succeeded()) {
		HealingItem->SetStaticMesh(SM_DISINFECTANT.Object);
	}



	HealingItem->SetCollisionProfileName(TEXT("NoCollision"));

	m_fHealingSpeed		= 2.f;
	m_fHealingDuration  = 10.f;

	HName = "Disinfectant";
}
