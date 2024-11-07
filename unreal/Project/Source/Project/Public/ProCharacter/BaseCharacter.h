// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProGamemode/ProGameInstance.h"

#include <atomic>

// 구조체
#include "ProData/ItemDataStructure.h"

#include "BaseCharacter.generated.h"

class UGamePlayerUI;
class UConditionUI;
class UProGameClearUI;
class UPlayerSight;
class ANormalWeaponActor;
class AThrowWeaponActor;
class AHealingItemActor;
class ABleedingHealingItemActor;
class AKeyActor;
class UGameTimerUI;
class UPickUpUI;
class UCircularPB_UI;
class AHealingNiagaEffect;
class UTextMissionUI;
class UOtherPlayerUI;


DECLARE_DELEGATE_FourParams(FThrowOnGround, FName, EItemClass, UTexture2D*, int);
DECLARE_MULTICAST_DELEGATE(FAttackEndPlayerDelegate);
DECLARE_MULTICAST_DELEGATE(FPickUpEndPlayerDelegate);
DECLARE_MULTICAST_DELEGATE(FBleedHealingEndPlayerDelegate);

// 플레이어 캐릭터들의 부모클래스
UCLASS()
class PROJECT_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	bool DraggingSwap(int from, ESlotType fromtype, int to, ESlotType totype);
	bool SwapInven(int from, int to);

	// 바닥에 떨어트리는 델리게이트 
	FThrowOnGround ThrowOnGround;
	void SpawnOnGround(int slotindex);

	//void AttackCheck();

	// 스프링 암
	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;

	// 카메라
	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	// 플레이어가 물체를 바라보는지
	UPROPERTY(EditAnywhere)
	UPlayerSight* PlayerSight;

	// 퀵슬롯에 있는 아이템들 넣을 예정
	UPROPERTY(EditAnywhere)
	ANormalWeaponActor* CurrentWeapon;

	UPROPERTY(EditAnywhere)
	AThrowWeaponActor* CurrentThrowWeapon;

	UPROPERTY(EditAnywhere)
	AHealingItemActor* CurrentHealingItem;

	UPROPERTY(EditAnywhere)
	ABleedingHealingItemActor* CurrentBleedingHealingItem;

	UPROPERTY(EditAnywhere)
	AKeyActor* CurrentKeyItem;

	UPROPERTY(EditAnywhere)
	AHealingNiagaEffect* HealingFX;

	UPROPERTY(EditAnywhere)
	USpotLightComponent* FlashLight;

	// 게임 전체 UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TArray<FItemDataStructure> Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TArray<FItemDataStructure> QuickSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UGamePlayerUI> GameUIClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UGamePlayerUI* GameUIWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UConditionUI> ConditionUIClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UConditionUI* ConditionUIWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UProGameClearUI> ProGameClearUIClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UProGameClearUI* ProGameClearUIWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UGameTimerUI> GameTimerUIClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UGameTimerUI* GameTimerUIWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TArray<FItemDataStructure> PickUpSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UPickUpUI> PickUpUIClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UPickUpUI* PickUpUIWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UTextMissionUI> TextMissionUIClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UTextMissionUI* TextMissionUIWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UOtherPlayerUI> OtherPlayerUIClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UOtherPlayerUI* OtherPlayerUIWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UOtherPlayerUI> OtherPlayer2UIClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UOtherPlayerUI* OtherPlayer2UIWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UOtherPlayerUI> OtherPlayer3UIClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UOtherPlayerUI* OtherPlayer3UIWidget;

	// 텍스트를 화면에 표시하는 함수
	void ShowActionText(FText Text, const FSlateColor& Color, float DisplayTime = 5); // fadeout이 5초

	void ShowMissionText(FText Text, const FSlateColor& Color, int TextNumber);


	void OnPickUPUISlot();

	FTimerHandle PickUpUIHandle;
	void ProStartPickUpUI();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UCircularPB_UI> CircularPB_Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UCircularPB_UI* CircularPB_Widget;



	UFUNCTION()
	void AttackMontageEnded(UAnimMontage* Montage, bool interrup);

	UFUNCTION()
	void HealingMontageEnded(UAnimMontage* Montage, bool interrup);

	UFUNCTION()
	void BleedHealingMontageEnded(UAnimMontage* Montage, bool interrup);

	// input
	void MoveForward(FVector RotateYaw, float NewAxisValue);
	void MoveLeft(FVector RotateYaw, float NewAxisValue);
	void Run();
	void Other_Run(bool mbrun);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);
	void GetItem();
	void OtherGetItem();
	void LightOnOff();
	void InventoryOnOff();

	FAttackEndPlayerDelegate m_DAttackEnd;
	void Attack();

	void Healing();
	void BleedHealing();
	void PlayKey();
	void Throw();

	void UpdateHealingSlot();
	void UpdateBHealingSlot();
	void UpdateKeySlot();
	void UpdateThrowWSlot();

	FPickUpEndPlayerDelegate m_DPickUpEnd;
	void PickUp();


	void QuickNWeapon();
	void QuickBHItem();
	void QuickHItem();
	void QuickTWeapon();
	void QuickKeyItem();

	// 행동 확인 Get 함수 08/18
	bool IsAttack() { return m_bIsAttacking; }
	bool IsHealing() { return m_bIsHealingTime; }
	bool IsBHealing() { return m_bIsBleedHealing; }
	// bool IsKey() { return m_bIsk; }
	// bool IsTH() { return m_bIsk; }

	float GetHP() { return m_fHP; }
	void SetHP(float hp) { m_fHP = hp; }

	float GetMaxHP() { return m_fMaxHP; }
	void SetMaxHP(float maxhp) { m_fMaxHP = maxhp; }

	float GetSTR() { return m_fSTR; }
	void SetSTR(float str) { m_fSTR = str; }

	float GetSpeed() { return m_fSpeed; }
	void SetSpeed(float speed) { m_fSpeed = speed; }

	float GetBasicSpeed() { return m_fBasicSpeed; }
	void SetBasicSpeed(float speed) { m_fBasicSpeed = speed; }


	float GetStamina() { return m_fStamina; }
	void SetStamina(float stamina) { m_fStamina = stamina; }

	float GetHealing() { return m_fHealing; }
	void SetHealing(float Healing) { m_fHealing = Healing; }

	bool IsBleeding() { return m_bBleeding; }
	void SetBleeding(bool bleeding) { m_bBleeding = bleeding; }

	bool IsSitDown() { return m_bSitDown; }
	void SetSitDown(bool sitdown) { m_bSitDown = sitdown; }

	bool IsRun() { return m_bRun; }
	void SetRun(bool run) { m_bRun = run; }


	bool IsNWHandIn() { return m_bNWHandIn; }
	void SetNWHandIn(bool handin) { m_bNWHandIn = handin; }

	bool IsBHHandIn() { return m_bBHHandIn; }
	void SetBHHandIn(bool handin) { m_bBHHandIn = handin; }

	bool IsHealHandIn() { return m_bHealHandIn; }
	void SetHealHandIn(bool handin) { m_bHealHandIn = handin; }

	bool IsKeyHandIn() { return m_bKeyHandIn; }
	void SetKeyHandIn(bool handin) { m_bKeyHandIn = handin; }

	bool IsThrowWHandIn() { return m_bThrowWHandIn; }
	void SetThrowWHandIn(bool handin) { m_bThrowWHandIn = handin; }

	bool IsDead() { return m_bIsDead; }
	void SetDead(bool dead) { m_bIsDead = dead; }

	bool IsDeadPlay() { return m_bIsDeadPlay; }
	void SetDeadPlay(bool deadplay) { m_bIsDeadPlay = deadplay; }

	bool IsBringCurrentWeapon() { return m_bIsBringCurrentWeapon; }
	void SetBringCurrentWeapon(bool bringcurrentweapon) { m_bIsBringCurrentWeapon = bringcurrentweapon; }

	bool IsBringCurrentBleedingHealingItem() { return m_bIsBringCurrentBleedingHealingItem; }
	void SetBringCurrentBleedingHealingItem(bool bringcurrentbleedinghealingitem) { m_bIsBringCurrentBleedingHealingItem = bringcurrentbleedinghealingitem; }

	bool IsBringCurrentHealingItem() { return m_bIsBringCurrentHealingItem; }
	void SetBringCurrentHealingItem(bool bringcurrenthealingitem) { m_bIsBringCurrentHealingItem = bringcurrenthealingitem; }

	bool IsBringCurrentThrowWeapon() { return m_bIsBringCurrentThrowWeapon; }
	void SetBringCurrentThrowWeapon(bool bringcurrentthrowweapon) { m_bIsBringCurrentThrowWeapon = bringcurrentthrowweapon; }

	bool IsBringCurrentKeyItem() { return m_bIsBringCurrentKeyItem; }
	void SetBringCurrentKeyItem(bool bringcurrentkeyitem) { m_bIsBringCurrentKeyItem = bringcurrentkeyitem; }


	bool IsSpecialEffect() { return m_bSpecialEffect; }
	void SetSpecialEffect(bool specialeffect) { m_bSpecialEffect = specialeffect; }

	bool IsSpotLight() { return m_bOnSpotLight; }
	void SetSpotLight(bool spotlight) { m_bOnSpotLight = spotlight; }

	bool IsInventory() { return m_bInventoryOn; }
	void SetInventory(bool inven) { m_bInventoryOn = inven; }

	void SetCharacterName(FName charactername) { m_nCharacterName = charactername; };
	FName GetCharacterName() { return m_nCharacterName; }

	void SetCarryBagName(FName carrybagname) { m_nCarryBagName = carrybagname; };
	FName GetCarryBagName() { return m_nCarryBagName; }

	void SetInvenSize(int invensize) { m_iInvenSize = invensize; };
	int GetInvenSize() { return m_iInvenSize; }

	void SetStartLocation(FVector startlocation) { m_VStartLocation = startlocation; }
	FVector GetStartLocation() { return m_VStartLocation; }

	bool CanSetWeapon();
	// void SetWeapon(ANormalWeaponActor* NewWeapon); // 아마도 안쓸예정

	void GameUIUpdate();

	// 퀵슬롯 장착 시 생성
	void SpawnNormalWeapon();
	void SpawnThrowWeapon();
	void SpawnHealingItem();
	void SpawnBleedingHealingItem();
	void SpawnKeyItem();

	// 퀵슬롯 해제 시 삭제
	void DestroyNormalWeapon();
	void DestroyThrowWeapon();
	void DestroyHealingItem();
	void DestroyBleedingHealingItem();
	void DestroyKeyItem();


	// 슬롯에 아이템 내용 삭제
	void DestroyNormalWepaonItemSlot();
	void DestroyThrowWeaponItemSlot();
	void DestroyHealingItemSlot();
	void DestroyBleedingHealingItemSlot();
	void DestroyKeyItemSlot();

	void FootSound();

	// 체력 Timer
	void StartHealingTimer(float healingspeed, float healingduration);
	void HealingTimerElapsed();

	FTimerHandle HealingHandle;
	UPROPERTY(EditAnywhere)
	bool m_bIsHealingTime = false;

	UPROPERTY(EditAnywhere)
	float m_fHealingCount = 0.f;

	UPROPERTY(EditAnywhere)
	float m_fItemHealingSpeed = 0.f;

	UPROPERTY(EditAnywhere)
	float m_fItemHealingDuration = 0.f;

	void AddScore(int32 score);

	UPROPERTY(EditAnywhere)
	int32 m_iClearScore = 0;


	FTimerHandle GameEndHandle;
	void ProStartGameEnd();
	void ProGameEnd();

	// 소방관
	virtual void Smoking() {};

	// 출혈 회복
	FBleedHealingEndPlayerDelegate m_DBleedingHealingEnd;
	FTimerHandle BleedingHandle;
	void StartBleedingTimer();
	void BleedingTimerElapsed();

	bool RandomBleeding();
	bool RandomBleedHealing(float bhpercent);

	UPROPERTY(EditAnywhere)
	bool m_bIsBleedHealing = false;

	UPROPERTY(EditAnywhere)
	float m_fBleedPercent = 0.3f;

	// 스태미나 부분
	FTimerHandle UseStaminaHandle;

	void UseStamina();
	void UseStaminaTimerElapsed();
	void StaminaWaitingByZero();

	FTimerHandle HealingStaminaHandle;
	void HealingStamina();
	void HealingStaminaTimerElapsed();


	UPROPERTY(EditAnywhere)
	bool m_bZeroStamina = false;

	void PlayDead();

	FTimerHandle GameDeadEndHandle;
	void ProStartGameDeadEnd();
	void ProGameDeadEnd();

	FTimerHandle GameTimerEndHandle;
	void ProStartGameTimerEnd();
	void ProGameTimerEnd();


	virtual uint32 GetPlayerId() const;
	void SetPlayerId(uint32 NewPlayerId);
	void UpdatePlayerData(FVector Location);

	void SetAttack(bool b_attack);

	void SetPlayerJump();

	bool GetAttack();

	void OtherSpawnNormalWeapon(const FString& WeaponName);

	void OtherSpawnBleedingHealingItem(const FString& ItemName);

	void OtherSpawnHealingItem(const FString& ItemName);

	void OtherSpawnThrowWeapon(const FString& ItemName);

	void OtherSpawnKeyItem(const FString& ItemName);

	void Send_Destroy(uint32 itemboxid);

	void Send_GetKey(uint32 itemid);

	void OtherPlayerDead();

	void DestroyItem(uint32 ItemId);


