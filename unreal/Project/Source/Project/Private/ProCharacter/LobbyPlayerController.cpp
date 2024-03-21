// Fill out your copyright notice in the Description page of Project Settings.


#include "ProCharacter/LobbyPlayerController.h"
#include "ProCharacter/LobbyPlayer.h"



ALobbyPlayerController::ALobbyPlayerController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

}

void ALobbyPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ALobbyPlayerController::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked(ButtonPressed);
}

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ALobbyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALobbyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("MouseLeftClick", IE_Pressed, this, &ALobbyPlayerController::OnMouseLeftClick);
}

void ALobbyPlayerController::OnMouseLeftClick()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Left Mouse Button Clicked");
	ALobbyPlayer* LobbyPlayer = Cast<ALobbyPlayer> (GetPawn());

	LobbyPlayer->OnMouseLeftClick();
}
