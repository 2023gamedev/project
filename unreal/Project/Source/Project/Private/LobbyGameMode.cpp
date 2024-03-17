// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

ALobbyGameMode::ALobbyGameMode()
{
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}


