// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWButchersKnife.h"

ANWButchersKnife::ANWButchersKnife()
{

	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButchersKnife"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("ButchersKnifeBox"));
	PlaneComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneComponent"));

	RootComponent = NormalWeapon;
	BoxComponent->SetupAttachment(NormalWeapon);
	PlaneComponent->SetupAttachment(BoxComponent);
	
	BoxComponent->SetRelativeLocation(FVector(30.4f, -10.19f, 1.f));
	BoxComponent->SetRelativeRotation(FRotator(-90.f, 180.0f, -150.f));
	BoxComponent->SetRelativeScale3D(FVector(0.03f, 0.25f, 0.5f));
	
	PlaneComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	PlaneComponent->SetRelativeRotation(FRotator(0.f, 90.f, 90.f));
	PlaneComponent->SetRelativeScale3D(FVector(5.3275f, 1.275f, 1.0f));
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BUTCHERSKNIFE(TEXT("/Game/Mesh/SM_ButherKnife.SM_ButherKnife"));
	if (SM_BUTCHERSKNIFE.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_BUTCHERSKNIFE.Object);
	}
	ConstructorHelpers::FObjectFinder<UStaticMesh> PLANE(TEXT("/Engine/BasicShapes/Plane.Plane"));
	if (PLANE.Succeeded()) {
		PlaneComponent->SetStaticMesh(PLANE.Object);
	}

	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	BoxComponent->SetCollisionProfileName(TEXT("NoCollision"));
	PlaneComponent->SetCollisionProfileName(TEXT("NoCollision"));

	PlaneComponent->SetHiddenInGame(true);

	m_fWeaponSTR = 4.f;
	m_fWeaponDurability = 7.f;
	m_fWeaponRange = 1.f;
	WeaponName = "ButchersKnife";
}
