// Fill out your copyright notice in the Description page of Project Settings.


#include "ProBehaviorTree/BTIsShouting.h"
#include "ProZombie/ZombieAIController.h"
#include "ProZombie/ShoutingZombieAIController.h"
#include "ProZombie/RunningZombieAIController.h"
#include "ProCharacter/BaseCharacter.h"
#include "ProZombie/BaseZombie.h"
#include "BehaviorTree/BlackboardComponent.h"

// AI 추가되면 수정될 것 같은 것!!

UBTIsShouting::UBTIsShouting()
{
	NodeName = TEXT("IsShouting");
}

bool UBTIsShouting::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr) {
		return false;
	}


	auto Zombie = Cast<ABaseZombie>(ControllingPawn);

	
	// 수정 요망
	if (Zombie->GetZombieName() == "NormalZombie") {	
		bResult = !(Zombie->IsShouted());
	}
	else if (Zombie->GetZombieName() == "ShoutingZombie") {
		bResult = !(Zombie->IsShouted());
		UE_LOG(LogTemp, Log, TEXT("IsShouted :: %s"), bResult ? TEXT("true") : TEXT("false"));
	}
	else if (Zombie->GetZombieName() == "RunningZombie") {
		bResult = !(Zombie->IsShouted());
	}
	else {
		return false;
	}
	return bResult;
}
