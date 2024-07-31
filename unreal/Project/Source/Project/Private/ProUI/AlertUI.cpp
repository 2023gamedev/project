// Fill out your copyright notice in the Description page of Project Settings.

#include "ProUI/AlertUI.h"


void UAlertUI::OnCloseButtonClicked()
{

	//UE_LOG(LogTemp, Warning, TEXT("Close button clicked"));

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