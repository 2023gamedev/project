// Fill out your copyright notice in the Description page of Project Settings.


#include "OtherCharacter.h"

// Sets default values
AOtherCharacter::AOtherCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ClientSocketPtr = nullptr;
}

// Called when the game starts or when spawned
void AOtherCharacter::BeginPlay()
{
	Super::BeginPlay();

	ClientSocket* SocketInstance = new ClientSocket();
	this->SetClientSocket(SocketInstance);
	
}

// Called every frame
void AOtherCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector OldLocation = GetActorLocation();
	FVector NewLocation = recvPlayerData.Location;

    float DistanceMoved = FVector::Dist(OldLocation, NewLocation);
    float Speed = (DeltaTime > 0) ? (DistanceMoved / DeltaTime) : 0;

    // 애니메이션 인스턴스에 속도 파라미터 설정
    UMyAnimInstance* AnimInstance = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
    if (AnimInstance)
    {
        AnimInstance->UpdateSpeed(Speed);
    }

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

