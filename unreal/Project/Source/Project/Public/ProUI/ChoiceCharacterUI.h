// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "ChoiceCharacterUI.generated.h"

/**
 * 
 */
DECLARE_DELEGATE(FChoicedGirl);
DECLARE_DELEGATE(FChoicedEmployee);
DECLARE_DELEGATE(FChoicedIdol);
DECLARE_DELEGATE(FChoicedFireFighter);



UCLASS()
class PROJECT_API UChoiceCharacterUI : public UUserWidget
{
	GENERATED_BODY()
public:

	void Init();

	FChoicedGirl ChoicedGirl;
	FChoicedEmployee ChoicedEmployee;
	FChoicedIdol ChoicedIdol;
	FChoicedFireFighter ChoicedFireFighter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* GirlButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* EmployeeButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* IdolButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* FireFighterButton;


private:
	UFUNCTION(BlueprintCallable)
	void OnClickedGirlButton();

	UFUNCTION(BlueprintCallable)
	void OnClickedEmployeeButton();

	UFUNCTION(BlueprintCallable)
	void OnClickedIdolButton();

	UFUNCTION(BlueprintCallable)
	void OnClickedFireFighterButton();
};
