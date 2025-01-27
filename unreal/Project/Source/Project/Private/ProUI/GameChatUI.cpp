// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/GameChatUI.h"

void UGameChatUI::AddChatMessageToLog(const FString& Message)
{
    if (GameChatBox)
    {
        UTextBlock* NewMessage = NewObject<UTextBlock>(GameChatBox);
        NewMessage->SetText(FText::FromString(Message));
        GameChatBox->AddChild(NewMessage);

        GameChatBox->ScrollToEnd();
    }
}
