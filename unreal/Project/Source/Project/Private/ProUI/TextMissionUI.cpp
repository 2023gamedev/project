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
