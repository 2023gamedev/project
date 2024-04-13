// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/HOintment.h"

AHOintment::AHOintment()
{
	PrimaryActorTick.bCanEverTick = false;

	HealingItem = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HEALINGITEM"));
	RootComponent = HealingItem;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_HEALINGITEM(TEXT("/Game/Mesh/SM_SquareWood.SM_SquareWood"));
	if (SK_HEALINGITEM.Succeeded()) {
		HealingItem->SetSkeletalMesh(SK_HEALINGITEM.Object);
	}


	HealingItem->SetCollisionProfileName(TEXT("NoCollision"));


	m_fHealingSpeed		= 2.f;
	m_fHealingDuration  = 15.f;
}
