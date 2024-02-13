// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "ItemBoxActor.generated.h"

class ANormalWeaponActor;

UCLASS()
class PROJECT_API AItemBoxActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBoxActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetInBoxItemId(FString itemid) { m_sItemID = itemid; };
	FString GetInBoxItemId() { return m_sItemID; }

public:
	UPROPERTY(EditAnywhere, Category = Box)
	UBoxComponent* Trigger;

	UPROPERTY(EditAnywhere, Category = Box)
	UStaticMeshComponent* Box;

	// 수정 필요	
	UPROPERTY(EditAnywhere, Category = Box)
	TSubclassOf<ANormalWeaponActor> NormalWeaponItemClass;


	UPROPERTY(EditAnywhere, Category = Item)
	FString m_sItemID;

private:
	UFUNCTION()
	void OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);


};
