// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/LoginUI.h"
#include "Kismet/GameplayStatics.h"
#include "LStruct.pb.h"

void ULoginUI::ShowAlert(const FString& Message)
{
    if (AlertUI == nullptr)
    {
        static ConstructorHelpers::FClassFinder<UAlertUI> PLAYER_ALERTUI(TEXT("/Game/UI/WBP_Alert"));
        if (PLAYER_ALERTUI.Succeeded()) {
            AlertUI = PLAYER_ALERTUI.Class;
        }
    }

    if (!AlertUI)
    {
        UE_LOG(LogTemp, Warning, TEXT("AlertUIClass is not set"));
        return;
    }

    if (!AlertUIWidget)
    {
        AlertUIWidget = CreateWidget<UAlertUI>(GetWorld(), AlertUI);
        if (AlertUIWidget != nullptr)
        {
            AlertUIWidget->AddToViewport();

            // AlertText 설정
            UTextBlock* AlertText = Cast<UTextBlock>(AlertUIWidget->GetWidgetFromName(TEXT("AlertText")));
            if (AlertText != nullptr)
            {
                AlertText->SetText(FText::FromString(Message));
            }
        }
    }

	if (AlertUIWidget)
	{
		AlertUIWidget->AddToViewport();
		UEditableTextBox* AlertText = Cast<UEditableTextBox>(AlertUIWidget->GetWidgetFromName(TEXT("AlertText")));
		if (AlertText)
		{
			AlertText->SetText(FText::FromString(Message));
		}

		// Add Close button click event binding
		if (AlertUIWidget->CloseButton)
		{
			AlertUIWidget->CloseButton->OnClicked.AddDynamic(this, &ULoginUI::OnCloseAlert);
		}
	}
    else
    {
        UEditableTextBox* AlertText = Cast<UEditableTextBox>(AlertUIWidget->GetWidgetFromName(TEXT("AlertText")));
        if (AlertText)
        {
            AlertText->SetText(FText::FromString(Message));
        }
    }
}

void ULoginUI::OnCloseAlert()
{
    if (AlertUIWidget)
    {
        AlertUIWidget->RemoveFromParent();
        AlertUIWidget = nullptr;
    }
}


void ULoginUI::OnLoginButtonClicked()
{
	FString UEUsername = IDBox->GetText().ToString();
	FString UEPassword = PasswordBox->GetText().ToString();

    std::string Username = TCHAR_TO_UTF8(*UEUsername);
    std::string Password = TCHAR_TO_UTF8(*UEPassword);


    Protocol::CS_Login packet;

    packet.set_type(1);
    packet.set_id(Username);
    packet.set_password(Password);

    std::string serializedData;
    packet.SerializeToString(&serializedData);

    // 직렬화된 데이터를 서버로 전송
    bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());

    float timeout = 5.0f;
    float elapsedTime = 0.0f;

    while (!GameInstance->ClientSocketPtr->recv_login && elapsedTime < timeout) {
        FPlatformProcess::Sleep(0.01f);
        elapsedTime += 0.01f;
    }

    if (GameInstance->ClientSocketPtr->b_login) {
        MoveStartGameUI.Execute();

        RemoveFromParent();
    }
    else {
        ShowAlert(TEXT("Login Fail"));
    }

    GameInstance->ClientSocketPtr->recv_login = false;

}

void ULoginUI::OnRegisterButtonClicked()
{
	FString UEUsername = IDBox->GetText().ToString();
	FString UEPassword = PasswordBox->GetText().ToString();

    std::string Username = TCHAR_TO_UTF8(*UEUsername);
    std::string Password = TCHAR_TO_UTF8(*UEPassword);

    Protocol::CS_Register packet;

    packet.set_type(2);
    packet.set_id(Username);
    packet.set_password(Password);

    std::string serializedData;
    packet.SerializeToString(&serializedData);

    // 직렬화된 데이터를 서버로 전송
    bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());

    float timeout = 5.0f;
    float elapsedTime = 0.0f;

    while (!GameInstance->ClientSocketPtr->recv_register && elapsedTime < timeout) {
        FPlatformProcess::Sleep(0.01f);
        elapsedTime += 0.01f;
    }

    GameInstance->ClientSocketPtr->recv_register = false;

    if (GameInstance->ClientSocketPtr->b_register) {
        ShowAlert(TEXT("Register Success"));
    }
    else {
        ShowAlert(TEXT("Register Fail"));
    }
}

void ULoginUI::Init()
{

    GameInstance = Cast<UProGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

    if (LoginButton)
    {
        LoginButton->SetClickMethod(EButtonClickMethod::DownAndUp);
        LoginButton->OnClicked.AddDynamic(this, &ULoginUI::OnLoginButtonClicked);
        LoginButton->SetIsEnabled(true);

    }

    if (RegisterButton)
    {

        RegisterButton->SetClickMethod(EButtonClickMethod::DownAndUp);
        RegisterButton->OnClicked.AddDynamic(this, &ULoginUI::OnRegisterButtonClicked);
        RegisterButton->SetIsEnabled(true);

    }
}
