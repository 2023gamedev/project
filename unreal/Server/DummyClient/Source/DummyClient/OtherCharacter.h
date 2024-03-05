// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyAnimInstance.h"
#include "ClientSocket.h"
#include "DummyClientGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "OtherCharacter.generated.h"

UCLASS()
class DUMMYCLIENT_API AOtherCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AOtherCharacter();

	void SetClientSocket(ClientSocket* InClientSocket)
	{
		ClientSocketPtr = InClientSocket;
	}

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

private:

	ClientSocket* ClientSocketPtr;

	uint32 PlayerId;

	PlayerData recvPlayerData;

};
