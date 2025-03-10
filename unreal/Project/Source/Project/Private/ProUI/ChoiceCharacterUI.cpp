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

    ReadyButton->SetIsEnabled(true);

    // 설명 텍스트 설정
    CharDescription_DisAdvantage->SetText(FText::FromString(TEXT("체력 낮음, 근력 매우낮음")));
    CharDescription_Advantage->SetText(FText::FromString(TEXT("스피드 아주 빠름, 스테미나 많음")));
    // 텍스트 스타일 설정
    FSlateFontInfo FontInfo;    FTextBlockStyle TextStyle;
    FontInfo.FontObject = LoadObject<UObject>(nullptr, TEXT("/Game/profont/MYYeongnamnu_Font.MYYeongnamnu_Font")); // 폰트 경로
    FontInfo.Size = 50; // 폰트 크기
    TextStyle = CharDescription_DisAdvantage->WidgetStyle.TextStyle;
    TextStyle.SetFont(FontInfo);
    CharDescription_DisAdvantage->SetTextStyle(TextStyle);
    TextStyle = CharDescription_Advantage->WidgetStyle.TextStyle;
    TextStyle.SetFont(FontInfo);
    CharDescription_Advantage->SetTextStyle(TextStyle);

    ChoicedGirl.Execute();
}

void UChoiceCharacterUI::OnClickedEmployeeButton()
{
    //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "OnClickedEmployeeButton");

    GirlBGButton->SetVisibility(ESlateVisibility::Hidden); // 숨김
    IdolBGButton->SetVisibility(ESlateVisibility::Hidden); // 숨김
    FireFighterBGButton->SetVisibility(ESlateVisibility::Hidden); // 숨김
    EmployeeBGButton->SetVisibility(ESlateVisibility::HitTestInvisible); // 보이지만 클릭 불가

    ReadyButton->SetIsEnabled(true);
    
    // 설명 텍스트 설정
    CharDescription_DisAdvantage->SetText(FText::FromString(TEXT("스피드 느림")));
    CharDescription_Advantage->SetText(FText::FromString(TEXT("빠른 문열기,\n 시작아이템 서류가방 제공 (인벤토리+5)")));
    // 텍스트 스타일 설정
    FSlateFontInfo FontInfo;    FTextBlockStyle TextStyle;
    FontInfo.FontObject = LoadObject<UObject>(nullptr, TEXT("/Game/profont/MYYeongnamnu_Font.MYYeongnamnu_Font")); // 폰트 경로
    FontInfo.Size = 50; // 폰트 크기
    TextStyle = CharDescription_DisAdvantage->WidgetStyle.TextStyle;
    TextStyle.SetFont(FontInfo);
    CharDescription_DisAdvantage->SetTextStyle(TextStyle);
    FontInfo.Size = 40; // 폰트 크기
    TextStyle = CharDescription_Advantage->WidgetStyle.TextStyle;
    TextStyle.SetFont(FontInfo);
    CharDescription_Advantage->SetTextStyle(TextStyle);

    ChoicedEmployee.Execute();
}

void UChoiceCharacterUI::OnClickedIdolButton()
{
    //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "OnClickedIdolButton");

    GirlBGButton->SetVisibility(ESlateVisibility::Hidden); // 숨김
    IdolBGButton->SetVisibility(ESlateVisibility::HitTestInvisible); // 보이지만 클릭 불가
    FireFighterBGButton->SetVisibility(ESlateVisibility::Hidden); // 숨김
    EmployeeBGButton->SetVisibility(ESlateVisibility::Hidden); // 숨김

    ReadyButton->SetIsEnabled(true);
    
    // 설명 텍스트 설정
    CharDescription_DisAdvantage->SetText(FText::FromString(TEXT("출혈확률 높음")));
    CharDescription_Advantage->SetText(FText::FromString(TEXT("출혈 회복 아이템\n 높은확률로 다시 사용가능")));
    // 텍스트 스타일 설정
    FSlateFontInfo FontInfo;    FTextBlockStyle TextStyle;
    FontInfo.FontObject = LoadObject<UObject>(nullptr, TEXT("/Game/profont/MYYeongnamnu_Font.MYYeongnamnu_Font")); // 폰트 경로
    FontInfo.Size = 50; // 폰트 크기
    TextStyle = CharDescription_DisAdvantage->WidgetStyle.TextStyle;
    TextStyle.SetFont(FontInfo);
    CharDescription_DisAdvantage->SetTextStyle(TextStyle);
    FontInfo.Size = 40; // 폰트 크기
    TextStyle = CharDescription_Advantage->WidgetStyle.TextStyle;
    TextStyle.SetFont(FontInfo);
    CharDescription_Advantage->SetTextStyle(TextStyle);

    ChoicedIdol.Execute();
}

