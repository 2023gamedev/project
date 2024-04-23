// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/HCannedTuna.h"

AHCannedTuna::AHCannedTuna()
{
	PrimaryActorTick.bCanEverTick = false;

	HealingItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannedTuna"));
	RootComponent = HealingItem;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_CANNEDTUNA(TEXT("/Game/TMSportsEquipmentPack2/SportsEquipment/meshes/EnergyDrink01Can02/SM_EnergyDrink01Can02.SM_EnergyDrink01Can02"));
	if (SM_CANNEDTUNA.Succeeded()) {
		HealingItem->SetStaticMesh(SM_CANNEDTUNA.Object);
	}


	HealingItem->SetCollisionProfileName(TEXT("NoCollision"));

	m_fHealingSpeed		= 3.f;
	m_fHealingDuration  = 8.f;

	HName = "CannedTuna";
}
