// Fill out your copyright notice in the Description page of Project Settings.


#include "ProBehaviorTree/BTService_CanAttack.h"
#include "ProZombie/ZombieAIController.h"
#include "ProZombie//ShoutingZombieAIController.h"
#include "ProZombie//RunningZombieAIController.h"
#include "ProCharacter/BaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ProZombie//BaseZombie.h"
#include "DrawDebugHelpers.h"

// AI 추가되면 수정될 것 같은 것!!

UBTService_CanAttack::UBTService_CanAttack()
{
	NodeName = "SCanAttack";
	Interval = 1.f;
}

void UBTService_CanAttack::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr) {
		return;
	}

	auto ZombieName = Cast<ABaseZombie>(ControllingPawn)->GetZombieName();

	AAIController* aicontroller = Cast<AAIController>(ControllingPawn->GetController());



	ABaseCharacter* Target = nullptr;
	if (ZombieName == "NormalZombie") {
		Target = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AZombieAIController::TargetKey));
		ControllingPawn->GetController()->StopMovement();
	}
	else if (ZombieName == "ShoutingZombie") {
		Target = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AShoutingZombieAIController::TargetKey));
		ControllingPawn->GetController()->StopMovement();
	}
	else if (ZombieName == "RunningZombie") {
		Target = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ARunningZombieAIController::TargetKey));
		ControllingPawn->GetController()->StopMovement();
	}
	else {
		if (Target == nullptr) {
			return;
		}
		return;
	}

	if (Target != nullptr) {
		if (Target->GetDistanceTo(ControllingPawn) <= 150.f) {
			ControllingPawn->GetController()->StopMovement();
		}
	}


}
