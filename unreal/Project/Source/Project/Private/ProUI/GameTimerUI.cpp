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
	if (!RoofTopClose && 10 - m_iMinites > 5) {
		RoofTopClose = true;
		if (Character) {
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("!RoofTopClose && m_iMinites >= 5")));
			FText KText = FText::FromString(TEXT("헬기가 떠났습니다."));
			FText KMissionText1 = FText::FromString(TEXT("실패"));
			FText KMissionText2 = FText::FromString(TEXT(""));
			Character->ShowActionText(KText, FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f)), 5.f);

			USoundBase* Sound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Sound/Leavehelicopter.Leavehelicopter")); // 에셋 경로
			Character->PlaySoundForPlayer(Sound);

			// 5분 지날 시 옥상문 잠기는 Text
			Character->ShowMissionText(KMissionText1, FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f)), 1);
			Character->ShowMissionText(KMissionText2, FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f)), 2);
			Character->ShowMissionText(KMissionText2, FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f)), 3);
		}

	}

}

void UGameTimerUI::SetTimerStart()
{
	m_fStartTimer = GetWorld()->GetTimeSeconds();
}


