// Fill out your copyright notice in the Description page of Project Settings.


#include "ProBehaviorTree/BTService_Detect.h"
#include "ProZombie/ZombieAIController.h"
#include "ProZombie/BaseZombie.h"
#include "ProZombie/RunningZombieAIController.h"
#include "ProZombie/ShoutingZombieAIController.h"
#include "ProCharacter/BaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = "Detect";
	Interval = 2.f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode( OwnerComp, NodeMemory, DeltaSeconds);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr) {
		return;
	}

	ABaseZombie* Zombie = Cast<ABaseZombie>(ControllingPawn);

	AAIController* aicontroller = Cast<AAIController>(ControllingPawn->GetController());

	if (Zombie->GetZombieName() == "NormalZombie") {
		AZombieAIController* AIZombieController = Cast<AZombieAIController>(ControllingPawn->GetController());
		if (AIZombieController) {
			if (AIZombieController->m_bFootSound) {
				AIZombieController->GetBlackboardComponent()->SetValueAsVector(TEXT("FootSoundLocation"), AIZombieController->m_vFootCharacterPos);
				AIZombieController->m_bFootSound = false;
			}
			if (AIZombieController->m_bShoutingSound) {
				AIZombieController->GetBlackboardComponent()->SetValueAsVector(TEXT("ShoutingSoundLocation"), AIZombieController->m_vShoutingPos);
				AIZombieController->m_bShoutingSound = false;
			}
		}
	}
	else if (Zombie->GetZombieName() == "ShoutingZombie") {
		AShoutingZombieAIController* AIShoutingZombieController = Cast<AShoutingZombieAIController>(ControllingPawn->GetController());
		if (AIShoutingZombieController) {
			if (AIShoutingZombieController->m_bFootSound) {
				AIShoutingZombieController->GetBlackboardComponent()->SetValueAsVector(TEXT("FootSoundLocation"), AIShoutingZombieController->m_vFootCharacterPos);
				AIShoutingZombieController->m_bFootSound = false;
			}
			if (AIShoutingZombieController->m_bShoutingSound) {
				AIShoutingZombieController->GetBlackboardComponent()->SetValueAsVector(TEXT("ShoutingSoundLocation"), AIShoutingZombieController->m_vShoutingPos);
				AIShoutingZombieController->m_bShoutingSound = false;
			}
		}
	}
	else if (Zombie->GetZombieName() == "RunningZombie") {
		ARunningZombieAIController* AIRunningZombieController = Cast<ARunningZombieAIController>(ControllingPawn->GetController());
		if (AIRunningZombieController) {
			if (AIRunningZombieController->m_bFootSound) {
				AIRunningZombieController->GetBlackboardComponent()->SetValueAsVector(TEXT("FootSoundLocation"), AIRunningZombieController->m_vFootCharacterPos);
				AIRunningZombieController->m_bFootSound = false;
			}
			if (AIRunningZombieController->m_bShoutingSound) {
				AIRunningZombieController->GetBlackboardComponent()->SetValueAsVector(TEXT("ShoutingSoundLocation"), AIRunningZombieController->m_vShoutingPos);
				AIRunningZombieController->m_bShoutingSound = false;
			}
		}
	}
	else {
		return;
	}
}
