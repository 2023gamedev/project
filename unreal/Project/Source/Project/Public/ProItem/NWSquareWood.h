// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NormalWeaponActor.h"
#include "NWSquareWood.generated.h"

/**
 * 
 */
// ¸ñ°¢
UCLASS()
class PROJECT_API ANWSquareWood : public ANormalWeaponActor
{
	GENERATED_BODY()
public:
	virtual void PlaceItem() override;

	// Sets default values for this actor's properties
	ANWSquareWood();

};
