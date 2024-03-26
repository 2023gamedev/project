// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "BaseZombie.generated.h"

DECLARE_MULTICAST_DELEGATE(FAttackEndDelegate);
DECLARE_MULTICAST_DELEGATE(FShoutingEndDelegate);

// Zombie Ŭ������ �θ� Ŭ����
UCLASS()
class PROJECT_API ABaseZombie : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseZombie();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void SetZombieName(FString zombiename) { m_sZombieName = zombiename; };
	FString GetZombieName() { return m_sZombieName; }

	float GetHP() { return m_fHP; }
	void SetHP(float hp) { m_fHP = hp; }

	float GetSTR() { return m_fSTR; }
	void SetSTR(float str) { m_fSTR = str; }

	float GetSpeed() { return m_fSpeed; }
	void SetSpeed(float speed) { m_fSpeed = speed; }

	bool IsDie() { return m_bDie; }
	void SetDie(bool die) { m_bDie = die; }

	bool IsFind() { return m_bFind; }
	void SetFind(bool find) { m_bFind = find; }


	bool IsSpecialAbility() { return m_bSpecialAbility; }
	void SetSpecialAbility(bool specialability) { m_bSpecialAbility = specialability; }


	// �Ҹ��ƴ���
	bool IsShouted() { return m_bIsShouted; }
	void SetShouted(bool shouted) { m_bIsShouted = shouted; }
		 
	// ���� �ʿ� ������ animinstance�� �ʿ��� �� ����.
	void Attack();

	UFUNCTION()
	void AttackMontageEnded(UAnimMontage* Montage, bool interrup);

	UPROPERTY(EditAnywhere)
	bool m_bIsAttacking = false;

	FAttackEndDelegate m_DAttackEnd;

	// ���� ������ �� �κе� ���� �ʿ�
	void Shouting();

	UFUNCTION()
	void ShoutingMontageEnded(UAnimMontage* Montage, bool interrup);

	UPROPERTY(EditAnywhere)
	bool m_bIsShouting = false;

	UPROPERTY(EditAnywhere)
	bool m_bIsShouted = false; // �Ҹ��ƴ���

	FShoutingEndDelegate m_DShoutingEnd;


	UPROPERTY(EditAnywhere)
	FString m_sZombieName;


private:

	UPROPERTY(EditAnywhere)
	float m_fHP = 0.f;
	
	UPROPERTY(EditAnywhere)
	float m_fSTR = 0.f;

	UPROPERTY(EditAnywhere)
	float m_fSpeed = 0.f;

	UPROPERTY(EditAnywhere)
	bool m_bDie = false;

	UPROPERTY(EditAnywhere)
	bool m_bFind = false;

	UPROPERTY(EditAnywhere)
	bool m_bSpecialAbility = false;

};
