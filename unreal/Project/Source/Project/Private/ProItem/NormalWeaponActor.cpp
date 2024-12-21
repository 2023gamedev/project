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


		// 나 자신 플레이어만 검사
		if (OwnerCharacter) {
			if (OwnerCharacter->GetPlayerId() != 99) {
				UE_LOG(LogTemp, Log, TEXT("Not a local player hit the zombie. PlayerId = %d"), OwnerCharacter->GetPlayerId());
				return;
			}
		}


		FDamageEvent DamageEvent;
		Zombie->TakeDamage(m_fCharacterSTR * m_fWeaponSTR, DamageEvent, GetInstigatorController(), this);

		bool hp_packet_send = true;

		// 좀비 사망시
		if (Zombie->GetHP() <= 0) {
			Zombie->SetDie(true);

			if (WeaponName == "ButchersKnife" || WeaponName == "FireAxe" || WeaponName == "SashimiKnife") {
				hp_packet_send = false;

				TArray<FVector> PlaneVertexs;

				if (PlaneComponent) {

					//for (int32 LODIndex = 0; LODIndex < PlaneComponent->GetStaticMesh()->GetRenderData()->LODResources.Num(); ++LODIndex)
					//{
					//	for (uint32 Index = 0; Index < PlaneComponent->GetStaticMesh()->GetRenderData()->LODResources[LODIndex].
					//		VertexBuffers.PositionVertexBuffer.GetNumVertices(); ++Index)
					//	{
					//		// 로컬 좌표로 점 위치 얻기
					//		FVector3f EachVector = PlaneComponent->GetStaticMesh()->GetRenderData()->LODResources[LODIndex].
					//			VertexBuffers.PositionVertexBuffer.VertexPosition(Index);

					//		// 로컬 좌표를 월드 좌표로 변환
					//		FVector WorldPosition = PlaneComponent->GetComponentTransform().TransformPosition((FVector)EachVector);

					//		PlaneVertexs.Add((FVector)WorldPosition);

					//		//UE_LOG(LogClass, Log, TEXT("Plane - Index(%d) : (%s)"), Index, *WorldPosition.ToString());

					//	}
					//}

					FVector Center = PlaneComponent->GetComponentLocation();
					FVector Right = PlaneComponent->GetRightVector();  // 평면의 오른쪽 방향
					FVector Forward = PlaneComponent->GetForwardVector();  // 평면의 앞쪽 방향
					FVector Scale = PlaneComponent->GetComponentScale();  // 평면의 스케일

					float Weapon_Scale = 0.f;	// 무기별 스케일 조정
					if (WeaponName == "ButchersKnife") { Weapon_Scale = 45.f; }
					else if (WeaponName == "FireAxe") { Weapon_Scale = 50.f; }
					else if (WeaponName == "SashimiKnife") { Weapon_Scale = 40.f; }

					float HalfWidth = Weapon_Scale * Scale.X;  // 평면의 폭
					float HalfHeight = Weapon_Scale * Scale.Y; // 평면의 높이

					// 꼭짓점 계산
					FVector TopLeft = Center - Right * HalfWidth + Forward * HalfHeight;
					FVector TopRight = Center + Right * HalfWidth + Forward * HalfHeight;
					FVector BottomLeft = Center - Right * HalfWidth - Forward * HalfHeight;
					FVector BottomRight = Center + Right * HalfWidth - Forward * HalfHeight;

					// 꼭짓점을 리스트에 추가
					PlaneVertexs = { TopLeft, TopRight, BottomLeft, BottomRight };

					if (PlaneVertexs.Num() >= 4) {
						float displaceTime = 30.f;

						// 히트 지점에 평면의 선 그리기
						DrawDebugLine(
							GetWorld(),
							PlaneVertexs[0],			// 시작 위치
							PlaneVertexs[1],			// 히트 지점
							FColor::Green,				// 선 색상
							false,						// 지속 여부
							displaceTime,				// 지속 시간
							0,							// 깊이 우선 여부
							1.0f						// 선 두께
						);

						DrawDebugLine(
							GetWorld(),
							PlaneVertexs[0],
							PlaneVertexs[2],
							FColor::Green,
							false,
							displaceTime,
							0,
							1.0f
						);

						DrawDebugLine(
							GetWorld(),
							PlaneVertexs[2],
							PlaneVertexs[3],
							FColor::Green,
							false,
							displaceTime,
							0,
							1.0f
						);

						DrawDebugLine(
							GetWorld(),
							PlaneVertexs[1],
							PlaneVertexs[3],
							FColor::Green,
							false,
							displaceTime,
							0,
							1.0f
						);

						FVector planeposition_center = Center;	//(PlaneVertexs[0] + PlaneVertexs[1] + PlaneVertexs[2] + PlaneVertexs[3]) / 4.0f;

						DrawDebugPoint(
							GetWorld(),
							planeposition_center,
							10.0f,
							FColor::Yellow,
							false,
							displaceTime,
							0
						);

						FVector planenormal = FVector::CrossProduct(PlaneVertexs[3] - PlaneVertexs[0], PlaneVertexs[1] - PlaneVertexs[2]).GetSafeNormal();

						DrawDebugLine(
							GetWorld(),
							planeposition_center,
							planeposition_center + planenormal * 20.0f,
							FColor::Yellow,
							false,
							displaceTime,
							0,
							1.0f
						);

						Zombie->WeaponForward = GetActorRotation().Vector();		// 잉 이건 뭐에 필요한 거지..?
						Zombie->CutZombie(planeposition_center, planenormal);

						// 여기에서 클라 좀비 절단 패킷 send
						//
					}
				}

				Zombie->SetCuttingDeadWithAnim();
			}
			else {
				Zombie->SetNormalDeadWithAnim();
			}

		}

		if (hp_packet_send == true) {
			// 좀비 hp 동기화
			int ZombieId = Zombie->GetZombieId();

			Protocol::Zombie_hp packet;
			packet.set_zombieid(ZombieId);
			packet.set_damage(m_fCharacterSTR * m_fWeaponSTR);
			packet.set_packet_type(12);

			std::string serializedData;
			packet.SerializeToString(&serializedData);

			bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());
		}

		BoxComponent->SetCollisionProfileName(TEXT("NoCollision"));

		// 무기 내구도 가감
		--m_fWeaponDurability;
		if (m_fWeaponDurability <= 0) {
			OwnerCharacter->DestroyNormalWepaonItemSlot();
			USoundBase* Sound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Sound/break.break")); // 에셋 경로

			OwnerCharacter->PlaySoundForPlayer(Sound);
		}

	}
}
