// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "ZombieAIController.h"
#include "BaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = "Detect";
	Interval = 1.f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode( OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 500.f;

	if (nullptr == World) return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_EngineTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	// ¼öÁ¤Áß
	//if (bResult) {
	//	for (auto const& OverlapResult : OverlapResults) {
	//		ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(OverlapResult.GetActor());
	//		if (BaseCharacter && BaseCharacter->GetController()->IsPlayerController()) {
	//			OwnerComp.GetBlackboardComponent()->SetValueAsObject(AZombieAIController::TargetKey, BaseCharacter);
	//			DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
	//		}

	//		DrawDebugPoint(World, BaseCharacter->GetActorLocation(), 10.f, FColor::Blue, false, 0.2f);

	//		DrawDebugLine(World, ControllingPawn->GetActorLocation(), BaseCharacter->GetActorLocation(), FColor::Blue, false, 0.2f);
	//		return;
	//	}
	//}


	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}
