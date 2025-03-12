// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/GameChatUI.h"
#include "Kismet/GameplayStatics.h"
#include "LStruct.pb.h"


void UGameChatUI::Init()
{
    GameInstance = Cast<UProGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

    if (ChatText)
    {
        ChatText->OnTextCommitted.AddDynamic(this, &UGameChatUI::OnSendButtonEntered);
        ChatText->SetIsEnabled(true);
        ChatText->SetVisibility(ESlateVisibility::Hidden);
    }
}

bool UGameChatUI::IsChatVisible() const
{
    return ChatText && ChatText->GetVisibility() == ESlateVisibility::Visible;
}


void UGameChatUI::ToggleChatVisibility(bool bChatVisible)
{
    if (!ChatText) return;

    if (bChatVisible)
    {
        ChatText->SetVisibility(ESlateVisibility::Visible);
        ChatText->SetKeyboardFocus();
    }
    else
    {
        ChatText->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UGameChatUI::AddChatMessage(const FString& Message)
{
    if (!GameChatBox) return;

    UTextBlock* NewChatMessage = NewObject<UTextBlock>(this);
    if (NewChatMessage)
    {
        NewChatMessage->SetText(FText::FromString(Message));
        NewChatMessage->Font.Size = 30;

        // ScrollBox에 추가
        GameChatBox->AddChild(NewChatMessage);

        // 스크롤을 가장 아래로 이동
        GameChatBox->ScrollToEnd();
    }
}

void UGameChatUI::SendChat(const FString& FormattedMessage)
{
    Protocol::chatting Packet;

    Packet.set_packet_type(26);
    Packet.set_playerid(GameInstance->ClientSocketPtr->GetMyPlayerId());

    std::string stringMessage = TCHAR_TO_UTF8(*FormattedMessage);
    Packet.set_chat(stringMessage);

    std::string serializedData;
    Packet.SerializeToString(&serializedData);

    bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());
}

void UGameChatUI::OnSendButtonEntered(const FText& Text, ETextCommit::Type CommitMethod)
{
    UE_LOG(LogTemp, Log, TEXT("OnSendButtonEntered() called!"));

    if (CommitMethod == ETextCommit::OnEnter)
    {
        if (!ChatText || ChatText->GetText().IsEmpty() || Text.IsEmpty())
        {
            // 입력이 없으면 채팅창을 숨김
            ToggleChatVisibility(false);
            return;
        }

        FString ChatMessage = ChatText->GetText().ToString();
        FString FormattedMessage = FString::Printf(TEXT("%s: %s"), *(GameInstance->MyUserName), *ChatMessage);

        SendChat(FormattedMessage);

        ChatText->SetText(FText::GetEmpty());
    }
}