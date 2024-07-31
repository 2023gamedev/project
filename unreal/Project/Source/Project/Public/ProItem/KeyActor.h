// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemActor.h"
#include "KeyActor.generated.h"

/**
 * 
 */

// 키 액터
class ABaseCharacter;

UCLASS()
class PROJECT_API AKeyActor : public AItemActor
{
	GENERATED_BODY()
public:
	AKeyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, Category = Key)
	UStaticMeshComponent* Key;

	//UPROPERTY(EditAnywhere, Category = "ItmePos")
	//FVector ItemHandPos;

	//UPROPERTY(EditAnywhere, Category = "ItmeRotate")
	//FRotator ItemHandRot;

	UPROPERTY(EditAnywhere)
	ABaseCharacter* OwnerCharacter;

	UPROPERTY(EditAnywhere)
	FName KeyName;

};
