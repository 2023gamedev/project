// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/ChoiceCharacterUI.h"

#include "Kismet/GameplayStatics.h"
#include "ProGamemode/LobbyGameMode.h"
#include "Components/CanvasPanelSlot.h"
#include "ProCharacter/LobbyPlayer.h"

void UChoiceCharacterUI::OnClickedGirlButton()
{
    //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "OnClickedGirlButton");

    GirlBGButton->SetVisibility(ESlateVisibility::HitTestInvisible); // 보이지만 클릭 불가
    IdolBGButton->SetVisibility(ESlateVisibility::Hidden); // 숨김
    FireFighterBGButton->SetVisibility(ESlateVisibility::Hidden); // 숨김
    EmployeeBGButton->SetVisibility(ESlateVisibility::Hidden); // 숨김

    ChoicedGirl.Execute();
}

void UChoiceCharacterUI::OnClickedEmployeeButton()
{
    //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "OnClickedEmployeeButton");

    GirlBGButton->SetVisibility(ESlateVisibility::Hidden); // 숨김
    IdolBGButton->SetVisibility(ESlateVisibility::Hidden); // 숨김
    FireFighterBGButton->SetVisibility(ESlateVisibility::Hidden); // 숨김
    EmployeeBGButton->SetVisibility(ESlateVisibility::HitTestInvisible); // 보이지만 클릭 불가

    ChoicedEmployee.Execute();
}

void UChoiceCharacterUI::OnClickedIdolButton()
{
    //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "OnClickedIdolButton");

    GirlBGButton->SetVisibility(ESlateVisibility::Hidden); // 숨김
    IdolBGButton->SetVisibility(ESlateVisibility::HitTestInvisible); // 보이지만 클릭 불가
    FireFighterBGButton->SetVisibility(ESlateVisibility::Hidden); // 숨김
    EmployeeBGButton->SetVisibility(ESlateVisibility::Hidden); // 숨김

    ChoicedIdol.Execute();
}

void UChoiceCharacterUI::OnClickedFireFighterButton()
{
    //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "OnClickedFireFighterButton");

    GirlBGButton->SetVisibility(ESlateVisibility::Hidden); // 숨김
    IdolBGButton->SetVisibility(ESlateVisibility::Hidden); // 숨김
    FireFighterBGButton->SetVisibility(ESlateVisibility::HitTestInvisible); // 보이지만 클릭 불가
    EmployeeBGButton->SetVisibility(ESlateVisibility::Hidden); // 숨김

    ChoicedFireFighter.Execute();
}

void UChoiceCharacterUI::OnClickedReadyButton()
{
    //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "OnClickedReadyButton");

    uint32 MyPlayerId = GameInstance->ClientSocketPtr->GetMyPlayerId();
    Protocol::SelectReady Packet;

    Packet.set_playerid(MyPlayerId);
    Packet.set_type(15);
    Packet.set_ready(b_ready);

    // 직렬화
    std::string serializedData;
    Packet.SerializeToString(&serializedData);

    // 직렬화된 데이터를 서버로 전송
    bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());


    b_ready = !b_ready;

    if (b_ready == false) {
        if (GirlButton) GirlButton->SetIsEnabled(false);
        if (EmployeeButton) EmployeeButton->SetIsEnabled(false);
        if (IdolButton) IdolButton->SetIsEnabled(false);
        if (FireFighterButton) FireFighterButton->SetIsEnabled(false);

        Ready_TextBlock->SetText(FText::FromString("Cancel"));
    }
    else {
        if (GirlButton) GirlButton->SetIsEnabled(true);
        if (EmployeeButton) EmployeeButton->SetIsEnabled(true);
        if (IdolButton) IdolButton->SetIsEnabled(true);
        if (FireFighterButton) FireFighterButton->SetIsEnabled(true);

        Ready_TextBlock->SetText(FText::FromString("Ready"));
    }

}

void UChoiceCharacterUI::HandleAllReady()
{
    //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "All players are ready!");

    RemoveFromParent();
}

void UChoiceCharacterUI::UpdateSelectImage(CharacterSelect recvSelect)
{
    //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "NewTexture?");
    UTexture2D* NewTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/CharacterAsset/Girl/girl.girl"));
    if (NewTexture)
    {
        //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "NewTexture!");
        if (recvSelect.Character_type == 1) {
            NewTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/CharacterAsset/Girl/girl.girl"));
        }
        else if (recvSelect.Character_type == 2) {
            NewTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/CharacterAsset/Employee/emp.emp"));
        }
        else if (recvSelect.Character_type == 3) {
            NewTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/CharacterAsset/Idol/idol.idol"));
        }
        else if (recvSelect.Character_type == 4) {
            NewTexture = LoadObject<UTexture2D>(nullptr, TEXT("/Game/CharacterAsset/FireFighter/fire.fire"));
        }

        if (recvSelect.PlayerId == 1)
        {
            FSlateBrush Brush;
            Brush.SetResourceObject(NewTexture);   // 텍스처를 브러시에 설정
            First_Image->SetBrush(Brush);      // UImage에 브러시 설정
        }
        else if (recvSelect.PlayerId == 2)
        {
            FSlateBrush Brush;
            Brush.SetResourceObject(NewTexture);
            Second_Image->SetBrush(Brush);
        }
        else if (recvSelect.PlayerId == 3)
        {
            FSlateBrush Brush;
            Brush.SetResourceObject(NewTexture);
            Third_Image->SetBrush(Brush);
        }
        else if (recvSelect.PlayerId == 4)
        {
            FSlateBrush Brush;
            Brush.SetResourceObject(NewTexture);
            Fourth_Image->SetBrush(Brush);
        }
    }
}

