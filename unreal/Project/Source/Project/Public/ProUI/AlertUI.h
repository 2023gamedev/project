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
	//{
	//	RemoveFromParent();
	//}

public:

	void Init();
	//{
	//	if (CloseButton)
	//	{
	//		CloseButton->SetClickMethod(EButtonClickMethod::DownAndUp);
	//		CloseButton->OnClicked.AddDynamic(this, &UAlertUI::OnCloseButtonClicked);
	//		CloseButton->SetIsEnabled(true);
	//	}
	//}

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UEditableTextBox* AlertText;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* CloseButton;
	
};
