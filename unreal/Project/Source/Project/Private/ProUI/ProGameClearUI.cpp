// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/ProGameClearUI.h"
#include "Components/TextBlock.h"

UProGameClearUI::UProGameClearUI(const FObjectInitializer& ObjectInitializer)
    :Super(ObjectInitializer)
{
    OneAnswer       = Cast<UTextBlock>(GetWidgetFromName(TEXT("OneAnswer")));
    TwoAnswer       = Cast<UTextBlock>(GetWidgetFromName(TEXT("TwoAnswer")));
    ThreeAnswer     = Cast<UTextBlock>(GetWidgetFromName(TEXT("ThreeAnswer")));
    ThreeAnswer2    = Cast<UTextBlock>(GetWidgetFromName(TEXT("ThreeAnswer2")));
    FourAnswer      = Cast<UTextBlock>(GetWidgetFromName(TEXT("FourAnswer")));
    FiveAnswer      = Cast<UTextBlock>(GetWidgetFromName(TEXT("FiveAnswer")));
    FiveAnswer2     = Cast<UTextBlock>(GetWidgetFromName(TEXT("FiveAnswer2")));
}

void UProGameClearUI::NativeConstruct()
{
    Super::NativeConstruct();


}


void UProGameClearUI::SetMessage(int index, const FString& mes)
{
    if(index == 1){
        if (OneAnswer)
        {
            OneAnswer->SetText(FText::FromString(mes));
        }
    }
    else if (index == 2) {
        if (TwoAnswer)
        {
            TwoAnswer->SetText(FText::FromString(mes));
        }
    }
    else if (index == 3) {
        if (ThreeAnswer)
        {
            ThreeAnswer->SetText(FText::FromString(mes));
        }
    }
    else if (index == 32) {
        if (ThreeAnswer2)
        {
            ThreeAnswer2->SetText(FText::FromString(mes));
        }
    }
    else if (index == 4) {
        if (FourAnswer)
        {
            FourAnswer->SetText(FText::FromString(mes));
        }
    }
    else if (index == 5) {
        if (FiveAnswer)
        {
            FiveAnswer->SetText(FText::FromString(mes));
        }
    }
    else if (index == 52) {
        if (FiveAnswer2)
        {
            FiveAnswer2->SetText(FText::FromString(mes));
        }
    }
}

void UProGameClearUI::SetScore(int index, int32 Score)
{

}

