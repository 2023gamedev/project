// Fill out your copyright notice in the Description page of Project Settings.


#include "BTIsinAttackRange.h"
#include "ZombieAIController.h"
#include "BaseCharacter.h"
#include "BaseZombie.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTIsinAttackRange::UBTIsinAttackRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTIsinAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr) {
		return false;
	}

	auto Target = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AZombieAIController::TargetKey));
	if (Target == nullptr) {
		return false;
	}

	bResult = (Target->GetDistanceTo(ControllingPawn) <= 200.f);

	return bResult;
}
