// Fill out your copyright notice in the Description page of Project Settings.

#include "ProUI/AlertUI.h"


void UAlertUI::OnCloseButtonClicked()
{
<<<<<<< HEAD
=======
	UE_LOG(LogTemp, Warning, TEXT("Close button clicked"));
>>>>>>> 524c51fb7f427e390d4f5a16137597dfd8bcf475
	RemoveFromParent();
}

void UAlertUI::Init()
{
	if (CloseButton)
	{
		CloseButton->SetClickMethod(EButtonClickMethod::DownAndUp);
		CloseButton->OnClicked.AddDynamic(this, &UAlertUI::OnCloseButtonClicked);
		CloseButton->SetIsEnabled(true);
	}
}