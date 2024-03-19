// Fill out your copyright notice in the Description page of Project Settings.


#include "OtherCharacter.h"

// Sets default values
AOtherCharacter::AOtherCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Speed = 0;
	PreviousSpeed = 0;
}

// Called when the game starts or when spawned
void AOtherCharacter::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void AOtherCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OldLocation != FVector(0.0f, 0.0f, 0.0f)) {
		float DistanceMoved = FVector::Dist(OldLocation, NewLocation);
		Speed = (DeltaTime > 0) ? (DistanceMoved / DeltaTime) : 0;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Current Speed: %f"), Speed);

    // 애니메이션 인스턴스에 속도 파라미터 설정
	if ((Speed != 0 && PreviousSpeed == 0) || (Speed == 0 && PreviousSpeed != 0))
	{
		UMyAnimInstance* AnimInstance = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
		if (AnimInstance) {
			AnimInstance->SetCurrentPawnSpeed(Speed);
		}
	}

	PreviousSpeed = Speed;
	OldLocation = NewLocation;

}

// Called to bind functionality to input
void AOtherCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

uint32 AOtherCharacter::GetPlayerId() const
{
	return PlayerId;
}

void AOtherCharacter::SetPlayerId(uint32 NewPlayerId)
{
	PlayerId = NewPlayerId;
}

void AOtherCharacter::UpdatePlayerData(FVector Location)
{
	NewLocation = Location;
}