void UChoiceCharacterUI::OnClickedFireFighterButton()
{
    //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "OnClickedFireFighterButton");

    GirlBGButton->SetVisibility(ESlateVisibility::Hidden); // 숨김
    IdolBGButton->SetVisibility(ESlateVisibility::Hidden); // 숨김
    FireFighterBGButton->SetVisibility(ESlateVisibility::HitTestInvisible); // 보이지만 클릭 불가
    EmployeeBGButton->SetVisibility(ESlateVisibility::Hidden); // 숨김

    ReadyButton->SetIsEnabled(true);

    // 설명 텍스트 설정
    CharDescription_DisAdvantage->SetText(FText::FromString(TEXT("담배를 주기적으로 피워줘야 함")));
    CharDescription_Advantage->SetText(FText::FromString(TEXT("체력 높음, 근력 매우 높음")));
    // 텍스트 스타일 설정
    FSlateFontInfo FontInfo;    FTextBlockStyle TextStyle;
    FontInfo.FontObject = LoadObject<UObject>(nullptr, TEXT("/Game/profont/MYYeongnamnu_Font.MYYeongnamnu_Font")); // 폰트 경로
    FontInfo.Size = 50; // 폰트 크기
    TextStyle = CharDescription_DisAdvantage->WidgetStyle.TextStyle;
    TextStyle.SetFont(FontInfo);
    CharDescription_DisAdvantage->SetTextStyle(TextStyle);
    TextStyle = CharDescription_Advantage->WidgetStyle.TextStyle;
    TextStyle.SetFont(FontInfo);
    CharDescription_Advantage->SetTextStyle(TextStyle);

    ChoicedFireFighter.Execute();
}

