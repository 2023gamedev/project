// Fill out your copyright notice in the Description page of Project Settings.


#include "ProBehaviorTree/BTTask_Shouting.h"
#include "ProZombie/ShoutingZombieAIController.h"
#include "ProZombie/BaseZombie.h"

// AI 추가되면 수정될 것 같은 것!!

UBTTask_Shouting::UBTTask_Shouting()
{
	bNotifyTick = true;
	m_bIsShouting = false;
}

EBTNodeResult::Type UBTTask_Shouting::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto BaseZombie = Cast<ABaseZombie>(OwnerComp.GetAIOwner()->GetPawn());
	if (BaseZombie == nullptr) {
		return EBTNodeResult::Failed;
	}

	BaseZombie->Shouting();
	m_bIsShouting = true;
	BaseZombie->m_DShoutingEnd.AddLambda([this]() -> void {
		m_bIsShouting = false;
		});

	return EBTNodeResult::InProgress;
}

void UBTTask_Shouting::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!m_bIsShouting) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

