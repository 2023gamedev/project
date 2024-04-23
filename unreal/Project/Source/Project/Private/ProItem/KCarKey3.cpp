// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/KCarKey3.h"

AKCarKey3::AKCarKey3()
{
	PrimaryActorTick.bCanEverTick = false;

	Key = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CarKey3"));

	RootComponent = Key;

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_CARKEY3(TEXT("/Game/Mesh/SM_Key.SM_Key"));
	if (SM_CARKEY3.Succeeded()) {
		Key->SetStaticMesh(SM_CARKEY3.Object);
	}

	Key->SetCollisionProfileName(TEXT("NoCollision"));


	KeyName = "CarKey3";
}
