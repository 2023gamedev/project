// Fill out your copyright notice in the Description page of Project Settings.


#include "ProGamemode/GameModeManager.h"
#include "ProGamemode/LobbyGameMode.h"
#include "ProGamemode/OneGameModeBase.h"

AGameModeManager::AGameModeManager()
{
    NewGameModeClass = nullptr;
}

void AGameModeManager::ChangeGameMode(TSubclassOf<AGameModeBase> NewGameMode)
{
    // Set the new game mode class
    NewGameModeClass = NewGameMode;
}