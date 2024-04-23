// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/KRoofKey2.h"

AKRoofKey2::AKRoofKey2()
{
	PrimaryActorTick.bCanEverTick = false;

	Key = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RoofKey2"));

	RootComponent = Key;

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_ROOFKEY2(TEXT("/Game/Mesh/SM_Key.SM_Key"));
	if (SM_ROOFKEY2.Succeeded()) {
		Key->SetStaticMesh(SM_ROOFKEY2.Object);
	}

	Key->SetCollisionProfileName(TEXT("NoCollision"));


	KeyName = "RoofKey2";
}
