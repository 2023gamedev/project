// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/Select_RoomUI.h"
#include "Kismet/GameplayStatics.h"
#include "LStruct.pb.h"

void USelect_RoomUI::OnRoom1ButtonClicked()
{
    SendJoin(1);
    MoveWaitingRoomUI.Execute();
    RemoveFromParent();
}

void USelect_RoomUI::OnRoom2ButtonClicked()
{
    SendJoin(2);
    MoveWaitingRoomUI.Execute();
    RemoveFromParent();
}

void USelect_RoomUI::OnRoom3ButtonClicked()
{
    SendJoin(3);
    MoveWaitingRoomUI.Execute();
    RemoveFromParent();
}

void USelect_RoomUI::OnRoom4ButtonClicked()
{
    SendJoin(4);
    MoveWaitingRoomUI.Execute();
    RemoveFromParent();
}

void USelect_RoomUI::Init()
{
    GameInstance = Cast<UProGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

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

void USelect_RoomUI::SendJoin(uint32 roomid) 
{
    Protocol::CS_Join packet;

    packet.set_type(8);
    packet.set_playerid(GameInstance->ClientSocketPtr->GetMyPlayerId());
    packet.set_roomid(roomid);

    std::string serializedData;
    packet.SerializeToString(&serializedData);

    // 직렬화된 데이터를 서버로 전송
    bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());
}