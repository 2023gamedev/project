// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerCharacter.generated.h"


UCLASS()
class REALPROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



	UPROPERTY(EditAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	void MoveForward(float NewAxisValue);
	void MoveLeft(float NewAxisValue);
	void Run();
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);
	void Throw();

	float GetSpeed() { return m_fSpeed; }
	void SetSpeed(float speed) { m_fSpeed = speed; }

	bool IsSitDown() { return m_bSitDown; }
	void SetSitDown(bool sitdown) { m_bSitDown = sitdown; }

	bool IsRun() { return m_bRun; }
	void SetRun(bool run) { m_bRun = run; }

	bool IsHandInBall() { return m_bHandInBall; }
	void SetHandInBall(bool hinball) { m_bHandInBall = hinball; }


private:

	UPROPERTY(EditAnywhere)
	float m_fSpeed = 0.f;

	UPROPERTY(EditAnywhere)
	bool m_bSitDown = false;

	UPROPERTY(EditAnywhere)
	bool m_bRun = false;

	UPROPERTY(EditAnywhere)
	bool m_bHandInBall = false;
};
