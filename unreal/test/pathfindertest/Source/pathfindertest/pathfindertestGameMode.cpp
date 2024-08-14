// Copyright Epic Games, Inc. All Rights Reserved.

#include "pathfindertestGameMode.h"
#include "pathfindertestCharacter.h"
#include "UObject/ConstructorHelpers.h"

ApathfindertestGameMode::ApathfindertestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
