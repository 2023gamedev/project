// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

/**
 * 
 */
 // Lobby에서 UI를 고르는 Player 클래스를 컨트롤하는 컨트롤러
UCLASS()
class PROJECT_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ALobbyPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void PostInitializeComponents() override;
	//virtual void OnPossess(APawn* aPawn) override;

	virtual void NotifyActorOnClicked(FKey ButtonPressed) override;

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void SetupInputComponent() override;
	void OnMouseLeftClick();
};
