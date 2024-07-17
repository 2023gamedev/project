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
	void StartVisibleAnimation(float PlaySpeed);

	UPROPERTY(Meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* CircularPB_ANIM;
};
