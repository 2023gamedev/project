// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Shouting.generated.h"



/**
 * 
 */
 // Zombie�� �Ҹ�ġ�� Ŭ����
 // AI �߰��Ǹ� ������ �� ���� ��!!
UCLASS()
class PROJECT_API UBTTask_Shouting : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Shouting();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


private:
	bool m_bIsShouting = false;

};