void UChoiceCharacterUI::OnClickedReadyButton()
{
    //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "OnClickedReadyButton");

    b_ready = !b_ready;

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

    if (b_ready == true) {
        // Cancel 상태일 때 버튼의 색상 설정 (노랑)
        FButtonStyle ButtonStyle = ReadyButton->WidgetStyle;
        ButtonStyle.Normal.TintColor = FSlateColor(FLinearColor(0.8f, 0.8f, 0.f, 0.7f));
        ButtonStyle.Hovered.TintColor = FSlateColor(FLinearColor(1.f, 1.f, 0.f, 0.8f));
        ButtonStyle.Pressed.TintColor = FSlateColor(FLinearColor(0.6f, 0.6f, 0.f, 0.6f));
        ReadyButton->SetStyle(ButtonStyle);

        if (GirlButton) GirlButton->SetIsEnabled(false);
        if (EmployeeButton) EmployeeButton->SetIsEnabled(false);
        if (IdolButton) IdolButton->SetIsEnabled(false);
        if (FireFighterButton) FireFighterButton->SetIsEnabled(false);

        Ready_TextBlock->SetText(FText::FromString("Cancel"));
    }
    else {
        // 다시 Ready 상태일 때 버튼의 색상 설정 (초록)
        FButtonStyle ButtonStyle = ReadyButton->WidgetStyle;
        ButtonStyle.Normal.TintColor = FSlateColor(FLinearColor(0.f, 0.8f, 0.f, 0.7f));
        ButtonStyle.Hovered.TintColor = FSlateColor(FLinearColor(0.f, 1.f, 0.f, 0.8f));
        ButtonStyle.Pressed.TintColor = FSlateColor(FLinearColor(0.f, 0.6f, 0.f, 0.6f));
        ReadyButton->SetStyle(ButtonStyle);

        if (GirlButton) GirlButton->SetIsEnabled(true);
        if (EmployeeButton) EmployeeButton->SetIsEnabled(true);
        if (IdolButton) IdolButton->SetIsEnabled(true);
        if (FireFighterButton) FireFighterButton->SetIsEnabled(true);

        PreventMultipleCharacter(); // 캐릭 중복 선택 방지 버튼 상태 갱신

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
    if (NewTexture) {
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

        if (recvSelect.PlayerId == 1) {
            FSlateBrush Brush;
            Brush.SetResourceObject(NewTexture);   // 텍스처를 브러시에 설정
            First_Image->SetBrush(Brush);      // UImage에 브러시 설정    
        }
        else if (recvSelect.PlayerId == 2) {
            FSlateBrush Brush;
            Brush.SetResourceObject(NewTexture);
            Second_Image->SetBrush(Brush);
        }
        else if (recvSelect.PlayerId == 3) {
            FSlateBrush Brush;
            Brush.SetResourceObject(NewTexture);
            Third_Image->SetBrush(Brush);
        }
        else if (recvSelect.PlayerId == 4) {
            FSlateBrush Brush;
            Brush.SetResourceObject(NewTexture);
            Fourth_Image->SetBrush(Brush);
        }

        // 다른 사람이 선택한 캐릭 중복선택 X 하기 위해 맵에 저장
        PlayersCharacter.Add(recvSelect.PlayerId, recvSelect.Character_type);

        if (b_ready == true)
            return; // 이미 레디 누른 상태면 아래 작업 필요 X

        PreventMultipleCharacter(); // 캐릭 중복 선택 방지 버튼 상태 갱신
    }
}

void UChoiceCharacterUI::PreventMultipleCharacter()
{
    GirlButton->SetIsEnabled(true);
    EmployeeButton->SetIsEnabled(true);
    IdolButton->SetIsEnabled(true);
    FireFighterButton->SetIsEnabled(true);

    uint32 MyPlayerId = GameInstance->ClientSocketPtr->GetMyPlayerId();
    for (uint32 playerID = 1; playerID <= (uint32)PlayersCharacter.Num(); playerID++) {
        if (MyPlayerId == playerID)
            continue;

        if (PlayersCharacter.Contains(playerID) == true) {
            uint32* playerCharType = PlayersCharacter.Find(playerID);
            if (*playerCharType == 0) continue;  // 초기화값임
            UE_LOG(LogTemp, Log, TEXT("[PreventMultipleCharacter] PlayerID: %d, CharacterType: %d"), playerID, *playerCharType);
            switch (*playerCharType) {
            case 1:
                GirlButton->SetIsEnabled(false);
                break;
            case 2:
                EmployeeButton->SetIsEnabled(false);
                break;
            case 3:
                IdolButton->SetIsEnabled(false);
                break;
            case 4:
                FireFighterButton->SetIsEnabled(false);
                break;
            }
        }
    }
}

void UChoiceCharacterUI::UpdatePlayerReadyState(uint32 player_num, bool Ready)
{
    if (Ready == true) {
        if (player_num == 1 && First_Ready)
        {
            First_Ready->SetText(FText::FromString("READY"));
            FSlateFontInfo FontInfo = First_Ready->GetFont();
            FontInfo.Size = 50;
            First_Ready->SetFont(FontInfo);
            First_Ready->SetJustification(ETextJustify::Center);
            First_Ready->SetColorAndOpacity(FSlateColor(FLinearColor::Green));
        }
        else if (player_num == 2 && Second_Ready)
        {
            Second_Ready->SetText(FText::FromString("READY"));
            FSlateFontInfo FontInfo = Second_Ready->GetFont();
            FontInfo.Size = 50;
            Second_Ready->SetFont(FontInfo);
            Second_Ready->SetJustification(ETextJustify::Center);
            Second_Ready->SetColorAndOpacity(FSlateColor(FLinearColor::Green));
        }
        else if (player_num == 3 && Third_Ready)
        {
            Third_Ready->SetText(FText::FromString("READY"));
            FSlateFontInfo FontInfo = Third_Ready->GetFont();
            FontInfo.Size = 50;
            Third_Ready->SetFont(FontInfo);
            Third_Ready->SetJustification(ETextJustify::Center);
            Third_Ready->SetColorAndOpacity(FSlateColor(FLinearColor::Green));
        }
        else if (player_num == 4 && Fourth_Ready)
        {
            Fourth_Ready->SetText(FText::FromString("READY"));
            FSlateFontInfo FontInfo = Fourth_Ready->GetFont();
            FontInfo.Size = 50;
            Fourth_Ready->SetFont(FontInfo);
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

         // 버튼의 초기 색상 설정 (초록)
        FButtonStyle ButtonStyle = ReadyButton->WidgetStyle;
        ButtonStyle.Normal.TintColor = FSlateColor(FLinearColor(0.f, 0.8f, 0.f, 0.7f));
        ButtonStyle.Hovered.TintColor = FSlateColor(FLinearColor(0.f, 1.f, 0.f, 0.8f));
        ButtonStyle.Pressed.TintColor = FSlateColor(FLinearColor(0.f, 0.6f, 0.f, 0.6f));
        ReadyButton->SetStyle(ButtonStyle);

        ReadyButton->SetClickMethod(EButtonClickMethod::DownAndUp);
        ReadyButton->OnClicked.AddUniqueDynamic(this, &UChoiceCharacterUI::OnClickedReadyButton);
        ReadyButton->SetIsEnabled(false);  // 캐릭을 선택안하고 클릭하는 걸 방지

        //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "ReadyButton Init End");
    }


    // 플레이어 이름 띄우기 & 빈 칸들 비활성화
    const TMap<uint32, TMap<uint32, FString>>& LobbyPlayers = LobbyPlayer->GetLobbyPlayers();
    uint32 CurrentRoomId = GameInstance->ClientSocketPtr->MyRoomId;
    const TMap<uint32, FString>* CurrentRoomPlayers = LobbyPlayers.Find(CurrentRoomId);

    if (CurrentRoomPlayers)
    {
        // 키 배열을 추출
        TArray<uint32> SortedKeys;
        CurrentRoomPlayers->GetKeys(SortedKeys);

        // 키 배열 정렬
        SortedKeys.Sort();

        // 정렬된 키-값 쌍을 TArray로 저장
        TArray<TPair<uint32, FString>> SortedArray;
        for (uint32 Key : SortedKeys)
        {
            if (const FString* Value = CurrentRoomPlayers->Find(Key))
            {
                SortedArray.Add(TPair<uint32, FString>(Key, *Value));
            }
        }

        int player_num = 0;
        // 이름 정렬된 순서에 맞게 출력
        for (auto player : SortedArray) {
            ++player_num;
            PlayersCharacter.Add(player_num, 0);        // 초기화 (맵 크기 설정 때문에)
            if (player_num == 1 && First_Player) {
                First_Player->SetText(FText::FromString(player.Value));
                First_Image->SetIsEnabled(true);
            }
            else if (player_num < 1) {
                First_Player->SetText(FText::FromString(""));
                First_Image->SetIsEnabled(false);
            }
            if (player_num == 2 && Second_Player) {
                Second_Player->SetText(FText::FromString(player.Value));
                Second_Image->SetIsEnabled(true);
            }
            else if (player_num < 2) {
                Second_Player->SetText(FText::FromString(""));
                Second_Image->SetIsEnabled(false);
            }
            if (player_num == 3 && Third_Player) {
                Third_Player->SetText(FText::FromString(player.Value));
                Third_Image->SetIsEnabled(true);
            }
            else if (player_num < 3) {
                Third_Player->SetText(FText::FromString(""));
                Third_Image->SetIsEnabled(false);
            }
            if (player_num == 4 && Fourth_Player) {
                Fourth_Player->SetText(FText::FromString(player.Value));
                Fourth_Image->SetIsEnabled(true);
            }
            else if (player_num < 4) {
                Fourth_Player->SetText(FText::FromString(""));
                Fourth_Image->SetIsEnabled(false);
            }
        }
    }
}
