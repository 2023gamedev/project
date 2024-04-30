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
// ��ӱ���� �ʿ��Ҷ� ���Ǵ� ���̽� UI
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
