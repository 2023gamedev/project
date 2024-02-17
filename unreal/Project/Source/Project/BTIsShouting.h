// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTIsShouting.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UBTIsShouting : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTIsShouting();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
