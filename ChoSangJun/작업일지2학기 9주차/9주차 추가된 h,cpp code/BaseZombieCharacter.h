// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseZombieCharacter.generated.h"

UCLASS()
class REALPROJECT_API ABaseZombieCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseZombieCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	float GetSpeed() { return m_fSpeed; }
	void SetSpeed(float speed) { m_fSpeed = speed; }

	bool IsRun() { return m_bRun; }
	void SetRun(bool run) { m_bRun = run; }

private:

	UPROPERTY(EditAnywhere)
	float m_fSpeed = 0.f;

	UPROPERTY(EditAnywhere)
	bool m_bSitDown = false;

	UPROPERTY(EditAnywhere)
	bool m_bRun = false;



};
