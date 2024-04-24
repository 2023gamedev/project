// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/KCarKey4.h"

AKCarKey4::AKCarKey4()
{
	PrimaryActorTick.bCanEverTick = false;

	Key = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CarKey4"));

	RootComponent = Key;

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_CARKEY4(TEXT("/Game/Mesh/SM_Key.SM_Key"));
	if (SM_CARKEY4.Succeeded()) {
		Key->SetStaticMesh(SM_CARKEY4.Object);
	}

	Key->SetCollisionProfileName(TEXT("NoCollision"));


	KeyName = "CarKey4";
}
