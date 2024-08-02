// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/LoginUI.h"
#include "Kismet/GameplayStatics.h"
#include "LStruct.pb.h"

void ULoginUI::ShowAlert(const FString& Message)
{   
    AlertUI = LoadClass<UAlertUI>(nullptr, TEXT("/Game/UI/BP_Alert.BP_Alert_C"));
    if (AlertUI == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Could not load AlertUI class"));
        return;
    }
    
    AlertUIWidget = CreateWidget<UAlertUI>(GetWorld(), AlertUI);
    if (AlertUIWidget != nullptr)
    {
        AlertUIWidget->AddToViewport();

        UEditableTextBox* AlertText = Cast<UEditableTextBox>(AlertUIWidget->GetWidgetFromName(TEXT("AlertText")));
        if (AlertText != nullptr)
        {
            AlertText->SetText(FText::FromString(Message));
        }
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
