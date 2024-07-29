// Fill out your copyright notice in the Description page of Project Settings.


#include "ProBehaviorTree/BTTask_ClearBlackboardBase.h"
#include "BehaviorTree/BlackboardComponent.h"

// AI �߰��Ǹ� ������ �� ���� ��!!

UBTTask_ClearBlackboardBase::UBTTask_ClearBlackboardBase()
{
	NodeName = TEXT("Clear Blackboard Value");
}

EBTNodeResult::Type UBTTask_ClearBlackboardBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());

	return EBTNodeResult::Type();
}
