// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shouting.h"
#include "ShoutingZombieAIController.h"
#include "BaseZombie.h"

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

