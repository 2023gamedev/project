// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/KRoofKey1.h"

AKRoofKey1::AKRoofKey1()
{
	PrimaryActorTick.bCanEverTick = false;

	Key = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RoofKey1"));

	RootComponent = Key;

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_ROOFKEY1(TEXT("/Game/Mesh/SM_Key.SM_Key"));
	if (SM_ROOFKEY1.Succeeded()) {
		Key->SetStaticMesh(SM_ROOFKEY1.Object);
	}

	Key->SetCollisionProfileName(TEXT("NoCollision"));


	KeyName = "RoofKey1";
}
