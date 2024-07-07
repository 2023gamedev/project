// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/LoginUI.h"

void ULoginUI::OnLoginButtonClicked()
{
	FString Username = UsernameTextBox->GetText().ToString();
	FString Password = PasswordTextBox->GetText().ToString();
}

void ULoginUI::OnRegisterButtonClicked()
{
	FString Username = UsernameTextBox->GetText().ToString();
	FString Password = PasswordTextBox->GetText().ToString();
}
