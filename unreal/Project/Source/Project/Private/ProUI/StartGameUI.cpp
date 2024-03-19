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
    QuitGame.Execute();
}

void UStartGameUI::Init()
{
    if (StartButton)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "StartButton Init");
        StartButton->SetClickMethod(EButtonClickMethod::DownAndUp);
        StartButton->OnClicked.AddUniqueDynamic(this, &UStartGameUI::OnStartButtonClicked);

        StartButton->SetIsEnabled(true);
        //StartButton->SetVisibility(ESlateVisibility::Hidden);

        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "StartButton Init End");
    }

    if (ExitButton)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "ExitButton Init");

        ExitButton->SetClickMethod(EButtonClickMethod::DownAndUp);
        ExitButton->OnClicked.AddUniqueDynamic(this, &UStartGameUI::OnExitButtonClicked);
        ExitButton->SetIsEnabled(true);

        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "ExitButton Init End");
    }
}
