// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BasePawn.generated.h"


UCLASS()
class TOONTANKS_API ABasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();


	//UPROPERTY(VisibleInstanceOnly)
	//int32 VisibleInstanceOnlyInt = 11;

	//UPROPERTY(VisibleDefaultsOnly)
	//int32 VisibleDefaultOnlyInt = 5;

	//UPROPERTY(EditDefaultsOnly)
	//int32 EditDefaultOnlyInt = 9;

	//UPROPERTY(EditInstanceOnly)
	//int32 EditInstanceOnlyInt = 14;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//float Speed = 400.f;

	void HandleDestruction();

protected:
	void RotateTurret(FVector LookAtTarget);
	void Fire();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,  Category = "Components", meta =(AllowPrivateAccess = "true"))
	class UCapsuleComponent* CapsuleComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,  Category = "Components", meta =(AllowPrivateAccess = "true"))
	UStaticMeshComponent* BaseMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,  Category = "Components", meta =(AllowPrivateAccess = "true"))
	UStaticMeshComponent* TurretMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,  Category = "Components", meta =(AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,  Category = "Super Duper Varibales", meta =(AllowPrivateAccess = "true"))
	int32 VisibleAnywhereInt = 12;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Super Duper Varibales", meta =(AllowPrivateAccess = "true"))
	int32 EditAnywhereInt = 32;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class UParticlSystem* DeathParticles;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class USoundBase* DeathSound;
};
