// Copyright Epic Games, Inc. All Rights Reserved.

#include "DummyClientGameMode.h"
#include "DummyClientCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "OtherCharacter.h"
#include "EngineUtils.h"

ADummyClientGameMode::ADummyClientGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

    static ConstructorHelpers::FClassFinder<ACharacter> OtherCharacterBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_OtherCharacter"));
    if (OtherCharacterBPClass.Class != NULL)
    {
        BP_OtherCharacterClass = OtherCharacterBPClass.Class;
    }
}

void ADummyClientGameMode::UpdateOtherPlayer(uint32 PlayerId, FVector NewLocation, FRotator NewRotation)
{
    UWorld* World = GetWorld();

    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("UpdateOtherPlayer: GetWorld() returned nullptr"));
        return;
    }

    // ĳ���� �˻�
    for (TActorIterator<AOtherCharacter> It(World); It; ++It)
    {
        AOtherCharacter* OtherPlayer = *It;
        if (OtherPlayer && OtherPlayer->GetPlayerId() == PlayerId)
        {
            // ���� ĳ���� ��ġ ������Ʈ
            OtherPlayer->SetActorLocation(NewLocation);
            OtherPlayer->SetActorRotation(NewRotation);
            return; // ��ġ�� ������Ʈ �����Ƿ� �Լ� ����
        }
    }

    // ���� ĳ���͸� ã�� ���� ��쿡�� �� ĳ���� ����
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    AOtherCharacter* NewCharacter = World->SpawnActor<AOtherCharacter>(BP_OtherCharacterClass, NewLocation, NewRotation, SpawnParams);

    if (NewCharacter)
    {
        // �� ĳ���Ϳ� PlayerId ����
        NewCharacter->SetPlayerId(PlayerId);
    }
}