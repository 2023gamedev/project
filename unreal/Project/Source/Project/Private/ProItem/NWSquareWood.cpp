// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWSquareWood.h"



ANWSquareWood::ANWSquareWood()
{
	PrimaryActorTick.bCanEverTick = false;
	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NORMALWEAPON"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BOX"));
	
	RootComponent = NormalWeapon;
	BoxComponent->SetupAttachment(NormalWeapon);

	BoxComponent->SetRelativeLocation(FVector(0.f, 20.f, 70.f));
	BoxComponent->SetRelativeScale3D(FVector(0.25f, 0.25f, 1.0f));



	 ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SQUREWOOD(TEXT("/Game/StorePropsCollection/StaticMeshes/OfficePapers/SM_Book4.SM_Book4"));
	if (SM_SQUREWOOD.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_SQUREWOOD.Object);
	}


	NormalWeapon->SetCollisionProfileName(TEXT("Attack")); // �浹 �������� ����� ���� �ִ�.

	m_fWeaponSTR		= 2.f;
	m_fWeaponDurability = 8.f;
	m_fWeaponRange		= 3.f;

}

void ANWSquareWood::PlaceItem()
{
}