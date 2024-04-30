// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "ProCharacter/BaseCharacter.h"
#include "BaseUI.generated.h"

/**
 * 
 */
// 상속기능이 필요할때 사용되는 베이스 UI
UCLASS()
class PROJECT_API UBaseUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UBaseUI(const FObjectInitializer& ObjectInitializer);

	virtual void Init() {};
	virtual void Update() {};

	virtual void NativeConstruct() override;

public:
	ABaseCharacter* Character;
};
