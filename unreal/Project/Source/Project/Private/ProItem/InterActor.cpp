// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/InterActor.h"

// Sets default values
AInterActor::AInterActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AInterActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInterActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

