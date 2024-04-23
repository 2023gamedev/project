// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/KCarkey1.h"

AKCarkey1::AKCarkey1()
{
	PrimaryActorTick.bCanEverTick = false;

	Key = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CarKey1"));

	RootComponent = Key;

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_CARKEY1(TEXT("/Game/Mesh/SM_Key.SM_Key"));
	if (SM_CARKEY1.Succeeded()) {
		Key->SetStaticMesh(SM_CARKEY1.Object);
	}

	Key->SetCollisionProfileName(TEXT("NoCollision"));


	KeyName = "CarKey1";
}
