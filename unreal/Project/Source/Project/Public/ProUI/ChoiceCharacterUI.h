// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "ProGamemode/ProGameInstance.h"
#include "ChoiceCharacterUI.generated.h"

/**
 * 
 */
DECLARE_DELEGATE(FChoicedGirl);
DECLARE_DELEGATE(FChoicedEmployee);
DECLARE_DELEGATE(FChoicedIdol);
DECLARE_DELEGATE(FChoicedFireFighter);


// Lobby에서 캐릭터를 선택하는데에 쓰이느 UI
UCLASS()
class PROJECT_API UChoiceCharacterUI : public UUserWidget
{
	GENERATED_BODY()
public:

	void Init();

	void HandleAllReady();

	void UpdateSelectImage();

	UProGameInstance* GameInstance;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* ReadyButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* First_Image ;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* Second_Image;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* Third_Image;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* Fourth_Image;

	bool b_ready = true;

	CharacterSelect recvSelect;

private:
	UFUNCTION(BlueprintCallable)
	void OnClickedGirlButton();

	UFUNCTION(BlueprintCallable)
	void OnClickedEmployeeButton();

	UFUNCTION(BlueprintCallable)
	void OnClickedIdolButton();

	UFUNCTION(BlueprintCallable)
	void OnClickedFireFighterButton();

	UFUNCTION(BlueprintCallable)
	void OnClickedReadyButton();
};
