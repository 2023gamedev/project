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
#include "ItemDataStructure.h"

#include "BaseCharacter.generated.h"

class UGamePlayerUI;
class UPlayerSight;
class ANormalWeaponActor;

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

	bool DraggingSwap(int from, ESlotType fromtype, int to, ESlotType totype);
	bool SwapInven(int from, int to);

	// 스프링 암
	UPROPERTY(EditAnywhere)
		USpringArmComponent* SpringArm;

	// 카메라
	UPROPERTY(EditAnywhere)
		UCameraComponent* Camera;

	// 플레이어가 물체를 바라보는지
	UPROPERTY(EditAnywhere)
		UPlayerSight* PlayerSight;

	UPROPERTY(EditAnywhere)
	ANormalWeaponActor* CurrentWeapon;

	UPROPERTY(EditAnywhere)
	USpotLightComponent* FlashLight;

	// 게임 전체 UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TArray<FItemDataStructure> Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UGamePlayerUI> GameUIClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UGamePlayerUI* GameUIWidget;
	

	void MoveForward(float NewAxisValue);
	void MoveLeft(float NewAxisValue);
	void Run();
	//void Jump();
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);
	void GetItem();
	void LightOnOff();
	void InventoryOnOff();

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

	bool IsHandIn() { return m_bHandIn; }
	void SetHandIn(bool handin) { m_bHandIn = handin; }

	bool IsSpecialEffect() { return m_bSpecialEffect; }
	void SetSpecialEffect(bool specialeffect) { m_bSpecialEffect = specialeffect; }

	bool IsSpotLight() { return m_bOnSpotLight; }
	void SetSpotLight(bool spotlight) { m_bOnSpotLight = spotlight; }

	bool IsInventory() { return m_bInventoryOn; }
	void SetInventory(bool inven) { m_bInventoryOn = inven; }

	void SetCharacterName(FString charactername) { m_sCharacterName = charactername; };
	FString GetCharacterName() { return m_sCharacterName; }

	bool CanSetWeapon();
	void SetWeapon(ANormalWeaponActor* NewWeapon);

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
		bool m_bHandIn = false;
		
	UPROPERTY(EditAnywhere)
		bool m_bSpecialEffect = false;

	// 손전등이 켜져있는지
	UPROPERTY(EditAnywhere)
	bool m_bOnSpotLight = true;

	UPROPERTY(EditAnywhere)
	bool m_bInventoryOn = true;



	UPROPERTY(EditAnywhere)
	FString m_sCharacterName;

};
