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
	if (Zombie) {

		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Zombie Damaged!"));

		//if (OtherComp)
		//{
		//	FString HitResult;
		//	if (OtherComp->IsA(USkeletalMeshComponent::StaticClass()))
		//	{
		//		// 메쉬와 충돌
		//		HitResult = "Hit Skeletal Mesh";
		//	}
		//	else if (OtherComp->IsA(UCapsuleComponent::StaticClass()))
		//	{
		//		// 캡슐 컴포넌트와 충돌
		//		HitResult = "Hit Capsule Component";
		//	}
		//	else
		//	{
		//		// 다른 컴포넌트와 충돌
		//		HitResult = "Hit Other Component";
		//	}

		//	// GEngine을 사용하여 화면에 디버그 메시지 출력
		//	if (GEngine)
		//	{
		//		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, HitResult);
		//	}
		//}

		FDamageEvent DamageEvent;
		Zombie->TakeDamage(m_fCharacterSTR * m_fWeaponSTR, DamageEvent, GetInstigatorController(), this);
		

		// 좀비 hp 동기화
		int ZombieId = Zombie->GetZombieId();

		Protocol::Zombie_hp packet;
		packet.set_zombieid(ZombieId);
		packet.set_damage(m_fCharacterSTR * m_fWeaponSTR);
		packet.set_packet_type(12);

		std::string serializedData;
		packet.SerializeToString(&serializedData);

		bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());


		// 좀비 사망시
		if (Zombie->GetHP() <= 0) {
			Zombie->SetDie(true);

			if (WeaponName == "ButchersKnife" || WeaponName == "FireAxe" || WeaponName == "SashimiKnife") {

				TArray<FVector> PlaneVertexs;

				if (PlaneComponent) {

					for (int32 LODIndex = 0; LODIndex < PlaneComponent->GetStaticMesh()->GetRenderData()->LODResources.Num(); ++LODIndex)
					{
						for (uint32 Index = 0; Index < PlaneComponent->GetStaticMesh()->GetRenderData()->LODResources[LODIndex].
							VertexBuffers.PositionVertexBuffer.GetNumVertices(); ++Index)
						{
							// 로컬 좌표로 점 위치 얻기
							FVector3f EachVector = PlaneComponent->GetStaticMesh()->GetRenderData()->LODResources[LODIndex].
								VertexBuffers.PositionVertexBuffer.VertexPosition(Index);

							// 로컬 좌표를 월드 좌표로 변환
							FVector WorldPosition = PlaneComponent->GetComponentTransform().TransformPosition((FVector)EachVector);

							PlaneVertexs.Add((FVector)WorldPosition);

							//UE_LOG(LogClass, Log, TEXT("Plane - Index(%d) : (%s)"), Index, *WorldPosition.ToString());

						}
					}

					if (PlaneVertexs.Num() >= 4) {
						// 히트 지점에 평면의 선 그리기
						DrawDebugLine(
							GetWorld(),
							PlaneVertexs[0],			// 시작 위치
							PlaneVertexs[1],			// 히트 지점
							FColor::Red,				// 선 색상
							false,						// 지속 여부
							5.0f,						// 지속 시간
							0,							// 깊이 우선 여부
							1.0f						// 선 두께
						);

						DrawDebugLine(
							GetWorld(),
							PlaneVertexs[0],
							PlaneVertexs[2],
							FColor::Red,
							false,
							5.0f,
							0,
							1.0f
						);

						DrawDebugLine(
							GetWorld(),
							PlaneVertexs[2],
							PlaneVertexs[3],
							FColor::Red,
							false,
							5.0f,
							0,
							1.0f
						);

						DrawDebugLine(
							GetWorld(),
							PlaneVertexs[1],
							PlaneVertexs[3],
							FColor::Red,
							false,
							5.0f,
							0,
							1.0f
						);

						FVector planeposition_center = (PlaneVertexs[0] + PlaneVertexs[1] + PlaneVertexs[2] + PlaneVertexs[3]) / 4.0f;

						DrawDebugPoint(
							GetWorld(),
							planeposition_center,
							3.0f,
							FColor::Yellow,
							false,
							5.0f,
							0
						);

						FVector planenormal = FVector::CrossProduct(PlaneVertexs[0] - PlaneVertexs[3], PlaneVertexs[1] - PlaneVertexs[2]).GetSafeNormal();

						DrawDebugLine(
							GetWorld(),
							planeposition_center,
							planeposition_center + planenormal * 10.0f,
							FColor::Yellow,
							false,
							5.0f,
							0,
							1.0f
						);
						
						Zombie->WeaponForward = GetActorRotation().Vector();
						Zombie->CutZombie(planeposition_center, planenormal);
					}
				}

				Zombie->SetCuttingDeadWithAnim();
			}
			else {
				Zombie->SetNormalDeadWithAnim();
			}

		}

		BoxComponent->SetCollisionProfileName(TEXT("NoCollision"));

		// 무기 내구도 가감
		--m_fWeaponDurability;
		if (m_fWeaponDurability <= 0) {
			OwnerCharacter->DestroyNormalWepaonItemSlot();
		}

	}
}
