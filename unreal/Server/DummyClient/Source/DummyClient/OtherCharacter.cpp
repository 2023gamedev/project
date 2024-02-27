// Fill out your copyright notice in the Description page of Project Settings.


#include "OtherCharacter.h"

// Sets default values
AOtherCharacter::AOtherCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AOtherCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOtherCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AOtherCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

uint32 AOtherCharacter::GetPlayerId() const
{
	return PlayerId;
}

void AOtherCharacter::SetPlayerId(uint32 NewPlayerId)
{
	PlayerId = NewPlayerId;
}

