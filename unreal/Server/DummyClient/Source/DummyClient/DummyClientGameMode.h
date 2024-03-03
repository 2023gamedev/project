// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ClientSocket.h"
#include "DummyClientGameMode.generated.h"

UCLASS(minimalapi)
class ADummyClientGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADummyClientGameMode();

	void UpdateOtherPlayer(uint32 PlayerID, FVector NewLocation, FRotator NewRotation);

protected:
	UPROPERTY()
		TSubclassOf<ACharacter> BP_OtherCharacterClass;
};



