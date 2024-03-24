// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/StartGameUI.h"

void UStartGameUI::OnStartButtonClicked()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "OnStartButtonClicked");

    MoveChoiceCharacterUI.Execute();

    RemoveFromParent();
}

void UStartGameUI::OnExitButtonClicked()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "OnExitButtonClicked");

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
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "StartButton Init");
        StartButton->SetClickMethod(EButtonClickMethod::DownAndUp);
        StartButton->OnClicked.AddDynamic(this, &UStartGameUI::OnStartButtonClicked);

        StartButton->SetIsEnabled(true);
        //StartButton->SetVisibility(ESlateVisibility::Hidden);

        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "StartButton Init End");
    }

    if (ExitButton)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "ExitButton Init");

        ExitButton->SetClickMethod(EButtonClickMethod::DownAndUp);
        ExitButton->OnClicked.AddDynamic(this, &UStartGameUI::OnExitButtonClicked);
        ExitButton->SetIsEnabled(true);

        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "ExitButton Init End");
    }
}
