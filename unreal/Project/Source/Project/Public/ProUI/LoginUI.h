#pragma once
// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "ProGamemode/ProGameInstance.h"
#include "LoginUI.generated.h"

/**
 * 
 */

class UAlertUI;

DECLARE_DELEGATE(FMoveStartGameUI);

UCLASS()
class ULoginUI : public UUserWidget
{
	GENERATED_BODY()

public:

	void Init();

	void ShowAlert(const FString& Message);

	FMoveStartGameUI MoveStartGameUI;

	UProGameInstance* GameInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* LoginButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* RegisterButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UEditableTextBox* IDBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UEditableTextBox* PasswordBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UAlertUI> AlertUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UAlertUI* AlertUIWidget;

private:

	UFUNCTION(BlueprintCallable)
	void OnLoginButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnRegisterButtonClicked();

};
