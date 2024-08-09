// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/GameTimerUI.h"
#include "Components/TextBlock.h"

UGameTimerUI::UGameTimerUI(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	TimerTxt = Cast<UTextBlock>(GetWidgetFromName(TEXT("TimerTxt")));
}

void UGameTimerUI::NativeConstruct()
{
	Super::NativeConstruct();

    SetTimerStart();

	UpdateTimer();
}

void UGameTimerUI::UpdateTimer()
{

	float CurrentTime = GetWorld()->GetTimeSeconds();


	float ElapsedTime = CurrentTime - m_fStartTimer;


	int32 RemainingTimeSeconds = 600 - FMath::RoundToInt(ElapsedTime);

	m_iMinites = RemainingTimeSeconds / 60;
	m_iSeconds = RemainingTimeSeconds % 60;

	FString TimerString = FString::Printf(TEXT("%d:%02d"), m_iMinites, m_iSeconds);
	FText TimerText = FText::FromString(TimerString);

	TimerTxt->SetText(TimerText);

}

void UGameTimerUI::SetTimerStart()
{
	m_fStartTimer = GetWorld()->GetTimeSeconds();
}


