// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ClearBlackboardBase.generated.h"



/**
 * 
 */
 // Blackboard에서 변수를 초기화하는 클래스
 // AI 추가되면 수정될 것 같은 것!!
UCLASS()
class PROJECT_API UBTTask_ClearBlackboardBase : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_ClearBlackboardBase();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};
