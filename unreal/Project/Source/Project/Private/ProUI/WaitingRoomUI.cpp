// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/WaitingRoomUI.h"
#include "Kismet/GameplayStatics.h"
#include "LStruct.pb.h"

void UWaitingRoomUI::OnReadyButtonClicked()
{
}

void UWaitingRoomUI::OnBackButtonClicked()
{
}

void UWaitingRoomUI::OnSendButtonClicked()
{
    if (!ChatText || ChatText->GetText().IsEmpty())
    {
        return;
    }

    FString ChatMessage = ChatText->GetText().ToString();
    FString FormattedMessage = FString::Printf(TEXT("%s: %s"), *(GameInstance->MyUserName), *ChatMessage);

    SendChat(FormattedMessage);

    ChatText->SetText(FText::GetEmpty());
}

void UWaitingRoomUI::AddPlayerToList(const FString& PlayerName)
{
    if (!PlayerList) return;

    UTextBlock* NewPlayerText = NewObject<UTextBlock>(this);
    NewPlayerText->SetText(FText::FromString(PlayerName));
    NewPlayerText->Font.Size = 60;

    PlayerList->AddChild(NewPlayerText);
}

void UWaitingRoomUI::AddChatMessage(const FString& Message)
{
    if (!ChattingList) return;

    UTextBlock* NewChatMessage = NewObject<UTextBlock>(this);
    if (NewChatMessage)
    {
        NewChatMessage->SetText(FText::FromString(Message));
        NewChatMessage->Font.Size = 30;

        // ScrollBox에 추가
        ChattingList->AddChild(NewChatMessage);

        // 스크롤을 가장 아래로 이동
        ChattingList->ScrollToEnd();
    }
}

void UWaitingRoomUI::Init()
{
    GameInstance = Cast<UProGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

    if (ReadyButton)
    {
        ReadyButton->SetClickMethod(EButtonClickMethod::DownAndUp);
        ReadyButton->OnClicked.AddDynamic(this, &UWaitingRoomUI::OnReadyButtonClicked);

        ReadyButton->SetIsEnabled(true);

    }

    if (BackButton)
    {
        BackButton->SetClickMethod(EButtonClickMethod::DownAndUp);
        BackButton->OnClicked.AddDynamic(this, &UWaitingRoomUI::OnBackButtonClicked);
        BackButton->SetIsEnabled(true);
    }

    if (SendButton)
    {
        SendButton->SetClickMethod(EButtonClickMethod::DownAndUp);
        SendButton->OnClicked.AddDynamic(this, &UWaitingRoomUI::OnSendButtonClicked);
        SendButton->SetIsEnabled(true);
    }

    if (PlayerList)
    {
        //test
        AddPlayerToList("Player1");
        AddPlayerToList("Player2");
    }
}

void UWaitingRoomUI::SendChat(const FString& FormattedMessage)
{
    Protocol::CS_Chatting Packet;

    Packet.set_type(13);
    Packet.set_playerid(GameInstance->ClientSocketPtr->GetMyPlayerId());

    std::string stringMessage = TCHAR_TO_UTF8(*FormattedMessage);
    Packet.set_chat(stringMessage);

    std::string serializedData;
    Packet.SerializeToString(&serializedData);

    bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());
}