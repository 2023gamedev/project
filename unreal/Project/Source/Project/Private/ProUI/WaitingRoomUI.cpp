// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/WaitingRoomUI.h"
#include "Kismet/GameplayStatics.h"
#include "LStruct.pb.h"

void UWaitingRoomUI::OnReadyButtonClicked()
{
    if (!bIsReady) {
        Protocol::CS_Ready ReadyPacket;
        ReadyPacket.set_type(5);
        ReadyPacket.set_playerid(GameInstance->ClientSocketPtr->GetMyPlayerId());
        ReadyPacket.set_ready(true);

        std::string SerializedData;
        ReadyPacket.SerializeToString(&SerializedData);

        GameInstance->ClientSocketPtr->Send(SerializedData.size(), (void*)SerializedData.data());

        Ready_TextBlock->SetText(FText::FromString("Cancel"));
        bIsReady = true;
    }
    else {
        Protocol::CS_Ready ReadyPacket;
        ReadyPacket.set_type(5);
        ReadyPacket.set_playerid(GameInstance->ClientSocketPtr->GetMyPlayerId());
        ReadyPacket.set_ready(false);

        std::string SerializedData;
        ReadyPacket.SerializeToString(&SerializedData);

        GameInstance->ClientSocketPtr->Send(SerializedData.size(), (void*)SerializedData.data());

        Ready_TextBlock->SetText(FText::FromString("Ready"));
        bIsReady = false;
    }

}

void UWaitingRoomUI::AllReady()
{
    MoveChoiceCharacterUI.Execute();
    RemoveFromParent();
}

void UWaitingRoomUI::OnBackButtonClicked()
{
    Protocol::CS_Leave LeavePacket;
    LeavePacket.set_playerid(GameInstance->ClientSocketPtr->GetMyPlayerId());
    LeavePacket.set_type(11);

    std::string serializedData;
    LeavePacket.SerializeToString(&serializedData);
    bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());

    MoveStartGameUI.Execute();
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

void UWaitingRoomUI::RemovePlayerFromList(const FString& PlayerName)
{
    if (!PlayerList) return;

    // PlayerList의 모든 자식을 순회하며 해당 이름의 항목을 찾음
    for (int32 i = 0; i < PlayerList->GetChildrenCount(); ++i) {
        UWidget* Child = PlayerList->GetChildAt(i);
        if (UTextBlock* PlayerText = Cast<UTextBlock>(Child)) {
            if (PlayerText->GetText().ToString() == PlayerName) {
                // 해당 이름의 항목을 제거
                PlayerList->RemoveChild(PlayerText);
                return; // 제거 후 함수 종료
            }
        }
    }
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