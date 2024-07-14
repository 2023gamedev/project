// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CircularPB_UI.generated.h"

/**
 * 
 */

UCLASS()
class PROJECT_API UCircularPB_UI : public UUserWidget
{
	GENERATED_BODY()

public:
	//UCircularPB_UI(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void FillAnimationsMap();

	UFUNCTION(BlueprintCallable)
	UWidgetAnimation* GetAnimationByName(FName AnimationName) const;
	
	UFUNCTION(BlueprintCallable)
	bool PlayAnimationByName(
		FName AnimationName,
		float StartAtTime = 0.f,
		int32 NumLoopsToPlay = 1,
		EUMGSequencePlayMode::Type PlayMode = EUMGSequencePlayMode::Forward,
		float PlaybackSpeed = 1.f);

protected:
	//virtual void BeginPlay() override;

	TMap<FName, UWidgetAnimation*> AnimationsMap;
};
