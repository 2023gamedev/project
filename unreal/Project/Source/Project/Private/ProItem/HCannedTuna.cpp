// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/HCannedTuna.h"

AHCannedTuna::AHCannedTuna()
{
	PrimaryActorTick.bCanEverTick = false;

	HealingItem = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HEALINGITEM"));
	RootComponent = HealingItem;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_HEALINGITEM(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Axes/Blade_Hatchet02/SK_Blade_Hatchet02.SK_Blade_Hatchet02"));
	if (SK_HEALINGITEM.Succeeded()) {
		HealingItem->SetSkeletalMesh(SK_HEALINGITEM.Object);
	}


	HealingItem->SetCollisionProfileName(TEXT("NoCollision"));

	m_fHealingSpeed		= 3.f;
	m_fHealingDuration  = 8.f;
}
