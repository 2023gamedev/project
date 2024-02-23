// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_PatrolPos.generated.h"

/**
 * 
 */
// 필요없는듯?

UCLASS()
class PROJECT_API UBTTask_PatrolPos : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_PatrolPos();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


private:
	bool m_bIsAttacking = false;
};
