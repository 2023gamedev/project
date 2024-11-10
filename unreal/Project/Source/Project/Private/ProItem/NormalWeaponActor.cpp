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

	GameInstance = Cast<UProGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
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

		int ZombieId = Zombie->GetZombieId();

		Protocol::Zombie_hp packet;
		packet.set_zombieid(ZombieId);
		packet.set_damage(m_fCharacterSTR * m_fWeaponSTR);
		packet.set_packet_type(12);

		std::string serializedData;
		packet.SerializeToString(&serializedData);

		bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());

		if (Zombie->GetHP() <= 0) {
			Zombie->SetDie(true);
			if (WeaponName == "ButchersKnife" || WeaponName == "FireAxe" || WeaponName == "SashimiKnife") {

				USkeletalMeshComponent* Skeleton = Zombie->GetMesh();
				if (Skeleton) {
					if (OwnerCharacter)
					{
						FVector CameraLocation = OwnerCharacter->Camera->GetComponentLocation();
						FVector CameraUpVector = OwnerCharacter->Camera->GetUpVector();
						FVector ForwardVector = OwnerCharacter->GetActorForwardVector() * 1000;

						FCollisionQueryParams QueryParams;

						FCollisionResponseParams ResponseParams;

						FHitResult HitResult;
						bool bHit;
						bHit = GetWorld()->LineTraceSingleByChannel(
							HitResult,
							CameraLocation,
							ForwardVector,
							ECC_Visibility,
							QueryParams,
							ResponseParams
						);
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("CameraLocation: %s"), *CameraLocation.ToString()));
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("CameraUpVector: %s"), *CameraUpVector.ToString()));
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("ForwardVector: %s"), *ForwardVector.ToString()));

						if (bHit) {
							GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Hit detected!"));
							GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Hit Location: %s"), *HitResult.Location.ToString()));
							GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Hit Normal: %s"), *HitResult.Normal.ToString()));

							Zombie->CutZombie(HitResult.Location, HitResult.Normal);
						}
						else
						{
							GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("No hit detected."));
						}

						
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



