// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZBoneStruct.generated.h"
/**
 * 
 */
class PROJECT_API ZBoneStruct
{
public:
	ZBoneStruct();
	~ZBoneStruct();
};

USTRUCT(BlueprintType)
struct FZBoneStructure
{
    //GENERATED_BODY()
    GENERATED_USTRUCT_BODY()

public:
    // 본 이름
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName Name;

    // 부모 본 (USTRUCT에서는 직접 포인터 사용 불가 -> TWeakPtr 사용)
    TWeakPtr<FZBoneStructure> Parent;

    // 자식 본 리스트 (TArray<TSharedPtr> 사용)
    TArray<TSharedPtr<FZBoneStructure>> Children;

    // 기본 생성자
    FZBoneStructure() {}

    // 초기화 생성자
    FZBoneStructure(const FName& InName)
        : Name(InName)
    {}

    bool IsChildOf(const TSharedPtr<FZBoneStructure>& PotentialParent) const
    {
        // 부모가 유효한 경우
        TSharedPtr<FZBoneStructure> ParentPtr = Parent.Pin(); // TWeakPtr을 TSharedPtr로 변환

        if (!ParentPtr.IsValid()) return false; // 부모가 없으면 false
        if (ParentPtr == PotentialParent) return true; // 바로 부모면 true

        if (ParentPtr == nullptr)
            return false;


        return ParentPtr->IsChildOf(PotentialParent); // 부모의 부모를 재귀적으로 확인
    }

};