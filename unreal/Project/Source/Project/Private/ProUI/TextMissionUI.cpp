// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/TextMissionUI.h"

void UTextMissionUI::PlayFadeOutAnimation()
{
    // 애니메이션이 유효한지 확인 후 실행
    if (FadeOutAnimation)
    {
        PlayAnimation(FadeOutAnimation);
    }
}

void UTextMissionUI::PlayFadeOutMT1()
{
    // 애니메이션이 유효한지 확인 후 실행
    if (FadeOutMT1)
    {
        PlayAnimation(FadeOutMT1);
    }
}

void UTextMissionUI::PlayFadeOutMT2()
{
    if (FadeOutMT2)
    {
        PlayAnimation(FadeOutMT2);
    }
}

void UTextMissionUI::PlayFadeOutMTitle1()
{
    if (FadeOutMTitle1)
    {
        PlayAnimation(FadeOutMTitle1);
    }
}

void UTextMissionUI::PlayFadeOutMTitle2()
{
    if (FadeOutMTitle2)
    {
        PlayAnimation(FadeOutMTitle2);
    }
}

void UTextMissionUI::PlayFadeOutMT11()
{
    if (FadeOutMT11)
    {
        PlayAnimation(FadeOutMT11);
    }
}
