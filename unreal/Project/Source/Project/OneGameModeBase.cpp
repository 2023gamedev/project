// Fill out your copyright notice in the Description page of Project Settings.


#include "OneGameModeBase.h"
#include "BaseCharacter.h"
#include "PlayerCharacterController.h"


AOneGameModeBase::AOneGameModeBase()
{
	DefaultPawnClass = ABaseCharacter::StaticClass();
	PlayerControllerClass = APlayerCharacterController::StaticClass();

	
}

void AOneGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}
