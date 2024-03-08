// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "ProData/ItemDataStructure.h"
#include "ItemBoxActor.generated.h"

class ANormalWeaponActor;
class ABaseCharacter;

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

	void OnChracterOvelapNew(ABaseCharacter* character);  // 이름 수정 필요

	//void SetInBoxItemId(FString itemid) { m_sItemID = itemid; };
	//FString GetInBoxItemId() { return m_sItemID; }

public:
	UPROPERTY(EditAnywhere, Category = Box)
	UBoxComponent* Trigger;

	UPROPERTY(EditAnywhere, Category = Box)
	UStaticMeshComponent* Box;

	//// 수정 필요	
	//UPROPERTY(EditAnywhere, Category = Box)
	//TSubclassOf<ANormalWeaponActor> NormalWeaponItemClass;


	//UPROPERTY(EditAnywhere, Category = Item)
	//FString m_sItemID;

	// 아이템 이름
	UPROPERTY(EditAnywhere, Category = item)
	FName ItemName;

	// 아이템 종류(클래스가 뭔지)
	UPROPERTY(EditAnywhere, Category = item)
	TEnumAsByte<EItemClass> ItemClassType;

	// 텍스쳐
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Texture;

	// 들고있는 갯수
	UPROPERTY(EditAnywhere, Category = item)
	int Count;


};
