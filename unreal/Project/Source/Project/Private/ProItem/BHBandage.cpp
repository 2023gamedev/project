// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/BHBandage.h"

ABHBandage::ABHBandage()
{
	PrimaryActorTick.bCanEverTick = false;

	BleedingHealingItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NORMALWEAPON"));
	RootComponent = BleedingHealingItem;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SK_BLEEDINGHEALINGITEM(TEXT("/Game/Mesh/SM_Band.SM_Band"));
	if (SK_BLEEDINGHEALINGITEM.Succeeded()) {
		BleedingHealingItem->SetStaticMesh(SK_BLEEDINGHEALINGITEM.Object);
	}


	BleedingHealingItem->SetCollisionProfileName(TEXT("NoCollision"));

	m_fHealingSuccessProbability = 0.8f;
}
