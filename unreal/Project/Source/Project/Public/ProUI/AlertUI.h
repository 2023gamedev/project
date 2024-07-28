// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "AlertUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UAlertUI : public UUserWidget
{
	GENERATED_BODY()

private:

	UFUNCTION(BlueprintCallable)
	void OnCloseButtonClicked();

public:

	void Init();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UEditableTextBox* AlertText;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* CloseButton;
	
};
