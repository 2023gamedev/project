// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "tuple"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "ProceduralMeshComponent.h"
#include "ProZombie/ZombieAnimInstance.h"
#include "ProGamemode/ProGameInstance.h"
#include "ProCharacter/BaseCharacter.h"

#include "BaseZombie.generated.h"


DECLARE_MULTICAST_DELEGATE(FAttackEndDelegate);
DECLARE_MULTICAST_DELEGATE(FShoutingEndDelegate);

class ABloodNiagaEffect;
class AShoutingNiagaEffect;

// Zombie 클래스의 부모 클래스
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

	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	//void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);


	void SetZombieName(FName zombiename) { m_sZombieName = zombiename; };
	FName GetZombieName() { return m_sZombieName; }

	float GetHP() { return m_fHP; }
	void SetHP(float hp) { m_fHP = hp; }

	float GetStartHP() { return m_fStartHP; }
	void SetStartHP(float hp) { m_fStartHP = hp; }

	float GetSTR() { return m_fSTR; }
	void SetSTR(float str) { m_fSTR = str; }

	float GetSpeed() { return m_fSpeed; }
	void SetSpeed(float speed) { m_fSpeed = speed; }	
	
	float GetTurningSpeed() { return m_fTurningSpeedDegree; }
	void SetTurningSpeed(float degree) { m_fTurningSpeedDegree = degree; }
		
	bool IsDie() { return m_bDie; }
	void SetDie(bool die) { m_bDie = die; }

	bool IsFind() { return m_bFind; }
	void SetFind(bool find) { m_bFind = find; }


	bool IsSpecialAbility() { return m_bSpecialAbility; }
	void SetSpecialAbility(bool specialability) { m_bSpecialAbility = specialability; }

	bool IsShouted() { return m_bIsShouted; }
	void SetShouted(bool shouted) { m_bIsShouted = shouted; }
		 

	void Attack(uint32 PlayerId);

	UFUNCTION()
	void AttackMontageEnded(UAnimMontage* Montage, bool interrup);

	UPROPERTY(EditAnywhere)
	bool m_bIsAttacking = false;

	FAttackEndDelegate m_DAttackEnd;

	void AttackCheck();


	void Shouting();

	UFUNCTION()
	void ShoutingMontageEnded(UAnimMontage* Montage, bool interrup);

	void BeAttacked();

	UFUNCTION()
	void BeAttackedMontageEnded(UAnimMontage* Montage, bool interrup);

	UFUNCTION()
	void OnZombieHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


	UPROPERTY(EditAnywhere)
	bool m_bBeAttacked = false;

	void SetNormalDeadWithAnim();
	void SetCuttingDeadWithAnim();


	void SetZombieId(uint32 NewPlayerId);
	uint32 GetZombieId() const;
	void UpdateZombieData(FVector Location);


	UPROPERTY(EditAnywhere)
	bool m_bIsShouting = false;

	UPROPERTY(EditAnywhere)
	bool m_bIsShouted = false; // 소리쳤는지

	UPROPERTY(EditAnywhere)
	bool m_bIsNormalDead = false;

	UPROPERTY(EditAnywhere)
	bool m_bIsCuttingOverlapOn = false;

	UPROPERTY(EditAnywhere)
	bool m_bIsCuttingDead = false;


	UPROPERTY(EditAnywhere)
	bool procMesh_AddImpulse_1 = false;

	UPROPERTY(EditAnywhere)
	bool procMesh_AddImpulse_2 = false;


	FShoutingEndDelegate m_DShoutingEnd;


	UPROPERTY(EditAnywhere)
	FName m_sZombieName;

	// TimerHandle 30초 뒤에 부활하기 위해
	FTimerHandle ResurrectionHandle;

	void StartResurrectionTimer();

	void ResurrectionTimerElapsed();


	FTimerHandle WattingHandle;

	void StartWatiingTimer();

	void WaittingTimerElapsed();

	UPROPERTY(EditAnywhere)
	bool m_bIsStanding = false;

	UFUNCTION(BlueprintCallable)
	void CutZombie(FVector planeposition, FVector planenormal);

	void SliceProceduralmeshTest(FVector planeposition, FVector planenormal);

	// Procedural mesh component for the cut part
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ProcMesh")
	UProceduralMeshComponent* CutProceduralMesh_1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ProcMesh") 
	UProceduralMeshComponent* CutProceduralMesh_2;

	void CreativeProceduralMesh(FVector planeposition, FVector planenormal);

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* Material_Blood;

	UPROPERTY(EditAnywhere)
	TArray<ABloodNiagaEffect*> BloodFX;

	UPROPERTY(EditAnywhere)
	AShoutingNiagaEffect* ShoutingFX;

	float SetImpulseByWeight(float targetWeight, float baseImpulse);

	UPROPERTY(EditAnywhere)
	FVector WeaponForward;


	float m_fHP_Prev = 0.f;

	bool doAction_takeDamage_onTick = true;		// 좀비 피격시 클라 동기화용 (다른 클라가 좀비를 때렸을때)
	bool doAction_setIsNormalDead_onTick = true;	// 좀비 사망시 클라 동기화용 (다른 클라가 좀비를 죽였을때)

private:

	UPROPERTY(EditAnywhere)
	float m_fHP = 0.f;

	UPROPERTY(EditAnywhere)
	float m_fStartHP = 0.f;
	
	UPROPERTY(EditAnywhere)
	float m_fSTR = 0.f;

	UPROPERTY(EditAnywhere)
	float m_fSpeed = 0.f;

	// 근데 m_bDie 변수 사용 안하고 있긴 함
	UPROPERTY(EditAnywhere)
	bool m_bDie = false;	

	UPROPERTY(EditAnywhere)
	bool m_bFind = false;

	UPROPERTY(EditAnywhere)
	bool m_bSpecialAbility = false;

	UPROPERTY(EditAnywhere)
	float m_fAttackRadius = 50.f;

	UPROPERTY(EditAnywhere)
	float m_fAttackRange = 100.f;

	// 1초당 고개 돌리는 각도
	UPROPERTY(EditAnywhere)
	float m_fTurningSpeedDegree;

public:
	UPROPERTY(EditAnywhere)
	UZombieAnimInstance* CachedAnimInstance;

	UProGameInstance* GameInstance;

	UPROPERTY(EditAnywhere)
	uint32 ZombieId = 99;

	FVector NewLocation;

	FVector OldLocation = FVector(0.0f, 0.0f, 0.0f);

	//UPROPERTY(VisibleAnywhere)
	std::tuple<float, float, float> NextPath[2];

	UPROPERTY(EditAnywhere)
	FVector TargetLocation;

	enum FLOOR {
		Zero, B2, B1, F1, F2
	};

	UPROPERTY(EditAnywhere)
	int floor = FLOOR::Zero;

	UPROPERTY(EditAnywhere)
	ABaseCharacter* MyChar;

	bool print_Velocity_1 = true;
	bool print_Velocity_2 = true;

};
