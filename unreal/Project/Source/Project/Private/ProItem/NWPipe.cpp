// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NWPipe.h"

ANWPipe::ANWPipe()
{
	PrimaryActorTick.bCanEverTick = false;

	NormalWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pipe"));
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("PipeBox"));

	RootComponent = NormalWeapon;
	BoxComponent->SetupAttachment(NormalWeapon);

	BoxComponent->SetRelativeLocation(FVector(10.f, 0.f, 0.f));
	BoxComponent->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	BoxComponent->SetRelativeScale3D(FVector(0.15f, 0.15f, 0.4f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_PIPE(TEXT("/Game/Safe_House/meshes/SM_Pipe.SM_Pipe"));
	if (SM_PIPE.Succeeded()) {
		NormalWeapon->SetStaticMesh(SM_PIPE.Object);
	}


	NormalWeapon->SetCollisionProfileName(TEXT("NoCollision"));
	BoxComponent->SetCollisionProfileName(TEXT("NoCollision"));

	m_fWeaponSTR		= 2.f;
	m_fWeaponDurability = 9.f;
	m_fWeaponRange		= 3.f;
	WeaponName = "Pipe";
}
