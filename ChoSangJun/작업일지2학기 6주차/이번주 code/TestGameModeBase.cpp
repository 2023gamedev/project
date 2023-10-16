// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGameModeBase.h"
#include "PlayerCharacter.h"
#include "TestPlayerController.h"

ATestGameModeBase::ATestGameModeBase()
{
	DefaultPawnClass = APlayerCharacter::StaticClass();
	PlayerControllerClass = ATestPlayerController::StaticClass();
}

void ATestGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}
