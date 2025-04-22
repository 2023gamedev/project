// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWBook.h"

ANWBook::ANWBook()
{

	PrimaryActorTick.bCanEverTick = false; 

	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Book"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BookBox"));

	RootComponent = NormalWeapon;
	BoxComponent->SetupAttachment(NormalWeapon);

	BoxComponent->SetRelativeLocation(FVector(0.f, 0.f, 2.f));
	BoxComponent->SetRelativeScale3D(FVector(0.2f, 0.28f, 0.15f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BOOK(TEXT("/Game/StorePropsCollection/StaticMeshes/OfficePapers/SM_Book4.SM_Book4"));
	if (SM_BOOK.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_BOOK.Object);
	}

	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	BoxComponent->SetCollisionProfileName(TEXT("NoCollision"));

	m_fWeaponSTR = 2.f;
	m_fWeaponDurability = WeaponDurability::Book;
	m_fWeaponDurability_Max = m_fWeaponDurability;
	m_fWeaponRange = 0.5f;
	WeaponName = "Book";

}
