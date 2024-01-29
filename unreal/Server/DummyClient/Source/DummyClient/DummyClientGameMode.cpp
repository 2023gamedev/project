// Copyright Epic Games, Inc. All Rights Reserved.

#include "DummyClientGameMode.h"
#include "DummyClientCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADummyClientGameMode::ADummyClientGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
