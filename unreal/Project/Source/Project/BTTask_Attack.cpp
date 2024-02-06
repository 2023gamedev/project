// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "ZombieAIController.h"
#include "RunningZombieAIController.h"
#include "ShoutingZombieAIController.h"
#include "BaseZombie.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
	m_bIsAttacking = false;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto BaseZombie = Cast<ABaseZombie>(OwnerComp.GetAIOwner()->GetPawn());
	if (BaseZombie == nullptr) {
		return EBTNodeResult::Failed;
	}

	BaseZombie->Attack();
	m_bIsAttacking = true;
	BaseZombie->m_DAttackEnd.AddLambda([this]() -> void {
		m_bIsAttacking = false;
		});

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!m_bIsAttacking) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
