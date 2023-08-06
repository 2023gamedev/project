// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

#include "Engine/World.h"
#include "DrawDebugHelpers.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if(PhysicsHandle != nullptr){
		UE_LOG(LogTemp, Display, TEXT("Got Physics Handle: %s"), *PhysicsHandle->GetName());
	}
	else{
		UE_LOG(LogTemp, Display, TEXT("No Phaysics Handle Found"));
	}
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	// if(PhysicsHandle == nullptr){
	// 	return;
	// }
	
	if(PhysicsHandle && PhysicsHandle->GetGrabbedComponent()){
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}

}

// void UGrabber::PrintDamage(const float& Damage)
// {
// 	UE_LOG(LogTemp, Display, TEXT("Damage: %f"), Damage);
// }

// bool UGrabber::HasDamage(float& OutDamage)
// {
// 	OutDamage = 5;
// 	return true;
// }

void UGrabber::Release()
{
	//UE_LOG(LogTemp, Display, TEXT("Released grabber"));
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	// if(PhysicsHandle == nullptr){
	// 	return;
	// }

	if(PhysicsHandle && PhysicsHandle->GetGrabbedComponent()){
		AActor* GrabbedActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();
		GrabbedActor->Tags.remove("Grabbed");
		PhysicsHandle ->ReleaseComponent();
	}
}

void UGrabber::Grab()
{
	// FRotator MyRotation = GetComponentRotation();
	// FString RotationString = MyRotation.ToCompactString();
	// UE_LOG(LogTemp, Display, TEXT("Grabber Rotation: %s"), *RotationString);

	// float Time = GetWorld()->TimeSeconds;
	// UE_LOG(LogTemp, Display, TEXT("Current Time Is: %f"), Time);

	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if(PhysicsHandle == nullptr){
		return;
	}

	FHitResult HitResult;
	bool HasHit = GetGrabbableInReach(HitResult);
	if(HasHit){

		// DrawDebugSphere(GetWorld(), HitResult.Location, 10, 19, FColor::Green, false, 5);
		// DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 19, FColor::Red, false, 5);
		// AActor* HitActor = HitResult.GetActor();
		// UE_LOG(LogTemp, Display, TEXT("Hit actor: %s"), *HitActor->GetActorNameOrLabel());

		UPrimitiveComponent* HitComponent = HitReault.GetComponent();
		HitComponent->SetSimulatePhysics(true);
		HitComponent->WakeAllRigidBodies();
		AActor* HitActor = HitResult.GetActor();
		HitResult.GetActor()->Tags.Add("Grabbed");
		HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		PhysicsHandle->GetComponentAtLocationWithRotation(
			HitComponent,
			NAME_NONE,
			HitResult.ImpactPoint,
			HitResult.GetComponent()->GetComponentRotation(),
		);
	}
	else{
		//UE_LOG(LogTemp, Display, TEXT("No actor hit"));
	}
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{	
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(Result == nullptr){
		UE_LOG(LogTemp, Display, TEXT("Grabber requires a UPhysicsHandleComponent."));
	}
	return Result;
}

bool UGrabber::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Blue, false, 5);

	// float Damage = 5;
	// float& DamageRef = Damage;
	// DamageRef = 5;
	//PrintDamage(Damage);
	// float Damage = 5;
	// if(HasDamage(Damage)){
	// 	PrintDamage(Damage);
	// }
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	bool HasHit = GetWorld()->SweepSingleByChannel(
		HitResult, 
		Start, End, 
		FQuat::Identity, 
		ECC_GameTraceChannel2, 
		Sphere
	);

	return HasHit;
}