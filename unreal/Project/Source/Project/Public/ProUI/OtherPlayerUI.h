// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProUI/BaseUI.h"
#include "OtherPlayerUI.generated.h"

/**
 * 
 */
class UImage;
class USlot;
class UTextBlock;
class UHPBar;

UCLASS()
class PROJECT_API UOtherPlayerUI : public UBaseUI
{
	GENERATED_BODY()
public:
	UOtherPlayerUI(const FObjectInitializer& ObjectInitializer);

	void Init() override;
	void Update() override;
	void UpdateOtherPlayerUI(float hp, uint32 charactertype, std::string playername);
	void UpdateHPBar(float hp);

	void UpdateDeadUI();


	void RefreshSlot(int slotindex);

public:
	// 텍스쳐 필요
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* OtherPlayerImg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UHPBar* HPBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* OtherPlayerID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* DefTex;


	int m_iOtherPlayerUINumber;
	int iMaxHealthFlag = 0;
	bool m_bPlayerImg = false;
	bool m_bPlayerDeadImg = false;
	bool m_bPlayerid = false;
	float m_fMaxHealth;
	int m_iOtherPlayerUIIndex;
	int m_iCharacterType;
private:
	TArray<USlot*> Slots;
};
