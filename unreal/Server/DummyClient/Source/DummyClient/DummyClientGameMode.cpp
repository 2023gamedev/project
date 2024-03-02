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
}

void ADummyClientGameMode::UpdateOtherPlayer(uint32 PlayerId, FVector NewLocation)
{
    UWorld* World = GetWorld();

    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("UpdateOtherPlayer: GetWorld() returned nullptr"));
        return;
    }

    // 캐릭터 검색
    for (TActorIterator<AOtherCharacter> It(World); It; ++It)
    {
        AOtherCharacter* OtherPlayer = *It;
        if (OtherPlayer && OtherPlayer->GetPlayerId() == PlayerId)
        {
            // 기존 캐릭터 위치 업데이트
            OtherPlayer->SetActorLocation(NewLocation);
            return; // 위치를 업데이트 했으므로 함수 종료
        }
    }

    // 기존 캐릭터를 찾지 못한 경우에만 새 캐릭터 스폰
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    AOtherCharacter* NewCharacter = World->SpawnActor<AOtherCharacter>(AOtherCharacter::StaticClass(), NewLocation, FRotator::ZeroRotator, SpawnParams);

    if (NewCharacter)
    {
        // 새 캐릭터에 PlayerId 설정
        NewCharacter->SetPlayerId(PlayerId);
    }
}