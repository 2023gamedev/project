// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// 구조체
#include "ProData/ItemDataStructure.h"

#include "BaseCharacter.generated.h"

class UGamePlayerUI;
class UConditionUI;
class UPlayerSight;
class ANormalWeaponActor;
class AThrowWeaponActor;
class AHealingItemActor;
class ABleedingHealingItemActor;
class AKeyActor;


DECLARE_DELEGATE_FourParams(FThrowOnGround, FName, EItemClass, UTexture2D*, int);
DECLARE_MULTICAST_DELEGATE(FAttackEndPlayerDelegate);

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

	void AttackCheck();

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

	UPROPERTY(EditAnywhere);
	AKeyActor* CurrentKeyItem;
	

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

	UFUNCTION()
	void AttackMontageEnded(UAnimMontage* Montage, bool interrup);




	// input
	void MoveForward(FVector RotateYaw, float NewAxisValue);
	void MoveLeft(FVector RotateYaw , float NewAxisValue);
	void Run();
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);
	void GetItem();
	void LightOnOff();
	void InventoryOnOff();

	FAttackEndPlayerDelegate m_DAttackEnd;
	void Attack();

	void QuickNWeapon();
	void QuickBHItem();
	void QuickHItem();
	void QuickTWeapon();
	void QuickKeyItem();

	float GetHP() { return m_fHP; }
	void SetHP(float hp) { m_fHP = hp; }

	float GetSTR() { return m_fSTR; }
	void SetSTR(float str) { m_fSTR = str; }

	float GetSpeed() { return m_fSpeed; }
	void SetSpeed(float speed) { m_fSpeed = speed; }

	float GetStamina() { return m_fStamina; }
	void SetStamina(float stamina) { m_fStamina = stamina; }

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

	void SetCharacterName(FString charactername) { m_sCharacterName = charactername; };
	FString GetCharacterName() { return m_sCharacterName; }

	void SetStartLocation(FVector startlocation) { m_VStartLocation = startlocation; }
	FVector GetStartLocation() { return m_VStartLocation; }

	bool CanSetWeapon();
	void SetWeapon(ANormalWeaponActor* NewWeapon);

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


	virtual uint32 GetPlayerId() const;
	void SetPlayerId(uint32 NewPlayerId);
	void UpdatePlayerData(FVector Location);

	void SetAttack(bool b_attack);
	bool GetAttack();

private:
	UPROPERTY(EditAnywhere)
	float m_fHP = 0.f;

	UPROPERTY(EditAnywhere)
	float m_fSTR = 0.f;

	UPROPERTY(EditAnywhere)
		float m_fSpeed = 0.f;

	UPROPERTY(EditAnywhere)
		float m_fStamina = 0.f;

	UPROPERTY(EditAnywhere)
		bool m_bBleeding = false;

	UPROPERTY(EditAnywhere)
		bool m_bSitDown = false;

	UPROPERTY(EditAnywhere)
		bool m_bRun = false;

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

	// 어택
	UPROPERTY(EditAnywhere)
	bool m_bIsAttacking = false;

	// 손전등이 켜져있는지
	UPROPERTY(EditAnywhere)
	bool m_bOnSpotLight = true;

	UPROPERTY(EditAnywhere)
	bool m_bInventoryOn = false;

	UPROPERTY(EditAnywhere)
	FVector m_VStartLocation;

	UPROPERTY(EditAnywhere)
	FString m_sCharacterName;

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

private:
	uint32 PlayerId = 99;

	FVector NewLocation;

	FVector OldLocation = FVector(0.0f, 0.0f, 0.0f);

	float Speed;
	float PreviousSpeed;

	bool b_attack = false;

};
