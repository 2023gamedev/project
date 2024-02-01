// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DummyClientCharacter.h"

/**
 * 
 */
class DUMMYCLIENT_API GameSyncManager
{
public:
	GameSyncManager();
	~GameSyncManager();

	FVector PlayerLocation;

	void Player(const FVector& NewLocation);
};
