// Copyright Epic Games, Inc. All Rights Reserved.

#include "RealProjectGameMode.h"
#include "RealProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARealProjectGameMode::ARealProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
