// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/WaitingRoomUI.h"

#include "Kismet/GameplayStatics.h"
#include "LStruct.pb.h"
#include "Widgets/Input/SEditableTextBox.h"

void UWaitingRoomUI::NativeConstruct()
{
    Super::NativeConstruct();

    if (!ChatText_IE.IsValid())
    {
        // SEditableTextBox_IgnoreEnter 초기화
        //ChatText_IE = SNew(SEditableTextBox_IgnoreEnter);



        // 방법1
        // Slate 위젯을 Overlay에 추가
        //TSharedRef<SWidget> SlateWidgetRef = ChatText_IE.ToSharedRef();

        //ChatOverlay->AddChild(Cast<UWidget>(SlateWidgetRef));



        // 방법2
        // 기존 컨테이너에서 사용할 수 있도록 추가하는 래퍼 생성
        //UWidget* WrappedWidget = WidgetTree->ConstructWidget<UWidget>(UUserWidget::StaticClass());
        //WidgetTree->RootWidget->AddChild(WrappedWidget);
    }
}

void UWaitingRoomUI::Init()
{
    GameInstance = Cast<UProGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

    if (ReadyButton)
    {
        // 버튼의 초기 색상 설정 (초록)
        FButtonStyle ButtonStyle = ReadyButton->WidgetStyle;
        ButtonStyle.Normal.TintColor = FSlateColor(FLinearColor(0.f, 0.8f, 0.f, 0.7f));
        ButtonStyle.Hovered.TintColor = FSlateColor(FLinearColor(0.f, 1.f, 0.f, 0.8f));
        ButtonStyle.Pressed.TintColor = FSlateColor(FLinearColor(0.f, 0.6f, 0.f, 0.6f));
        ReadyButton->SetStyle(ButtonStyle);

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

    //if (!ChatText_IE.IsValid())
    //{
    //    ChatText_IE = SNew(SEditableTextBox_IgnoreEnter);
    //
    //    // Slate 위젯을 UWidget으로 감싸서 ChatTextWidget에 할당
    //    //ChatTextWidget->SetWidget(SNew(SEditableTextBox_IgnoreEnter));
    //}

    if (ChatText) 
    {
        ChatText->OnTextCommitted.AddDynamic(this, &UWaitingRoomUI::OnSendButtonEntered);
        ChatText->SetIsEnabled(true);
    }

    PlayerSlots.Add(PlayerSlot1);
    PlayerSlots.Add(PlayerSlot2);
    PlayerSlots.Add(PlayerSlot3);
    PlayerSlots.Add(PlayerSlot4);

    SlotOccupied.Init(false, PlayerSlots.Num());
}

void UWaitingRoomUI::OnReadyButtonClicked()
{
    if (!bIsReady) {
        // Cancel 상태일 때 버튼의 색상 설정 (노랑)
        FButtonStyle ButtonStyle = ReadyButton->WidgetStyle;
        ButtonStyle.Normal.TintColor = FSlateColor(FLinearColor(0.8f, 0.8f, 0.f, 0.7f));
        ButtonStyle.Hovered.TintColor = FSlateColor(FLinearColor(1.f, 1.f, 0.f, 0.8f));
        ButtonStyle.Pressed.TintColor = FSlateColor(FLinearColor(0.6f, 0.6f, 0.f, 0.6f));
        ReadyButton->SetStyle(ButtonStyle);


        Protocol::WaitingReady ReadyPacket;
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
        // 다시 Ready 상태일 때 버튼의 색상 설정 (초록)
        FButtonStyle ButtonStyle = ReadyButton->WidgetStyle;
        ButtonStyle.Normal.TintColor = FSlateColor(FLinearColor(0.f, 0.8f, 0.f, 0.7f));
        ButtonStyle.Hovered.TintColor = FSlateColor(FLinearColor(0.f, 1.f, 0.f, 0.8f));
        ButtonStyle.Pressed.TintColor = FSlateColor(FLinearColor(0.f, 0.6f, 0.f, 0.6f));
        ReadyButton->SetStyle(ButtonStyle);


        Protocol::WaitingReady ReadyPacket;
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

void UWaitingRoomUI::PlayerReady(uint32 playerid, bool ready)
{

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

    // 입력창에 다시 포커스를 설정
    FSlateApplication::Get().SetKeyboardFocus(ChatText->TakeWidget());
}

void UWaitingRoomUI::OnSendButtonEntered(const FText& Text, ETextCommit::Type CommitMethod)
{
    if (CommitMethod == ETextCommit::OnEnter)
    {
        // 포커스를 계속 유지하기 위한 코드
        ChatText->SetFocus();   // 작동안됨 -> EditableTextBox 자체가 엔터를 입력받으면 (OnTextCommitted 실행되면) 포커스를 자동으로 잃는 특성이 있음

        if (!ChatText || ChatText->GetText().IsEmpty() || Text.IsEmpty())
        {
            return;
        }

        FString ChatMessage = ChatText->GetText().ToString();
        FString FormattedMessage = FString::Printf(TEXT("%s: %s"), *(GameInstance->MyUserName), *ChatMessage);

        SendChat(FormattedMessage);

        ChatText->SetText(FText::GetEmpty());
    }
}

void UWaitingRoomUI::AddPlayerToList(const uint32 roomid, const FString& PlayerName)
{
    int32 SlotIndex = FindEmptySlot();
    if (SlotIndex == -1 || SlotIndex >= PlayerSlots.Num())
    {
        UE_LOG(LogTemp, Warning, TEXT("No available slots for new player!"));
        return;
    }

    UHorizontalBox* PlayerSlot = PlayerSlots[SlotIndex];
    if (!PlayerSlot) return;

    UTextBlock* PlayerNameText = NewObject<UTextBlock>(this);
    PlayerNameText->SetText(FText::FromString(PlayerName));
    PlayerNameText->Font.Size = 60;

    UTextBlock* ReadyStateText = NewObject<UTextBlock>(this);
    ReadyStateText->SetText(FText::FromString(""));
    ReadyStateText->Font.Size = 60;
    ReadyStateText->SetColorAndOpacity(FSlateColor(FLinearColor::Transparent));


    PlayerSlot->ClearChildren();

    UHorizontalBoxSlot* NameSlot = PlayerSlot->AddChildToHorizontalBox(PlayerNameText);
    NameSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill)); // 비율로 공간 분배
    NameSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Left);

    // READY 상태 텍스트 추가 (슬롯 비율 1)
    UHorizontalBoxSlot* ReadySlot = PlayerSlot->AddChildToHorizontalBox(ReadyStateText);
    ReadySlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic)); // READY는 필요한 공간만 차지
    ReadySlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Right);

    SlotOccupied[SlotIndex] = true;
    UE_LOG(LogTemp, Log, TEXT("Added player %s to slot %d"), *PlayerName, SlotIndex);
}


