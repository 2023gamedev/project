// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "OtherCharacter.generated.h"

UCLASS()
class DUMMYCLIENT_API AOtherCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AOtherCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual uint32 GetPlayerId() const;
	void SetPlayerId(uint32 NewPlayerId);

	void UpdatePlayerData(FVector Location);

private:
	uint32 PlayerId;

	FVector NewLocation;

	FVector OldLocation = FVector(0.0f, 0.0f, 0.0f);

	float Speed;
	float PreviousSpeed;

};
