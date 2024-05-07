// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/ProGameClearUI.h"
#include "Components/TextBlock.h"

UProGameClearUI::UProGameClearUI(const FObjectInitializer& ObjectInitializer)
    :Super(ObjectInitializer)
{
    Message = Cast<UTextBlock>(GetWidgetFromName(TEXT("Message")));
    SText = Cast<UTextBlock>(GetWidgetFromName(TEXT("SText")));
}

void UProGameClearUI::NativeConstruct()
{
    Super::NativeConstruct();


}


void UProGameClearUI::SetMessage(const FString& mes)
{
    if (Message)
    {
        Message->SetText(FText::FromString(mes));
    }
}

void UProGameClearUI::SetScore(int32 Score)
{
    if (SText){
        SText->SetText(FText::AsNumber(Score));
    }
}

