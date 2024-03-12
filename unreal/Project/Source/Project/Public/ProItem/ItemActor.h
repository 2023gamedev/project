// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProData/ItemDataStructure.h"    // ����� �ص� �ǰ���..?
#include "ItemActor.generated.h"


// ������ ����

UCLASS()
class PROJECT_API AItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PlaceItem();

	//void SetItemName(FString itemname) { m_sItemName = itemname; };
	//FString GetItemName() { return m_sItemName; }

public:
	//UPROPERTY(EditAnywhere)
	//FString m_sItemName;

	UPROPERTY(EditAnywhere)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) // ���� ���������� �������� �븻��������, ��ô�������� (�ʿ� ���� ���� �־� ������ �� �����)
	TEnumAsByte<EItemClass> ItemClassType;

};
