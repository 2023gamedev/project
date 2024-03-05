// Fill out your copyright notice in the Description page of Project Settings.


#include "BTIsinAttackRange.h"
#include "ZombieAIController.h"
#include "ShoutingZombieAIController.h"
#include "RunningZombieAIController.h"
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
		
		//UE_LOG(LogTemp, Error, TEXT("PROBLEM:: %s"), *ZombieName);
		//UE_LOG(LogTemp, Error, TEXT("StartLocation-IsintAttackRange Problem"));
		if (Target == nullptr) {
			//UE_LOG(LogTemp, Error, TEXT("StartLocation-IsintAttackRange Problem11111111"));
			return false;
		}
		//UE_LOG(LogTemp, Error, TEXT("StartLocation-IsintAttackRange Problem2222222222222"));
		return false;
	}

	if (Target == nullptr) {
		return false;
	}

	if (Target->GetDistanceTo(ControllingPawn) <= 200.f) {
		bResult = true;
		UE_LOG(LogTemp, Error, TEXT("TRUE"));
	}
	else {
		bResult = false;
		UE_LOG(LogTemp, Error, TEXT("FALSE"));
	}

	//UE_LOG(LogTemp, Error, TEXT("PROBLEM:: %f"), Target->GetDistanceTo(ControllingPawn));
	//UE_LOG(LogTemp, Error, TEXT("PROBLEM_CHAR:: %s"), *Target->GetActorLocation().ToString());
	//UE_LOG(LogTemp, Error, TEXT("PROBLEM_ZOMBIE:: %s"), *ControllingPawn->GetActorLocation().ToString());

	return bResult;
}
