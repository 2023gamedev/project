// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProData/ItemDataStructure.h"    // 여기다 해도 되겠지..?
#include "ItemActor.generated.h"


// 아이템 액터

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite) // 무슨 아이템인진 힐링인지 노말무기인지, 투척무기인지 (필요 없을 수도 있어 삭제할 지 고민중)
	TEnumAsByte<EItemClass> ItemClassType;

};
