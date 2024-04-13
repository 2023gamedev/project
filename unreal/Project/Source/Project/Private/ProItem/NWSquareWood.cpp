// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWSquareWood.h"



ANWSquareWood::ANWSquareWood()
{
	PrimaryActorTick.bCanEverTick = false;
	NormalWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("NORMALWEAPON"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BOX"));
	
	RootComponent = NormalWeapon;
	BoxComponent->SetupAttachment(NormalWeapon);

	BoxComponent->SetRelativeLocation(FVector(0.f, 20.f, 70.f));
	BoxComponent->SetRelativeScale3D(FVector(0.25f, 0.25f, 1.0f));



	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_NORMALWEAPON(TEXT("/Game/Mesh/SM_SquareWood.SM_SquareWood"));
	if (SK_NORMALWEAPON.Succeeded()) {
		NormalWeapon->SetSkeletalMesh(SK_NORMALWEAPON.Object);
	}


	NormalWeapon->SetCollisionProfileName(TEXT("Attack")); // 충돌 프리셋은 변경될 수도 있다.

	m_fWeaponSTR		= 2.f;
	m_fWeaponDurability = 8.f;
	m_fWeaponRange		= 3.f;

}

void ANWSquareWood::PlaceItem()
{
}