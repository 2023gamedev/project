// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NormalWeaponActor.h"
#include "ProZombie/BaseZombie.h"
#include "Engine/DamageEvents.h"
#include "Components/SkeletalMeshComponent.h"
#include "ProCharacter/BaseCharacter.h"

ANormalWeaponActor::ANormalWeaponActor()
{
	//PrimaryActorTick.bCanEverTick = false;
}

void ANormalWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	if (BoxComponent) {
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ANormalWeaponActor::WeaponBeginOverlap);
	}
}

void ANormalWeaponActor::Tick(float DeltaTime)
{

}

void ANormalWeaponActor::PlaceItem()
{
}

// 플레이어 무기로 좀비 후두릴때 충돌체크 (좀비 피격 충돌체크)
void ANormalWeaponActor::WeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseZombie* Zombie = Cast<ABaseZombie>(OtherActor);
	if(Zombie){


		if(OtherComp)
		{
			FString HitResult;
			if (OtherComp->IsA(USkeletalMeshComponent::StaticClass()))
			{
				// 메쉬와 충돌
				HitResult = "Hit Skeletal Mesh";
			}
			else if (OtherComp->IsA(UCapsuleComponent::StaticClass()))
			{
				// 캡슐 컴포넌트와 충돌
				HitResult = "Hit Capsule Component";
			}
			else
			{
				// 다른 컴포넌트와 충돌
				HitResult = "Hit Other Component";
			}

			// GEngine을 사용하여 화면에 디버그 메시지 출력
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, HitResult);
			}
		}
		FDamageEvent DamageEvent;
		Zombie->TakeDamage(m_fCharacterSTR * m_fWeaponSTR, DamageEvent, GetInstigatorController(), this);
		if (Zombie->GetHP() <= 0) {
			Zombie->SetDie(true);
			if (WeaponName == "ButchersKnife" || WeaponName == "FireAxe" || WeaponName == "SashimiKnife") {

				USkeletalMeshComponent* Skeleton = Zombie->GetMesh();
				if (Skeleton)
				{

					GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("ClosetBone!"));
					FName ClosestBoneName;
					float ClosestBoneDistance = TNumericLimits<float>::Max();

					GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Impact Point: %f, %f, %f"), GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z));
					TArray<FName> BoneNames;
					Skeleton->GetBoneNames(BoneNames);
					for (const FName& BoneName : BoneNames)
					{
						FVector BoneLocation = Skeleton->GetBoneLocation(BoneName);
						FVector BoneLocationIgnoreZ = FVector(BoneLocation.X, 0, BoneLocation.Z);

						float DistanceSquared = FVector::DistSquared(BoneLocation, GetActorLocation());
						if (DistanceSquared < ClosestBoneDistance)
						{
							ClosestBoneDistance = DistanceSquared;
							ClosestBoneName = BoneName;
						}
					}
					int32 BoneIndex = Skeleton->GetBoneIndex(ClosestBoneName);
					if (BoneIndex != INDEX_NONE)
					{

						GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("ClosestBone: %s, BoneIndex: %d"), *ClosestBoneName.ToString(), BoneIndex));
						Zombie->CutZombie(ClosestBoneName);
					}
				}
				Zombie->SetCuttingDeadWithAnim();
			}
			else {
				Zombie->SetNormalDeadWithAnim();
			}
		}
		else {

			Zombie->StartAttackedStunHandle();

		}
		BoxComponent->SetCollisionProfileName(TEXT("NoCollision"));
		--m_fWeaponDurability;
		if (m_fWeaponDurability <= 0) {
			OwnerCharacter->DestroyNormalWepaonItemSlot();
		}
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Damaged!"));
	}
}



