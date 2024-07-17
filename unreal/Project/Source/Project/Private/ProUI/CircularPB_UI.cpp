// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/CircularPB_UI.h"
#include "Animation/WidgetAnimation.h"
#include "Runtime/Engine/Classes/Animation/AnimNodeBase.h"
#include "UMG.h"

void UCircularPB_UI::StartVisibleAnimation(float PlaySpeed)
{
	PlayAnimation(CircularPB_ANIM, 0.f, 1, EUMGSequencePlayMode::Forward, PlaySpeed);
}