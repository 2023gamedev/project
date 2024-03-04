// Fill out your copyright notice in the Description page of Project Settings.


#include "BHClothes.h"

ABHClothes::ABHClothes()
{	
	//PrimaryActorTick.bCanEverTick = false;

	//NormalWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("NORMALWEAPON"));
	//RootComponent = NormalWeapon;

	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_NORMALWEAPON(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Axes/Blade_Hatchet02/SK_Blade_Hatchet02.SK_Blade_Hatchet02"));
	//if (SK_NORMALWEAPON.Succeeded()) {
	//	NormalWeapon->SetSkeletalMesh(SK_NORMALWEAPON.Object);
	//}


	//NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));

	m_fHealingSuccessProbability = 0.5f;
}
