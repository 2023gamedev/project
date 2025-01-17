// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "ProGamemode/ProGameInstance.h"
#include "Components/MultiLineEditableText.h"
#include "Components/MultiLineEditableTextBox.h"

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

	void UpdateSelectImage(CharacterSelect recvSelect);

	void UpdatePlayerReadyState(uint32 player_num, bool ready);

	UProGameInstance* GameInstance;

	FChoicedGirl ChoicedGirl;
	FChoicedEmployee ChoicedEmployee;
	FChoicedIdol ChoicedIdol;
	FChoicedFireFighter ChoicedFireFighter;

	TMap<uint32, uint32> PlayersCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* GirlButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* GirlBGButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* EmployeeButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* EmployeeBGButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* IdolButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* IdolBGButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* FireFighterButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* FireFighterBGButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UMultiLineEditableTextBox* CharDescription_DisAdvantage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UMultiLineEditableTextBox* CharDescription_Advantage;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* First_Player;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Second_Player;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Third_Player;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Fourth_Player;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock * First_Ready;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Second_Ready;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Third_Ready;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Fourth_Ready;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Ready_TextBlock;


	bool b_ready = false;


//protected:
	//virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;


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
