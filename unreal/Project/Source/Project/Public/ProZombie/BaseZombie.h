// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "ProceduralMeshComponent.h"
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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


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

	bool IsDie() { return m_bDie; }
	void SetDie(bool die) { m_bDie = die; }

	bool IsFind() { return m_bFind; }
	void SetFind(bool find) { m_bFind = find; }


	bool IsSpecialAbility() { return m_bSpecialAbility; }
	void SetSpecialAbility(bool specialability) { m_bSpecialAbility = specialability; }

	bool IsShouted() { return m_bIsShouted; }
	void SetShouted(bool shouted) { m_bIsShouted = shouted; }
		 

	void Attack();

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

	FTimerHandle AttakcedStunHandle;

	void StartAttackedStunHandle();

	void AttackedStunTimerElapsed();

	void UpdateLastKnownPositionByFootSound(FVector playerlocation);

	void UpdateLastKnownPositionByShoutingSound(FVector playerlocation);

	void StopAITree();
	void StartAITree();


	UFUNCTION(BlueprintCallable)
	void CutZombie(FName bonename);

	// Procedural mesh component for the cut part
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UProceduralMeshComponent* CutProceduralMesh;

	void CreativeProceduralMesh(FName bonename);

	UPROPERTY(EditAnywhere, Category = StaticMesh)
	UStaticMeshComponent* CopyStaticMesh;

	void GetBoneInfluencedVertices(USkeletalMeshComponent* SkeletalMeshComp, FName BoneName, TArray<int32>& OutVertexIndices);
	void CreateProceduralMeshFromBoneVertices(USkeletalMeshComponent* SkeletalMeshComp, FName BoneName, UProceduralMeshComponent* ProceduralMeshComp);


	void FillCutMeshGap(USkeletalMeshComponent* SkeletalMeshComp, const TArray<FVector>& CutVertices);
	void FindVertexIndices(USkeletalMeshComponent* SkeletalMeshComp, const TArray<FVector>& CutVertices, TArray<int32>& OutVertexIndices);
	void FindAdjacentVertices(USkeletalMeshComponent* SkeletalMeshComp, const TArray<int32>& VertexIndices, TArray<int32>& OutAdjacentVertices);
	void GenerateNewVertices(USkeletalMeshComponent* SkeletalMeshComp, const TArray<int32>& VertexIndices, const TArray<int32>& AdjacentVertices, TArray<FVector>& OutNewVertices);
	void GenerateNewTriangles(const TArray<int32>& VertexIndices, const TArray<int32>& AdjacentVertices, TArray<int32>& OutNewTriangles);
	void AddNewGeometryToMesh(UProceduralMeshComponent* ProceduralMeshComp, const TArray<FVector>& NewVertices, const TArray<int32>& NewTriangles);

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* Material;

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* Material2;

	UPROPERTY(EditAnywhere, Category = "Materials")
	UMaterialInterface* Material3;

	UPROPERTY(EditAnywhere)
	ABloodNiagaEffect* BloodFX;

	UPROPERTY(EditAnywhere)
	AShoutingNiagaEffect* ShoutingFX;

private:

	UPROPERTY(EditAnywhere)
	float m_fHP = 0.f;

	UPROPERTY(EditAnywhere)
	float m_fStartHP = 0.f;
	
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

	UPROPERTY(EditAnywhere)
	float m_fAttackRadius = 50.f;

	UPROPERTY(EditAnywhere)
	float m_fAttackRange = 100.f;


public:
	uint32 ZombieId;
	FVector NewLocation;

};
