// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/ChoiceCharacterUI.h"
#include "Components/CanvasPanelSlot.h"

void UChoiceCharacterUI::OnClickedGirlButton()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "OnClickedGirlButton");

    ChoicedGirl.Execute();

    RemoveFromParent();
}
void UChoiceCharacterUI::OnClickedEmployeeButton()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "OnClickedEmployeeButton");

    ChoicedEmployee.Execute();

    RemoveFromParent();
}

void UChoiceCharacterUI::OnClickedIdolButton()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "OnClickedIdolButton");

    ChoicedIdol.Execute();

    RemoveFromParent();
}

void UChoiceCharacterUI::OnClickedFireFighterButton()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "OnClickedFireFighterButton");

    ChoicedFireFighter.Execute();

    RemoveFromParent();
}

void UChoiceCharacterUI::Init()
{


    if (GirlButton)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "GirlButton Init");
        GirlButton->SetClickMethod(EButtonClickMethod::DownAndUp);
        GirlButton->OnClicked.AddUniqueDynamic(this, &UChoiceCharacterUI::OnClickedGirlButton);
        GirlButton->SetIsEnabled(true);
        //StartButton->SetVisibility(ESlateVisibility::Hidden);

        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "GirlButton Init End");
    }

    if (EmployeeButton)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "EmployeeButton Init");

        EmployeeButton->SetClickMethod(EButtonClickMethod::DownAndUp);
        EmployeeButton->OnClicked.AddUniqueDynamic(this, &UChoiceCharacterUI::OnClickedEmployeeButton);
        EmployeeButton->SetIsEnabled(true);

        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "EmployeeButton Init End");
    }

    if (IdolButton)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "IdolButton Init");
        IdolButton->SetClickMethod(EButtonClickMethod::DownAndUp);
        IdolButton->OnClicked.AddUniqueDynamic(this, &UChoiceCharacterUI::OnClickedIdolButton);
        IdolButton->SetIsEnabled(true);
        //StartButton->SetVisibility(ESlateVisibility::Hidden);

        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "IdolButton Init End");
    }

    if (FireFighterButton)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "FireFighterButton Init");

        FireFighterButton->SetClickMethod(EButtonClickMethod::DownAndUp);
        FireFighterButton->OnClicked.AddUniqueDynamic(this, &UChoiceCharacterUI::OnClickedFireFighterButton);
        FireFighterButton->SetIsEnabled(true);

        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "FireFighterButton Init End");
    }
}


