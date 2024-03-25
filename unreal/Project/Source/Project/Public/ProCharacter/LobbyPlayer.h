// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "LobbyPlayer.generated.h"

class UStartGameUI;
class UChoiceCharacterUI;

DECLARE_DELEGATE(FChoicedGirlCharcaterD);
DECLARE_DELEGATE(FChoicedEmployeeCharcaterD);
DECLARE_DELEGATE(FChoicedIdolCharcaterD);
DECLARE_DELEGATE(FChoicedFireFighterCharcaterD);

// Lobby에서 UI를 고르는 Player 클래스
UCLASS()
class PROJECT_API ALobbyPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ALobbyPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	FChoicedGirlCharcaterD ChoicedGirlCharcaterD;
	FChoicedEmployeeCharcaterD ChoicedEmployeeCharcaterD;
	FChoicedIdolCharcaterD ChoicedIdolCharcaterD;
	FChoicedFireFighterCharcaterD ChoicedFireFighterCharcaterD;
	
	void ChoicedGirlCharacter();
	void ChoicedEmployeeCharacter();
	void ChoicedIdolCharacter();
	void ChoicedFireFighterCharacter();
	
	void MoveChoiceCharacterUI();
	void OnMouseLeftClick();

	

	// 카메라
	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	UCapsuleComponent* Capsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UStartGameUI> StartGameUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UStartGameUI* StartGameUIWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UChoiceCharacterUI> ChoiceCharacterUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UChoiceCharacterUI* ChoiceCharacterUIWidget;

};
