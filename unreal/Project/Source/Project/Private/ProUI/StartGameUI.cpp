// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/StartGameUI.h"

void UStartGameUI::OnStartButtonClicked()
{

    MoveChoiceCharacterUI.Execute();

    RemoveFromParent();
}

void UStartGameUI::OnExitButtonClicked()
{

    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        // 'quit' 명령을 실행하여 게임 종료
        PlayerController->ConsoleCommand("quit");
    }

}

void UStartGameUI::Init()
{
    if (StartButton)
    {
        StartButton->SetClickMethod(EButtonClickMethod::DownAndUp);
        StartButton->OnClicked.AddDynamic(this, &UStartGameUI::OnStartButtonClicked);

        StartButton->SetIsEnabled(true);

    }

    if (ExitButton)
    {

        ExitButton->SetClickMethod(EButtonClickMethod::DownAndUp);
        ExitButton->OnClicked.AddDynamic(this, &UStartGameUI::OnExitButtonClicked);
        ExitButton->SetIsEnabled(true);

    }
}
