// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZombieMeshData.generated.h"

/**
 * 
 */
class PROJECT_API ZombieMeshData
{
public:
	ZombieMeshData();
	~ZombieMeshData();
};

USTRUCT(BlueprintType)
struct FZombieMeshData
{
    GENERATED_USTRUCT_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SectionIndex;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 VertexIndex;


    FZombieMeshData() {}
    FZombieMeshData(int32 InSectionIndex, int32 InVertexIndex)
        : SectionIndex(InSectionIndex), VertexIndex(InVertexIndex) {
    }

    bool operator==(const FZombieMeshData& Other) const
    {
        return SectionIndex == Other.SectionIndex && VertexIndex == Other.VertexIndex;
    }

};

FORCEINLINE uint32 GetTypeHash(const FZombieMeshData& Key)
{
    return HashCombine(GetTypeHash(Key.SectionIndex), GetTypeHash(Key.VertexIndex));
}