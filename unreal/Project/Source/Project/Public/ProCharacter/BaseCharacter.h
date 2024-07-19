// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <atomic>

// ����ü
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


DECLARE_DELEGATE_FourParams(FThrowOnGround, FName, EItemClass, UTexture2D*, int);
DECLARE_MULTICAST_DELEGATE(FAttackEndPlayerDelegate);
DECLARE_MULTICAST_DELEGATE(FPickUpEndPlayerDelegate);
DECLARE_MULTICAST_DELEGATE(FBleedHealingEndPlayerDelegate);

// �÷��̾� ĳ���͵��� �θ�Ŭ����
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

	// �ٴڿ� ����Ʈ���� ��������Ʈ 
	FThrowOnGround ThrowOnGround;
	void SpawnOnGround(int slotindex);

	void AttackCheck();

	// ������ ��
	UPROPERTY(EditAnywhere)
		USpringArmComponent* SpringArm;

	// ī�޶�
	UPROPERTY(EditAnywhere)
		UCameraComponent* Camera;

	// �÷��̾ ��ü�� �ٶ󺸴���
	UPROPERTY(EditAnywhere)
		UPlayerSight* PlayerSight;

	// �����Կ� �ִ� �����۵� ���� ����
	UPROPERTY(EditAnywhere)
	ANormalWeaponActor* CurrentWeapon;

	UPROPERTY(EditAnywhere)
	AThrowWeaponActor* CurrentThrowWeapon;

	UPROPERTY(EditAnywhere)
	AHealingItemActor* CurrentHealingItem;

	UPROPERTY(EditAnywhere)
	ABleedingHealingItemActor* CurrentBleedingHealingItem;

	UPROPERTY(EditAnywhere);
	AKeyActor* CurrentKeyItem;
	

	UPROPERTY(EditAnywhere)
	USpotLightComponent* FlashLight;

	// ���� ��ü UI
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
	void MoveLeft(FVector RotateYaw , float NewAxisValue);
	void Run();
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
	void SetWeapon(ANormalWeaponActor* NewWeapon);

	void GameUIUpdate();

	// ������ ���� �� ����
	void SpawnNormalWeapon();
	void SpawnThrowWeapon();
	void SpawnHealingItem();
	void SpawnBleedingHealingItem();
	void SpawnKeyItem();

	// ������ ���� �� ����
	void DestroyNormalWeapon();
	void DestroyThrowWeapon();
	void DestroyHealingItem();
	void DestroyBleedingHealingItem();
	void DestroyKeyItem();


	// ���Կ� ������ ���� ����
	void DestroyNormalWepaonItemSlot();
	void DestroyThrowWeaponItemSlot();
	void DestroyHealingItemSlot();
	void DestroyBleedingHealingItemSlot();
	void DestroyKeyItemSlot();

	void FootSound();

	// ü�� Timer
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

	// �ҹ��
	virtual void Smoking() {};

	// ���� ȸ��
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

	// ���¹̳� �κ�
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
	void SetPlayerRun();
	void SetPlayerJump();
	bool GetAttack();

	void OtherSpawnNormalWeapon(const FString& WeaponName);

	void OtherSpawnBleedingHealingItem(const FString& ItemName);

	void OtherSpawnHealingItem(const FString& ItemName);

	void OtherSpawnThrowWeapon(const FString& ItemName);

	void OtherSpawnKeyItem(const FString& ItemName);

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

	// �κ��丮

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

	// ����
	UPROPERTY(EditAnywhere)
	bool m_bIsAttacking = false;

	UPROPERTY(EditAnywhere)
	bool m_bIsPickUping = false;

	// �������� �����ִ���
	UPROPERTY(EditAnywhere)
	bool m_bOnSpotLight = true;

	UPROPERTY(EditAnywhere)
	bool m_bInventoryOn = false;

	UPROPERTY(EditAnywhere)
	FVector m_VStartLocation;

	UPROPERTY(EditAnywhere)
	FName m_nCharacterName;

	// ���� 
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

private:
	uint32 PlayerId = 99;

	FVector NewLocation;

	FVector OldLocation = FVector(0.0f, 0.0f, 0.0f);

	float Speed;
	float PreviousSpeed;

public:
	std::atomic<bool> b_attack{ false };
	bool b_run = false;

	uint32 ItemBoxId = 999;

};
