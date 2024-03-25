// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "StartGameUI.generated.h"

/**
 * 
 */
DECLARE_DELEGATE(FMoveChoiceCharacterUI);

// 시작할 때 나타나는 UI Start, Exit가 있다.
UCLASS()
class PROJECT_API UStartGameUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Init();


	FMoveChoiceCharacterUI MoveChoiceCharacterUI;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* StartButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* ExitButton;

private:
	UFUNCTION(BlueprintCallable)
	void OnStartButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnExitButtonClicked();

};
