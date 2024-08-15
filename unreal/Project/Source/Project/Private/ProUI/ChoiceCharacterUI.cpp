// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/ChoiceCharacterUI.h"
#include "Kismet/GameplayStatics.h"
#include "ProGamemode/LobbyGameMode.h"
#include "Components/CanvasPanelSlot.h"

void UChoiceCharacterUI::OnClickedGirlButton()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "OnClickedGirlButton");

    ChoicedGirl.Execute();
}
void UChoiceCharacterUI::OnClickedEmployeeButton()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "OnClickedEmployeeButton");

    ChoicedEmployee.Execute();
}

void UChoiceCharacterUI::OnClickedIdolButton()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "OnClickedIdolButton");

    ChoicedIdol.Execute();
}

void UChoiceCharacterUI::OnClickedFireFighterButton()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "OnClickedFireFighterButton");

    ChoicedFireFighter.Execute();
}

void UChoiceCharacterUI::OnClickedReadyButton()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "OnClickedReadyButton");

    uint32 MyPlayerId = GameInstance->ClientSocketPtr->GetMyPlayerId();
    Protocol::CS_Ready Packet;

    Packet.set_playerid(MyPlayerId);
    Packet.set_type(5);
    Packet.set_ready(b_ready);

    // 직렬화
    std::string serializedData;
    Packet.SerializeToString(&serializedData);

    // 직렬화된 데이터를 서버로 전송
    bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());

    b_ready = !b_ready;

}

void UChoiceCharacterUI::HandleAllReady()
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "All players are ready!");

    RemoveFromParent();
}

void UChoiceCharacterUI::UpdateSelectImage(CharacterSelect recvSelect)
{
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "NewTexture?");
    UTexture2D* NewTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/UI/testgirl.testgirl"));
    if (NewTexture)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "NewTexture!");
        if (recvSelect.PlayerId == 1)
        {
            First_Image->SetBrushFromTexture(NewTexture);
        }
        else if (recvSelect.PlayerId == 2)
        {
            Second_Image->SetBrushFromTexture(NewTexture);
        }
        else if (recvSelect.PlayerId == 3)
        {
            Third_Image->SetBrushFromTexture(NewTexture);
        }
        else if (recvSelect.PlayerId == 4)
        {
            Fourth_Image->SetBrushFromTexture(NewTexture);
        }
    }
}


void UChoiceCharacterUI::Init()
{

    GameInstance = Cast<UProGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));


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

    if (ReadyButton)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "ReadyButton Init");

        ReadyButton->SetClickMethod(EButtonClickMethod::DownAndUp);
        ReadyButton->OnClicked.AddUniqueDynamic(this, &UChoiceCharacterUI::OnClickedReadyButton);
        ReadyButton->SetIsEnabled(true);

        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "ReadyButton Init End");
    }
}


