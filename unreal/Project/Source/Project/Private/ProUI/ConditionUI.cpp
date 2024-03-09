// Fill out your copyright notice in the Description page of Project Settings.


#include "ProUI/ConditionUI.h"

UConditionUI::UConditionUI(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	HPBar = CreateDefaultSubobject<UHPBar>(TEXT("HPBar"));
	StaminaBar = CreateDefaultSubobject<UStaminaBar>(TEXT("StaminaBar"));
}