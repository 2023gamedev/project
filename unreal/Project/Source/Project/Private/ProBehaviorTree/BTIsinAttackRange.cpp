// Fill out your copyright notice in the Description page of Project Settings.


#include "ProBehaviorTree/BTIsinAttackRange.h"
#include "ProZombie//ZombieAIController.h"
#include "ProZombie//ShoutingZombieAIController.h"
#include "ProZombie//RunningZombieAIController.h"
#include "ProCharacter/BaseCharacter.h"
#include "ProZombie//BaseZombie.h"
#include "BehaviorTree/BlackboardComponent.h"

// AI 추가되면 수정될 것 같은 것!!

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

	FVector ToTarget = Target->GetActorLocation() - ControllingPawn->GetActorLocation();
	float DistanceToTarget = ToTarget.Size();

	if (DistanceToTarget > 150.f)
	{
		return false;
	}

	ToTarget.Normalize();
	FVector Forward = ControllingPawn->GetActorForwardVector();
	float DotProduct = FVector::DotProduct(Forward, ToTarget);
	float CosineOfAngle = FMath::Cos(FMath::DegreesToRadians(60.f/2.f));

	if (DotProduct >= CosineOfAngle) {
		bResult = true;
		UE_LOG(LogTemp, Error, TEXT("TRUE"));
	}
	else {
		bResult = false;
		UE_LOG(LogTemp, Error, TEXT("FALSE"));
	}


	//if (Target->GetDistanceTo(ControllingPawn) <= 150.f) {
	//	bResult = true;
	//	UE_LOG(LogTemp, Error, TEXT("TRUE"));
	//}
	//else {
	//	bResult = false;
	//	UE_LOG(LogTemp, Error, TEXT("FALSE"));
	//}


	return bResult;
}