void UWaitingRoomUI::RemovePlayerFromList(const FString& PlayerName)
{
    if (!PlayerList) return;

    for (int i = 0; i < PlayerList->GetChildrenCount(); ++i)
    {
        UWidget* Child = PlayerList->GetChildAt(i);
        if (UHorizontalBox* PlayerRow = Cast<UHorizontalBox>(Child))
        {
            if (UTextBlock* PlayerText = Cast<UTextBlock>(PlayerRow->GetChildAt(0)))
            {
                if (PlayerText->GetText().ToString() == PlayerName)
                {
                    SlotOccupied[i] = false;

                    PlayerList->RemoveChild(PlayerRow);
                    return;
                }
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

int32 UWaitingRoomUI::FindEmptySlot()
{
    for (int32 i = 0; i < SlotOccupied.Num(); ++i)
    {
        if (!SlotOccupied[i]) // 빈 슬롯 찾기
        {
            return i;
        }
    }
    return -1; // 빈 슬롯 없음
}

void UWaitingRoomUI::UpdatePlayerReadyState(uint32 playerid, bool Ready)
{
    FString PlayerName = Waiting_LobbyPlayers[playerid];

    for (int32 i = 0; i < PlayerSlots.Num(); ++i)
    {
        UHorizontalBox* PlayerSlot = PlayerSlots[i];
        if (PlayerSlot && SlotOccupied[i])
        {
            // TextBlock에서 PlayerName 찾기
            UTextBlock* PlayerNameText = Cast<UTextBlock>(PlayerSlot->GetChildAt(0));
            if (PlayerNameText && PlayerNameText->GetText().ToString() == PlayerName)
            {
                // READY 상태 TextBlock 업데이트
                UTextBlock* ReadyStateText = Cast<UTextBlock>(PlayerSlot->GetChildAt(1));
                if (ReadyStateText)
                {
                    ReadyStateText->SetText(Ready ? FText::FromString("READY") : FText::FromString(""));
                    ReadyStateText->SetColorAndOpacity(Ready ? FSlateColor(FLinearColor::Green) : FSlateColor(FLinearColor::Transparent));
                }
                return;
            }
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Player %s not found for READY state update"), *PlayerName);
}