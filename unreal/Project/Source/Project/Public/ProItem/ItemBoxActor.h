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

	void OnChracterOvelapNew(ABaseCharacter* character);  // �̸� ���� �ʿ�

	//void SetInBoxItemId(FString itemid) { m_sItemID = itemid; };
	//FString GetInBoxItemId() { return m_sItemID; }

public:
	UPROPERTY(EditAnywhere, Category = Box)
	UBoxComponent* Trigger;

	UPROPERTY(EditAnywhere, Category = Box)
	UStaticMeshComponent* Box;

	//// ���� �ʿ�	
	//UPROPERTY(EditAnywhere, Category = Box)
	//TSubclassOf<ANormalWeaponActor> NormalWeaponItemClass;


	//UPROPERTY(EditAnywhere, Category = Item)
	//FString m_sItemID;

	// ������ �̸�
	UPROPERTY(EditAnywhere, Category = item)
	FName ItemName;

	// ������ ����(Ŭ������ ����)
	UPROPERTY(EditAnywhere, Category = item)
	TEnumAsByte<EItemClass> ItemClassType;

	// �ؽ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Texture;

	// ����ִ� ����
	UPROPERTY(EditAnywhere, Category = item)
	int Count;


};