private:

	UPROPERTY(EditAnywhere)
	bool m_bIsStanding = false;

	UPROPERTY(EditAnywhere)
	float m_fHP = 0.f;

	UPROPERTY(EditAnywhere)
	float m_fMaxHP = 0.f;

	UPROPERTY(EditAnywhere)
	float m_fSTR = 0.f;

	UPROPERTY(EditAnywhere)
	float m_fSpeed = 0.f;

	UPROPERTY(EditAnywhere)
	float m_fBasicSpeed = 0.f;


	UPROPERTY(EditAnywhere)
	float m_fStamina = 0.f;

	UPROPERTY(EditAnywhere)
	float m_fHealing = 0.f;

	UPROPERTY(EditAnywhere)
	bool m_bBleeding = false;

	UPROPERTY(EditAnywhere)
	bool m_bSitDown = false;

	UPROPERTY(EditAnywhere)
	bool m_bRun = false;

	UPROPERTY(EditAnywhere)
	bool m_bJump = false;

	// 인벤토리

	UPROPERTY(EditAnywhere)
	bool m_bNWHandIn = false;

	UPROPERTY(EditAnywhere)
	bool m_bBHHandIn = false;

	UPROPERTY(EditAnywhere)
	bool m_bHealHandIn = false;

	UPROPERTY(EditAnywhere)
	bool m_bKeyHandIn = false;

	UPROPERTY(EditAnywhere)
	bool m_bThrowWHandIn = false;

	UPROPERTY(EditAnywhere)
	bool m_bSpecialEffect = false;

	UPROPERTY(EditAnywhere)
	FName m_nCarryBagName;

	UPROPERTY(EditAnywhere)
	int m_iInvenSize = 5;

	// 어택
	UPROPERTY(EditAnywhere)
	bool m_bIsAttacking = false;

	UPROPERTY(EditAnywhere)
	bool m_bIsPickUping = false;

	// 손전등이 켜져있는지
	UPROPERTY(EditAnywhere)
	bool m_bOnSpotLight = true;

	UPROPERTY(EditAnywhere)
	bool m_bInventoryOn = false;

	UPROPERTY(EditAnywhere)
	FVector m_VStartLocation;

	UPROPERTY(EditAnywhere)
	FName m_nCharacterName;

	// 현재 
	UPROPERTY(EditAnywhere)
	bool m_bIsBringCurrentWeapon;

	UPROPERTY(EditAnywhere)
	bool m_bIsBringCurrentBleedingHealingItem;

	UPROPERTY(EditAnywhere)
	bool m_bIsBringCurrentHealingItem;

	UPROPERTY(EditAnywhere)
	bool m_bIsBringCurrentThrowWeapon;

	UPROPERTY(EditAnywhere)
	bool m_bIsBringCurrentKeyItem;

	UPROPERTY(EditAnywhere)
	bool m_bIsDead;

	UPROPERTY(EditAnywhere)
	bool m_bIsDeadPlay;

	UPROPERTY(EditAnywhere)
	int m_iHealingMontageFlag;

	UPROPERTY(EditAnywhere)
	int m_iBleedHealingMontageFlag;

private:
	UPROPERTY(VisibleAnywhere)
	uint32 PlayerId = 99;

	FVector NewLocation;

	FVector OldLocation = FVector(0.0f, 0.0f, 0.0f);

	float Speed;
	float PreviousSpeed;

public:
	UProGameInstance* GameInstance;

	std::atomic<bool> b_attack{ false };
	bool b_run = false;

	uint32 ItemBoxId = 9;


	enum FLOOR {
		Zero, B2, B1, F1, F2, F3
	};

	UPROPERTY(EditAnywhere)
	int floor = FLOOR::Zero;

};