void UChoiceCharacterUI::UpdatePlayerReadyState(uint32 player_num, bool Ready)
{
    if (Ready == true) {
        if (player_num == 1)
        {
            First_Ready->SetText(FText::FromString("READY"));
            FSlateFontInfo FontInfo = First_Ready->GetFont();
            FontInfo.Size = 50;
            First_Ready->SetJustification(ETextJustify::Center);
            First_Ready->SetColorAndOpacity(FSlateColor(FLinearColor::Green));
        }
        else if (player_num == 2)
        {
            Second_Ready->SetText(FText::FromString("READY"));
            FSlateFontInfo FontInfo = Second_Ready->GetFont();
            FontInfo.Size = 50;
            Second_Ready->SetJustification(ETextJustify::Center);
            Second_Ready->SetColorAndOpacity(FSlateColor(FLinearColor::Green));
        }
        else if (player_num == 3)
        {
            Third_Ready->SetText(FText::FromString("READY"));
            FSlateFontInfo FontInfo = Third_Ready->GetFont();
            FontInfo.Size = 50;
            Third_Ready->SetJustification(ETextJustify::Center);
            Third_Ready->SetColorAndOpacity(FSlateColor(FLinearColor::Green));
        }
        else if (player_num == 4)
        {
            Fourth_Ready->SetText(FText::FromString("READY"));
            FSlateFontInfo FontInfo = Fourth_Ready->GetFont();
            FontInfo.Size = 50;
            Fourth_Ready->SetJustification(ETextJustify::Center);
            Fourth_Ready->SetColorAndOpacity(FSlateColor(FLinearColor::Green));
        }
    }
    else {
        if (player_num == 1) {
            First_Ready->SetText(FText::FromString(""));
        }
        else if (player_num == 2) {
            Second_Ready->SetText(FText::FromString(""));
        }
        else if (player_num == 3) {
            Third_Ready->SetText(FText::FromString(""));
        }
        else if (player_num == 4) {
            Fourth_Ready->SetText(FText::FromString(""));
        }
    }
}

void UChoiceCharacterUI::Init()
{

    GameInstance = Cast<UProGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!PlayerPawn)
    {
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "PlayerPawn not found!");
        return;
    }

    ALobbyPlayer* LobbyPlayer = Cast<ALobbyPlayer>(PlayerPawn);
    if (!LobbyPlayer)
    {
        //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "LobbyPlayer not found!");
        return;
    }


    if (GirlButton)
    {
        //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "GirlButton Init");
        GirlButton->SetClickMethod(EButtonClickMethod::DownAndUp);
        GirlButton->OnClicked.AddUniqueDynamic(this, &UChoiceCharacterUI::OnClickedGirlButton); // [+] AddUniqueDynamic <-> AddDynamic: AddUniqueDynamic은 중복하여 함수를 호출하지 X (버튼에 사용하기 적합)
        GirlButton->SetIsEnabled(true);
        //StartButton->SetVisibility(ESlateVisibility::Hidden);

        //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "GirlButton Init End");
    }

    if (EmployeeButton)
    {
        //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "EmployeeButton Init");

        EmployeeButton->SetClickMethod(EButtonClickMethod::DownAndUp);
        EmployeeButton->OnClicked.AddUniqueDynamic(this, &UChoiceCharacterUI::OnClickedEmployeeButton);
        EmployeeButton->SetIsEnabled(true);

        //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "EmployeeButton Init End");
    }

    if (IdolButton)
    {
        //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "IdolButton Init");
        IdolButton->SetClickMethod(EButtonClickMethod::DownAndUp);
        IdolButton->OnClicked.AddUniqueDynamic(this, &UChoiceCharacterUI::OnClickedIdolButton);
        IdolButton->SetIsEnabled(true);
        //StartButton->SetVisibility(ESlateVisibility::Hidden);

        //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "IdolButton Init End");
    }

    if (FireFighterButton)
    {
        //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "FireFighterButton Init");

        FireFighterButton->SetClickMethod(EButtonClickMethod::DownAndUp);
        FireFighterButton->OnClicked.AddUniqueDynamic(this, &UChoiceCharacterUI::OnClickedFireFighterButton);
        FireFighterButton->SetIsEnabled(true);

        //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "FireFighterButton Init End");
    }

    if (ReadyButton)
    {
        //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "ReadyButton Init");

        ReadyButton->SetClickMethod(EButtonClickMethod::DownAndUp);
        ReadyButton->OnClicked.AddUniqueDynamic(this, &UChoiceCharacterUI::OnClickedReadyButton);
        ReadyButton->SetIsEnabled(true);

        //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "ReadyButton Init End");
    }

    const TMap<uint32, TMap<uint32, FString>>& LobbyPlayers = LobbyPlayer->GetLobbyPlayers();
    uint32 CurrentRoomId = GameInstance->ClientSocketPtr->MyRoomId;
    const TMap<uint32, FString>* CurrentRoomPlayers = LobbyPlayers.Find(CurrentRoomId);

    if (CurrentRoomPlayers)
    {
        if (First_Player && CurrentRoomPlayers->Contains(1))
        {
            First_Player->SetText(FText::FromString((*CurrentRoomPlayers)[1]));
        }
        if (Second_Player && CurrentRoomPlayers->Contains(2))
        {
            Second_Player->SetText(FText::FromString((*CurrentRoomPlayers)[2]));
        }
        if (Third_Player && CurrentRoomPlayers->Contains(3))
        {
            Third_Player->SetText(FText::FromString((*CurrentRoomPlayers)[3]));
        }
        if (Fourth_Player && CurrentRoomPlayers->Contains(4))
        {
            Fourth_Player->SetText(FText::FromString((*CurrentRoomPlayers)[4]));
        }
    }

}


