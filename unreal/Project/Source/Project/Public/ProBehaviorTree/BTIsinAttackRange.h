// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTIsinAttackRange.generated.h"


/**
 * 
 */
// Zombie의 공격범위에 Player가 들어와 있는가에 대한 Decorator Class
// AI 추가되면 수정될 것 같은 것!!
UCLASS()
class PROJECT_API UBTIsinAttackRange : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTIsinAttackRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
