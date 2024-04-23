// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemActor.h"
#include "Components/BoxComponent.h"
#include "NormalWeaponActor.generated.h"

/**
 * 
 */

// 무기 액터
class ABaseCharacter;

UCLASS()
class PROJECT_API ANormalWeaponActor : public AItemActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANormalWeaponActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PlaceItem() override;

	UFUNCTION()
	void WeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);


public:
	UPROPERTY(EditAnywhere, Category= NormalWeapon)
	UStaticMeshComponent* NormalWeapon;

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, Category = "ItmePos")
	FVector ItemHandPos;

	UPROPERTY(EditAnywhere, Category = "ItmeRotate")
	FRotator ItemHandRot;

	UPROPERTY(EditAnywhere)
	ABaseCharacter* OwnerCharacter;

	UPROPERTY(EditAnywhere)
	FName WeaponName;


	// 무기의 스탯들
	UPROPERTY(EditAnywhere, Category = "ItemStat")
	float m_fWeaponSTR;

	UPROPERTY(EditAnywhere, Category = "ItemStat")
	float m_fWeaponDurability;

	UPROPERTY(EditAnywhere, Category = "ItemStat")
	float m_fWeaponRange;

	// 무기의 스탯들
	UPROPERTY(EditAnywhere, Category = "ItemStat")
	float m_fCharacterSTR;
};
