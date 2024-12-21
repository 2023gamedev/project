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
						FVector planeposition_center = Center;	//(PlaneVertexs[0] + PlaneVertexs[1] + PlaneVertexs[2] + PlaneVertexs[3]) / 4.0f;
			
						FVector planenormal = FVector::CrossProduct(PlaneVertexs[3] - PlaneVertexs[0], PlaneVertexs[1] - PlaneVertexs[2]).GetSafeNormal();

						Zombie->PlayerWeapon = this;
						Zombie->CutZombie(planeposition_center, planenormal, true);

						// 여기에서 클라 좀비 절단 패킷 send

						Protocol::slice_vector slicepacket;

						slicepacket.set_packet_type(24);
						slicepacket.set_zombieid(Zombie->GetZombieId());
						
						// Location 설정
						slicepacket.mutable_location()->set_x(Zombie->GetActorLocation().X);
						slicepacket.mutable_location()->set_y(Zombie->GetActorLocation().Y);
						slicepacket.mutable_location()->set_z(Zombie->GetActorLocation().Z);

						// Position 설정
						slicepacket.mutable_position()->set_x(planeposition_center.X);
						slicepacket.mutable_position()->set_y(planeposition_center.Y);
						slicepacket.mutable_position()->set_z(planeposition_center.Z);

						// Normal 설정
						slicepacket.mutable_normal()->set_x(planenormal.X);
						slicepacket.mutable_normal()->set_y(planenormal.Y);
						slicepacket.mutable_normal()->set_z(planenormal.Z);

						FVector WeaponForward = GetActorRotation().Vector();

						// Impulse 설정
						slicepacket.mutable_impulse()->set_x(WeaponForward.X);
						slicepacket.mutable_impulse()->set_y(WeaponForward.Y);
						slicepacket.mutable_impulse()->set_z(WeaponForward.Z);

						std::string serializedData;
						slicepacket.SerializeToString(&serializedData);

						bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());
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
