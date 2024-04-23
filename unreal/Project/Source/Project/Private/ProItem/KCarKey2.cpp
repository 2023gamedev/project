// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/KCarKey2.h"

AKCarKey2::AKCarKey2()
{
	PrimaryActorTick.bCanEverTick = false;

	Key = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CarKey2"));

	RootComponent = Key;

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_CARKEY2(TEXT("/Game/Mesh/SM_Key.SM_Key"));
	if (SM_CARKEY2.Succeeded()) {
		Key->SetStaticMesh(SM_CARKEY2.Object);
	}

	Key->SetCollisionProfileName(TEXT("NoCollision"));


	KeyName = "CarKey2";
}
