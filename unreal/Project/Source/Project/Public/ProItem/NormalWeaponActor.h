// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemActor.h"
#include "ProGamemode/ProGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "NormalWeaponActor.generated.h"

/**
 * 
 */


enum WeaponDurability {
	Book = 4,
	Bottle = 3,
	ButchersKnife = 7,
	FireAxe = 10,
	FireExtinguisher = 10,
	FryingPan = 5,
	GolfClub = 7,
	Iron = 6,
	MagicStick = 6,
	MannequinArm = 6,
	MannequinLeg = 6,
	Pipe = 8,
	Plunger = 7,
	SashimiKnife = 5,
	Scissors = 5,
	Shovels = 9,
	SquareWood = 5,
	WoodenBat = 7
};


class ABaseCharacter;


UCLASS()
class PROJECT_API ANormalWeaponActor : public AItemActor	// 무기 액터
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* PlaneComponent;
	
	UPROPERTY(EditAnywhere)
	ABaseCharacter* OwnerCharacter;

	UPROPERTY(EditAnywhere)
	FName WeaponName;


	// 무기의 스탯들
	UPROPERTY(EditAnywhere, Category = "ItemStat")
	float m_fWeaponSTR;

	UPROPERTY(EditAnywhere, Category = "ItemStat")
	float m_fWeaponDurability;		// 현재 내구도
	UPROPERTY(EditAnywhere, Category = "ItemStat")
	float m_fWeaponDurability_Max;	// 본래 내구도

	UPROPERTY(EditAnywhere, Category = "ItemStat")
	float m_fWeaponRange;


public:
	UProGameInstance* GameInstance;
};
