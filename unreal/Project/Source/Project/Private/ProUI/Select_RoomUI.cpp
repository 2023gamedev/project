// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/Select_RoomUI.h"

void USelect_RoomUI::OnRoom1ButtonClicked()
{

    RemoveFromParent();
}

void USelect_RoomUI::OnRoom2ButtonClicked()
{

    RemoveFromParent();
}

void USelect_RoomUI::OnRoom3ButtonClicked()
{

    RemoveFromParent();
}

void USelect_RoomUI::OnRoom4ButtonClicked()
{

    RemoveFromParent();
}

void USelect_RoomUI::Init()
{
    if (Room1Button)
    {
        Room1Button->SetClickMethod(EButtonClickMethod::DownAndUp);
        Room1Button->OnClicked.AddDynamic(this, &USelect_RoomUI::OnRoom1ButtonClicked);

        Room1Button->SetIsEnabled(true);

    }

    if (Room2Button)
    {
        Room2Button->SetClickMethod(EButtonClickMethod::DownAndUp);
        Room2Button->OnClicked.AddDynamic(this, &USelect_RoomUI::OnRoom2ButtonClicked);

        Room2Button->SetIsEnabled(true);

    }

    if (Room3Button)
    {
        Room3Button->SetClickMethod(EButtonClickMethod::DownAndUp);
        Room3Button->OnClicked.AddDynamic(this, &USelect_RoomUI::OnRoom3ButtonClicked);

        Room3Button->SetIsEnabled(true);

    }

    if (Room4Button)
    {
        Room4Button->SetClickMethod(EButtonClickMethod::DownAndUp);
        Room4Button->OnClicked.AddDynamic(this, &USelect_RoomUI::OnRoom4ButtonClicked);

        Room4Button->SetIsEnabled(true);

    }
}