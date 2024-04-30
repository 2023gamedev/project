// Fill out your copyright notice in the Description page of Project Settings.


#include "ProBehaviorTree/BTIsinAttackRange.h"
#include "ProZombie//ZombieAIController.h"
#include "ProZombie//ShoutingZombieAIController.h"
#include "ProZombie//RunningZombieAIController.h"
#include "ProCharacter/BaseCharacter.h"
#include "ProZombie//BaseZombie.h"
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

	auto ZombieName = Cast<ABaseZombie>(ControllingPawn)->GetZombieName();




	ABaseCharacter* Target = nullptr;
	if (ZombieName == "NormalZombie") {
		Target = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AZombieAIController::TargetKey));

	}
	else if (ZombieName == "ShoutingZombie") {
		Target = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AShoutingZombieAIController::TargetKey));

	}
	else if (ZombieName == "RunningZombie"){
		Target = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ARunningZombieAIController::TargetKey));
		
	}
	else {
		

		if (Target == nullptr) {

			return false;
		}
		return false;
	}

	if (Target->GetDistanceTo(ControllingPawn) <= 150.f) {
		bResult = true;
		UE_LOG(LogTemp, Error, TEXT("TRUE"));
	}
	else {
		bResult = false;
		UE_LOG(LogTemp, Error, TEXT("FALSE"));
	}


	return bResult;
}
