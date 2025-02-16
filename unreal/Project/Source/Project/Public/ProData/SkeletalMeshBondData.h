// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkeletalMeshBondData.generated.h"
/**
 * 
 */
class PROJECT_API SkeletalMeshBondData
{
public:
	SkeletalMeshBondData();
	~SkeletalMeshBondData();
};

USTRUCT(BlueprintType)
struct FVertexBoneData
{
    GENERATED_USTRUCT_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SectionIndex;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 VertexIndex;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName BoneName;

    FVertexBoneData() {}
    FVertexBoneData(int32 InSectionIndex, int32 InVertexIndex, FName InBoneName)
        : SectionIndex(InSectionIndex), VertexIndex(InVertexIndex), BoneName(InBoneName) {
    }
};

