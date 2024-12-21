// Fill out your copyright notice in the Description page of Project Settings.


#include "ProZombie/BaseZombie.h"

#include "Engine/DamageEvents.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ProZombie/ZombieAIController.h"
#include "ProZombie/RunningZombieAIController.h"
#include "ProZombie/ShoutingZombieAIController.h"
#include "Engine/SkeletalMesh.h"
#include "ProNiagaFX/BloodNiagaEffect.h"
#include "ProNiagaFX/ShoutingNiagaEffect.h"
//#include "ProItem/NormalWeaponActor.h"

#include "Rendering/SkeletalMeshRenderData.h"
#include "Rendering/SkeletalMeshLODRenderData.h"
#include "Rendering/SkeletalMeshLODModel.h"
#include "KismetProceduralMeshLibrary.h"
#include "Materials/MaterialInterface.h"
#include "Kismet/GameplayStatics.h"
#include "ProceduralMeshComponent.h"

#include "PhysicsEngine/BodyInstance.h"
#include "PhysicsEngine/ConvexElem.h"
#include "PhysicsEngine/PhysicsAsset.h"


// Sets default values
ABaseZombie::ABaseZombie()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.f, 0.0f);
	GetCapsuleComponent()->SetCollisionProfileName("ZombieCol");
	GetMesh()->SetCollisionProfileName("Zombie");
	GetMesh()->SetGenerateOverlapEvents(true);

	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceConsiderationRadius = 400.f;

	ShoutingFX = CreateDefaultSubobject<AShoutingNiagaEffect>(TEXT("ShoutingFX"));

	ShoutingFX = nullptr;
	
	ConstructorHelpers::FObjectFinder<UMaterial> MaterialFinder(TEXT("/Game/Mesh/SlicedBloodMaterial.SlicedBloodMaterial"));
	if (MaterialFinder.Succeeded())
	{
		Material_Blood = MaterialFinder.Object;
	}
}

// Called when the game starts or when spawned
void ABaseZombie::BeginPlay()
{
	Super::BeginPlay();

	CachedAnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());

	GameInstance = Cast<UProGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ABaseZombie::OnZombieHit);
}

// Called every frame
void ABaseZombie::Tick(float DeltaTime)
{
	// CutProceduralMesh Impulse 적용
	if (CutProceduralMesh_1) {
		FVector WeaponForward;

		if (procMesh_AddImpulse_1 == false && PlayerWeapon) {
			WeaponForward = PlayerWeapon->GetActorRotation().Vector();

			//UE_LOG(LogTemp, Log, TEXT("(CutProcedural_1)"));

			float weight = CutProceduralMesh_1->CalculateMass();
			//UE_LOG(LogTemp, Log, TEXT("Weight: %f"), weight);

			float x_baseImpulse = 3500.0f; //FMath::RandRange(2000.0f, 5000.0f);
			float x_impulse = SetImpulseByWeight(weight, x_baseImpulse);
			//UE_LOG(LogTemp, Log, TEXT("X Impulse: %f"), x_impulse);

			float y_baseImpulse = 3500.0f; //FMath::RandRange(2000.0f, 5000.0f);
			float y_impulse = SetImpulseByWeight(weight, y_baseImpulse);
			//UE_LOG(LogTemp, Log, TEXT("Y Impulse: %f"), y_impulse);

			float z_baseImpulse = 12500.0f; //FMath::RandRange(7000.0f, 10000.0f);
			float z_impulse = SetImpulseByWeight(weight, z_baseImpulse);
			//UE_LOG(LogTemp, Log, TEXT("Z Impulse: %f"), z_impulse);

			if (doAction_setIsCuttingDead_onTick == false) {
				UE_LOG(LogTemp, Log, TEXT("Weapon Foward Vector: %s"), *WeaponForward.ToString());

				CutProceduralMesh_1->AddImpulseAtLocation(FVector(WeaponForward.X * x_impulse, WeaponForward.Y * y_impulse, WeaponForward.Z * z_impulse), CutProceduralMesh_1->K2_GetComponentLocation());
				procMesh_AddImpulse_1 = true;
			}
			else {
				UE_LOG(LogTemp, Log, TEXT("Sync Impulse Direction Vector: %s"), *sync_cutImpulse.ToString());

				CutProceduralMesh_1->AddImpulseAtLocation(FVector(sync_cutImpulse.X * x_impulse, sync_cutImpulse.Y * y_impulse, sync_cutImpulse.Z * z_impulse), CutProceduralMesh_1->K2_GetComponentLocation());
				procMesh_AddImpulse_1 = true;
			}
		}


		/*if (CutProceduralMesh_1->GetComponentVelocity().X != 0 || CutProceduralMesh_1->GetComponentVelocity().Y != 0 || CutProceduralMesh_1->GetComponentVelocity().Z != 0) {
			UE_LOG(LogTemp, Log, TEXT("GetVelocity - CutProcedural_1 ( %f , %f , %f )"), CutProceduralMesh_1->GetComponentVelocity().X, CutProceduralMesh_1->GetComponentVelocity().Y, CutProceduralMesh_1->GetComponentVelocity().Z);
			if (print_Velocity_1 == false)
				print_Velocity_1 = true;
		}
		else {
			if (print_Velocity_1)
				UE_LOG(LogTemp, Log, TEXT("GetVelocity - CutProcedural_1 ( 0.000000 , 0.000000 , 0.000000 )"));

			if (procMesh_AddImpulse_1)
				print_Velocity_1 = false;
		}*/
	}

	if (CutProceduralMesh_2) {
		FVector WeaponForward;

		if (procMesh_AddImpulse_2 == false && PlayerWeapon) {
			WeaponForward = PlayerWeapon->GetActorRotation().Vector();

			//UE_LOG(LogTemp, Log, TEXT("(CutProcedural_2)"));

			float weight = CutProceduralMesh_2->CalculateMass();
			//UE_LOG(LogTemp, Log, TEXT("Weight: %f"), weight);

			float x_baseImpulse = 10000.0f; //FMath::RandRange(8000.0f, 12000.0f);
			float x_impulse = SetImpulseByWeight(weight, x_baseImpulse);
			//UE_LOG(LogTemp, Log, TEXT("X Impulse: %f"), x_impulse);

			float y_baseImpulse = 10000.0f; //FMath::RandRange(8000.0f, 12000.0f);
			float y_impulse = SetImpulseByWeight(weight, y_baseImpulse);
			//UE_LOG(LogTemp, Log, TEXT("Y Impulse: %f"), y_impulse);

			float z_baseImpulse = 3000.0f; //FMath::RandRange(2000.0f, 4000.0f);
			float z_impulse = SetImpulseByWeight(weight, z_baseImpulse);
			//UE_LOG(LogTemp, Log, TEXT("Z Impulse: %f"), z_impulse);

			if (doAction_setIsCuttingDead_onTick == false) {
				UE_LOG(LogTemp, Log, TEXT("Weapon Backward Vector: %s"), *(-WeaponForward).ToString());

				CutProceduralMesh_2->AddImpulseAtLocation(FVector(-WeaponForward.X * x_impulse, -WeaponForward.Y * y_impulse, z_impulse), CutProceduralMesh_2->K2_GetComponentLocation());
				procMesh_AddImpulse_2 = true;
			}
			else {
				UE_LOG(LogTemp, Log, TEXT("Sync Impulse Backward Direction Vector : % s"), *(-sync_cutImpulse).ToString());

				CutProceduralMesh_2->AddImpulseAtLocation(FVector(-sync_cutImpulse.X * x_impulse, -sync_cutImpulse.Y * y_impulse, z_impulse), CutProceduralMesh_2->K2_GetComponentLocation());
				procMesh_AddImpulse_2 = true;
			}
		}

		/*if (CutProceduralMesh_2->GetComponentVelocity().X != 0 || CutProceduralMesh_2->GetComponentVelocity().Y != 0 || CutProceduralMesh_2->GetComponentVelocity().Z != 0) {
			UE_LOG(LogTemp, Log, TEXT("GetVelocity - CutProcedural_2 ( %f , %f , %f )"), CutProceduralMesh_2->GetComponentVelocity().X, CutProceduralMesh_2->GetComponentVelocity().Y, CutProceduralMesh_2->GetComponentVelocity().Z);
			if (print_Velocity_2 == false)
				print_Velocity_2 = true;
		}
		else {
			if (print_Velocity_2)
				UE_LOG(LogTemp, Log, TEXT("GetVelocity - CutProcedural_2 ( 0.000000 , 0.000000 , 0.000000 )"));

			if (procMesh_AddImpulse_2)
				print_Velocity_2 = false;
		}*/
	}


	// 클라 - 보이지 않는 좀비 최적화 작업
	if (MyChar == nullptr)
		return;

	if (MyChar->floor != floor) {
		SetActorHiddenInGame(true);	// 렌더링 중지
		UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));
		if (MeshComponent)
		{
			MeshComponent->SetSimulatePhysics(false); // 물리 시뮬레이션 중지
		}
		return;
		//SetActorTickEnabled(false);	// tick 연산 중지
	}
	else {
		SetActorHiddenInGame(false);	// 렌더링 다시 시작
		UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));
		if (MeshComponent)
		{
			MeshComponent->SetSimulatePhysics(true); // 물리 시뮬레이션 다시 시작
		}
		//SetActorTickEnabled(true);	// tick 연산 다시 시작
	}


	float bloodspawn_z_offset = 80.f;

	// 좀비 피격시 클라 동기화
	if (m_fHP_Prev != m_fHP && GetHP() > 0 && m_bBeAttacked == false && doAction_takeDamage_onTick == true) {

		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Purple, FString::Printf(TEXT("좀비 피격 클라 동기화 작업실행!")));	// 직접 때리는 클라에서는 해당 메세지 보이면 안 됨1

		BeAttacked();
		m_fHP_Prev = m_fHP;

		BloodFX.Empty();

		FRotator RandRotate;

		RandRotate.Yaw = FMath::FRandRange(0.f, 1.f);
		RandRotate.Roll = FMath::FRandRange(0.f, 1.f);
		RandRotate.Pitch = FMath::FRandRange(0.f, 1.f);

		ABloodNiagaEffect* NewBloodFX = GetWorld()->SpawnActor<ABloodNiagaEffect>(ABloodNiagaEffect::StaticClass(), GetActorLocation() + FVector(0, 0, bloodspawn_z_offset), RandRotate);

		if (NewBloodFX) {
			NewBloodFX->blood_spawncount = FMath::RandRange(80, 100);
			NewBloodFX->spawn_flag = true;

			BloodFX.Add(NewBloodFX);
		}
	}

	// 좀비 사망처리 클라 동기화 - 애니메이션 재생, 피 이펙트 생성

	// normal dead 동기화
	if (GetHP() <= 0 && m_bIsNormalDead == false && doAction_setIsNormalDead_onTick == true) {

		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Purple, FString::Printf(TEXT("좀비 사망 클라 동기화 작업실행!")));
		//UE_LOG(LogTemp, Log, TEXT("좀비 사망 클라 동기화 작업실행!"));

		m_bIsNormalDead = true;
		auto CharacterAnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());
		if (nullptr != CharacterAnimInstance) {
			CharacterAnimInstance->SetIsNormalDead(m_bIsNormalDead);
		}
		GetCapsuleComponent()->SetCollisionProfileName("NoCollision");


		BloodFX.Empty();

		FRotator RandRotate;

		RandRotate.Yaw = FMath::FRandRange(0.f, 1.f);
		RandRotate.Roll = FMath::FRandRange(0.f, 1.f);
		RandRotate.Pitch = FMath::FRandRange(0.f, 1.f);

		ABloodNiagaEffect* NewBloodFX = GetWorld()->SpawnActor<ABloodNiagaEffect>(ABloodNiagaEffect::StaticClass(), GetActorLocation() + FVector(0, 0, bloodspawn_z_offset), RandRotate);

		if (NewBloodFX) {
			NewBloodFX->blood_spawncount = FMath::RandRange(450, 600);
			//NewBloodFX->blood_spawnloop = true;
			NewBloodFX->spawn_flag = true;

			BloodFX.Add(NewBloodFX);
		}
	}

	// cut dead 동기화
	if (GetHP() <= 0 && m_bIsCuttingDead == false && doAction_setIsCuttingDead_onTick == true) {

		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Purple, FString::Printf(TEXT("좀비 절단 사망 클라 동기화 작업실행!")));
		//UE_LOG(LogTemp, Log, TEXT("좀비 절단 사망 클라 동기화 작업실행!"));

		m_bIsCuttingDead = true;
		auto CharacterAnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());
		if (nullptr != CharacterAnimInstance) {
			CharacterAnimInstance->SetIsCuttingDead(m_bIsCuttingDead);
		}
		GetCapsuleComponent()->SetCollisionProfileName("NoCollision");


		CutZombie(sync_cutPlane, sync_cutNormal, false);


		BloodFX.Empty();

		FRotator RandRotate;

		RandRotate.Yaw = FMath::FRandRange(0.f, 1.f);
		RandRotate.Roll = FMath::FRandRange(0.f, 1.f);
		RandRotate.Pitch = FMath::FRandRange(0.f, 1.f);

		ABloodNiagaEffect* NewBloodFX = GetWorld()->SpawnActor<ABloodNiagaEffect>(ABloodNiagaEffect::StaticClass(), GetActorLocation() + FVector(0, 0, bloodspawn_z_offset), RandRotate);

		if (NewBloodFX) {
			NewBloodFX->blood_spawncount = FMath::RandRange(450, 600);
			//NewBloodFX->blood_spawnloop = true;
			NewBloodFX->spawn_flag = true;

			BloodFX.Add(NewBloodFX);
		}
	}


	Super::Tick(DeltaTime);

}

void ABaseZombie::OnZombieHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != nullptr && OtherActor != this) {
		//UE_LOG(LogTemp, Display, TEXT("Zombie collided with %s"), *OtherActor->GetName());
	}

	Protocol::patrol_hit Packet;
	Packet.set_zombieid(ZombieId);
	Packet.set_packet_type(14);

	std::string serializedData;
	Packet.SerializeToString(&serializedData);

	bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());

}

void ABaseZombie::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 변경 필요 각각의 animinstance가 필요할 것 같다.
	auto AnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());

	AnimInstance->OnMontageEnded.AddDynamic(this, &ABaseZombie::AttackMontageEnded);
	AnimInstance->OnMontageEnded.AddDynamic(this, &ABaseZombie::ShoutingMontageEnded);
	AnimInstance->OnMontageEnded.AddDynamic(this, &ABaseZombie::BeAttackedMontageEnded);

	AnimInstance->OnAttackStartCheck.AddLambda([this]() -> void {
		AttackCheck();
		});

}

void ABaseZombie::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

// 100kg 기준 baseImpulse 만큼의 Impulse 적용
float ABaseZombie::SetImpulseByWeight(float targetWeight, float baseImpulse)
{
	float baseWeight = 100.0f;

	float targetImpulse = baseImpulse * (targetWeight / baseWeight);

	return targetImpulse;
}

// 좀비가 공격을 받았을 때
float ABaseZombie::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("HP %f"), GetHP()));

	doAction_takeDamage_onTick = false;
	doAction_setIsNormalDead_onTick = false;
	doAction_setIsCuttingDead_onTick = false;

	SetHP(GetHP() - Damage);
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("HP %f"), GetHP()));

	m_fHP_Prev = m_fHP;

	ANormalWeaponActor* Weapon = Cast<ANormalWeaponActor>(DamageCauser);

	if (Weapon == nullptr) {
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("[ERROR] TakeDamage - Weapon is nullptr!!! No Damage!")));
		UE_LOG(LogTemp, Error, TEXT("TakeDamage - Weapon is nullptr!!! No Damage!"));
		return 0;
	}

	//UE_LOG(LogTemp, Log, TEXT("Weapon Location: %s"), *Weapon->GetActorLocation().ToString());
	//UE_LOG(LogTemp, Log, TEXT("Weapon Rotation: %s"), *Weapon->GetActorRotation().ToString());
	//UE_LOG(LogTemp, Log, TEXT("Weapon Rotation Vector: %s"), *Weapon->GetActorRotation().Vector().ToString());

	BloodFX.Empty();

	if (GetHP() <= 0) {	// 죽을때(절단 될 때)는 피가 더 많이 튀도록 & 절단 된 부분들에서 따로 피 생성되도록
		if (Weapon->WeaponName == "ButchersKnife" || Weapon->WeaponName == "FireAxe" || Weapon->WeaponName == "SashimiKnife") {	// 날붙이 무기
			ABloodNiagaEffect* NewBloodFX_0 = GetWorld()->SpawnActor<ABloodNiagaEffect>(ABloodNiagaEffect::StaticClass(), Weapon->GetActorLocation(), Weapon->GetActorRotation()); // 무기가 닿은 위치에서 무기가 바라보는 방향으로 피 이펙트 생성

			if (NewBloodFX_0) {
				NewBloodFX_0->blood_spawncount = FMath::RandRange(300, 400);
				//NewBloodFX_0->blood_spawnloop = true;

				BloodFX.Add(NewBloodFX_0);
			}

			ABloodNiagaEffect* NewBloodFX_1 = GetWorld()->SpawnActor<ABloodNiagaEffect>(ABloodNiagaEffect::StaticClass(), Weapon->GetActorLocation(), Weapon->GetActorRotation()); // 무기가 닿은 위치에서 무기가 바라보는 방향으로 피 이펙트 생성

			if (NewBloodFX_1) {
				NewBloodFX_1->blood_spawncount = FMath::RandRange(300, 400);
				//NewBloodFX_1->blood_spawnloop = true;

				BloodFX.Add(NewBloodFX_1);
			}
		}
		else {	// 그 외 타격무기
			ABloodNiagaEffect* NewBloodFX = GetWorld()->SpawnActor<ABloodNiagaEffect>(ABloodNiagaEffect::StaticClass(), Weapon->GetActorLocation(), Weapon->GetActorRotation()); // 무기가 닿은 위치에서 무기가 바라보는 방향으로 피 이펙트 생성

			if (NewBloodFX) {
				NewBloodFX->blood_spawncount = FMath::RandRange(450, 600);
				//NewBloodFX->blood_spawnloop = true;
				NewBloodFX->spawn_flag = true;

				BloodFX.Add(NewBloodFX);
			}
		}
	}
	else {
		ABloodNiagaEffect* NewBloodFX = GetWorld()->SpawnActor<ABloodNiagaEffect>(ABloodNiagaEffect::StaticClass(), Weapon->GetActorLocation(), Weapon->GetActorRotation()); // 무기가 닿은 위치에서 무기가 바라보는 방향으로 피 이펙트 생성
		
		if (NewBloodFX) {
			NewBloodFX->blood_spawncount = FMath::RandRange(80, 100);
			NewBloodFX->spawn_flag = true;

			BloodFX.Add(NewBloodFX);
		}
	}

	BeAttacked();

	return Damage;
}

void ABaseZombie::SetNormalDeadWithAnim()
{ 
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, FString::Printf(TEXT("좀비 사망 직접 실행! - normal dead")));
	//UE_LOG(LogTemp, Log, TEXT("좀비 사망 직접 실행! - normal dead"));

	m_bIsNormalDead = true;
	auto CharacterAnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr != CharacterAnimInstance) {
		CharacterAnimInstance->SetIsNormalDead(m_bIsNormalDead);
	}
	GetCapsuleComponent()->SetCollisionProfileName("NoCollision");

	//StartResurrectionTimer();

}

void ABaseZombie::CutZombie(FVector planeposition, FVector planenormal, bool do_self)
{
	// 절단 디버깅 용 - 화면 출력
	//========================================

	//FVector Center;
	//FVector Right;  // 평면의 오른쪽 방향
	//FVector Forward;  // 평면의 앞쪽 방향
	//FVector Scale;  // 평면의 스케일
	//float Weapon_Scale = 0.f;	// 무기별 스케일 조정

	//if (do_self == true && PlayerWeapon) {
	//	Center = PlayerWeapon->PlaneComponent->GetComponentLocation();
	//	Right = PlayerWeapon->PlaneComponent->GetRightVector();  // 평면의 오른쪽 방향
	//	Forward = PlayerWeapon->PlaneComponent->GetForwardVector();  // 평면의 앞쪽 방향
	//	Scale = PlayerWeapon->PlaneComponent->GetComponentScale();  // 평면의 스케일


	//	if (PlayerWeapon->WeaponName == "ButchersKnife") { Weapon_Scale = 45.f; }
	//	else if (PlayerWeapon->WeaponName == "FireAxe") { Weapon_Scale = 50.f; }
	//	else if (PlayerWeapon->WeaponName == "SashimiKnife") { Weapon_Scale = 40.f; }

	//	float HalfWidth = Weapon_Scale * Scale.X;  // 평면의 폭
	//	float HalfHeight = Weapon_Scale * Scale.Y; // 평면의 높이

	//	// 꼭짓점 계산
	//	FVector TopLeft = Center - Right * HalfWidth + Forward * HalfHeight;
	//	FVector TopRight = Center + Right * HalfWidth + Forward * HalfHeight;
	//	FVector BottomLeft = Center - Right * HalfWidth - Forward * HalfHeight;
	//	FVector BottomRight = Center + Right * HalfWidth - Forward * HalfHeight;

	//	// 꼭짓점을 리스트에 추가
	//	TArray PlaneVertexs = { TopLeft, TopRight, BottomLeft, BottomRight };

	//	if (PlaneVertexs.Num() >= 4) {
	//		float displaceTime = 30.f;

	//		// 히트 지점에 평면의 선 그리기
	//		DrawDebugLine(
	//			GetWorld(),
	//			PlaneVertexs[0],			// 시작 위치
	//			PlaneVertexs[1],			// 히트 지점
	//			FColor::Green,				// 선 색상
	//			false,						// 지속 여부
	//			displaceTime,				// 지속 시간
	//			0,							// 깊이 우선 여부
	//			1.0f						// 선 두께
	//		);

	//		DrawDebugLine(
	//			GetWorld(),
	//			PlaneVertexs[0],
	//			PlaneVertexs[2],
	//			FColor::Green,
	//			false,
	//			displaceTime,
	//			0,
	//			1.0f
	//		);

	//		DrawDebugLine(
	//			GetWorld(),
	//			PlaneVertexs[2],
	//			PlaneVertexs[3],
	//			FColor::Green,
	//			false,
	//			displaceTime,
	//			0,
	//			1.0f
	//		);

	//		DrawDebugLine(
	//			GetWorld(),
	//			PlaneVertexs[1],
	//			PlaneVertexs[3],
	//			FColor::Green,
	//			false,
	//			displaceTime,
	//			0,
	//			1.0f
	//		);

	//		FVector planeposition_center = Center;	//(PlaneVertexs[0] + PlaneVertexs[1] + PlaneVertexs[2] + PlaneVertexs[3]) / 4.0f;

	//		DrawDebugPoint(
	//			GetWorld(),
	//			planeposition_center,
	//			5.0f,
	//			FColor::Yellow,
	//			false,
	//			displaceTime,
	//			0
	//		);

	//		FVector planenormal = FVector::CrossProduct(PlaneVertexs[3] - PlaneVertexs[0], PlaneVertexs[1] - PlaneVertexs[2]).GetSafeNormal();

	//		DrawDebugLine(
	//			GetWorld(),
	//			planeposition_center,
	//			planeposition_center + planenormal * 20.0f,
	//			FColor::Yellow,
	//			false,
	//			displaceTime,
	//			0,
	//			1.0f
	//		);
	//	}
	//}
	//========================================

	USkeletalMeshComponent* Skeleton = GetMesh();
	if (Skeleton) {

		// ProceduralMeshComponent 생성 및 설정
		CutProceduralMesh_1 = NewObject<UProceduralMeshComponent>(this);

		if (!CutProceduralMesh_1) return;

		CreativeProceduralMesh(planeposition, planenormal);

	}
}

// 프로시저럴 메쉬 생성되는 부분
void ABaseZombie::CreativeProceduralMesh(FVector planeposition, FVector planenormal)
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("CreativeProceduralMesh")));

	USkeletalMeshComponent* Skeleton = GetMesh();

	FSkeletalMeshRenderData* SkMeshRenderData = Skeleton->GetSkeletalMeshRenderData();
	const FSkeletalMeshLODRenderData& DataArray = SkMeshRenderData->LODRenderData[0];
	FSkinWeightVertexBuffer& SkinWeights = *Skeleton->GetSkinWeightBuffer(0);

	TArray<FVector> Vertices;
	TArray<FVector> Normals;
	TArray<FVector2D> UV;
	TArray<FColor> Colors;
	TArray<FProcMeshTangent> Tangents;

	TArray<FVector> Vertices_Convex;
	int interval_Convex = 100;			// Convex 에서 사용할 vertices 개수 조절 (원래 vertices의 1/? 개)

	//UE_LOG(LogTemp, Log, TEXT("RenderSections.Num (int) %d"), DataArray.RenderSections.Num());

	for (int32 j = 0; j < DataArray.RenderSections.Num(); j++)
	{
		const int32 NumSourceVertices = DataArray.RenderSections[j].NumVertices;
		const int32 BaseVertexIndex = DataArray.RenderSections[j].BaseVertexIndex;

		//UE_LOG(LogTemp, Log, TEXT("RenderSections[%d] - NumSourceVertices.Num (int) %d"), j, DataArray.RenderSections[j].NumVertices);

		for (int32 i = 0; i < NumSourceVertices; i++)
		{
			const int32 VertexIndex = i + BaseVertexIndex;
			const FVector3f SkinnedVectorPos = USkeletalMeshComponent::GetSkinnedVertexPosition(Skeleton, VertexIndex, DataArray, SkinWeights);

			Vertices.Add(FVector(SkinnedVectorPos.X, SkinnedVectorPos.Y, SkinnedVectorPos.Z));

			if (i % interval_Convex == 0) {
				Vertices_Convex.Add(FVector(SkinnedVectorPos.X, SkinnedVectorPos.Y, SkinnedVectorPos.Z));
			}

			const FVector3f ZTangentStatic = DataArray.StaticVertexBuffers.StaticMeshVertexBuffer.VertexTangentZ(VertexIndex);
			const FVector3f XTangentStatic = DataArray.StaticVertexBuffers.StaticMeshVertexBuffer.VertexTangentX(VertexIndex);

			Normals.Add(FVector(ZTangentStatic.X, ZTangentStatic.Y, ZTangentStatic.Z));
			Tangents.Add(FProcMeshTangent(FVector(XTangentStatic.X, XTangentStatic.Y, XTangentStatic.Z), false));

			const FVector2f SourceUVs = DataArray.StaticVertexBuffers.StaticMeshVertexBuffer.GetVertexUV(VertexIndex, 0);
			FVector2d ResUVs;

			ResUVs.X = SourceUVs.X;
			ResUVs.Y = SourceUVs.Y;

			UV.Add(ResUVs);

			Colors.Add(FColor(0.0, 0.0, 0.0, 255));
		}
	}

	// ProceduralMesh에 Convex Collision 추가하기 위해서
	CutProceduralMesh_1->bUseComplexAsSimpleCollision = false;

	FMultiSizeIndexContainerData IndicesData;
	DataArray.MultiSizeIndexContainer.GetIndexBuffer(IndicesData.Indices);

	for (int32 j = 0; j < DataArray.RenderSections.Num(); j++)
	{
		TArray<int32> Triangles;

		const int32 SectionNumTriangles = DataArray.RenderSections[j].NumTriangles;
		const int32 SectionBaseIndex = DataArray.RenderSections[j].BaseIndex;

		for (int32 i = 0; i < SectionNumTriangles; i++)
		{
			int32 TriVertexIndex = i * 3 + SectionBaseIndex;
			Triangles.Add(IndicesData.Indices[TriVertexIndex]);
			Triangles.Add(IndicesData.Indices[TriVertexIndex + 1]);
			Triangles.Add(IndicesData.Indices[TriVertexIndex + 2]);
		}


		FTransform SkeletonTransform = Skeleton->GetComponentTransform();
		CutProceduralMesh_1->SetWorldTransform(SkeletonTransform);

		CutProceduralMesh_1->CreateMeshSection(j, Vertices, Triangles, Normals, UV, Colors, Tangents, true);

		for (int n = 0; n < Skeleton->GetNumMaterials(); n++) {
			CutProceduralMesh_1->SetMaterial(n, GetMesh()->GetMaterial(n));
		}
	}


	CutProceduralMesh_1->SetVisibility(true);
	CutProceduralMesh_1->SetHiddenInGame(false);
	CutProceduralMesh_1->RegisterComponent();

	// ProceduralMesh에 Convex Collision 추가
	CutProceduralMesh_1->AddCollisionConvexMesh(Vertices_Convex);

	CutProceduralMesh_1->SetSimulatePhysics(true);
	CutProceduralMesh_1->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	CutProceduralMesh_1->SetCollisionProfileName(TEXT("Ragdoll"));


	// 절단 부위 proceduralmesh 생성
	SliceProceduralmeshTest(planeposition, planenormal);

	//UE_LOG(LogTemp, Log, TEXT("IsPhysicsStateCreated: %s"), CutProceduralMesh_1->IsPhysicsStateCreated() ? TEXT("true") : TEXT("false"));
	//UE_LOG(LogTemp, Log, TEXT("ShouldCreatePhysicsState: %s"), CutProceduralMesh_1->ShouldCreatePhysicsState() ? TEXT("true") : TEXT("false"));
	//UE_LOG(LogTemp, Log, TEXT("IsSimulatingPhysics: %s"), CutProceduralMesh_1->IsSimulatingPhysics() ? TEXT("true") : TEXT("false"));


	GetMesh()->SetCollisionProfileName("NoCollision");		// 생각해보니 이거 궂이 필요한...가? (+ 초기값은 "Zombie")
															// 어차피 플레이어는 콜리전 프리셋 설정이 메시는 NoCollision, 캡슐은 Pawn 이라서 좀비 메시랑은 항상 충돌 무시되어 왔음 (초기값 Zombie가 Pawn을 무시함)
	GetMesh()->SetGenerateOverlapEvents(false);
	// 기존 SkeletalMesh 안 보이게 설정
	GetMesh()->SetVisibility(false);


	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("CreativeProceduralMesh END")));

}

void ABaseZombie::SliceProceduralmeshTest(FVector planeposition, FVector planenormal)
{
	if (CutProceduralMesh_1)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("SliceProceduralmeshTest START")));

		UProceduralMeshComponent* procHit = Cast<UProceduralMeshComponent>(CutProceduralMesh_1);

		UKismetProceduralMeshLibrary::SliceProceduralMesh(
			procHit,
			planeposition,
			planenormal,
			true,
			CutProceduralMesh_2,
			EProcMeshSliceCapOption::CreateNewSectionForCap,
			Material_Blood
		);

		if (CutProceduralMesh_2)
		{
			CutProceduralMesh_2->RegisterComponent(); // 컴포넌트 등록

			// 러닝 좀비 텍스쳐 깨짐 해결
			if (m_sZombieName == "RunningZombie") {
				if (CutProceduralMesh_2->GetNumMaterials() == 2) {
					CutProceduralMesh_2->SetMaterial(0, GetMesh()->GetMaterial(2));
				}
				else if (CutProceduralMesh_2->GetNumMaterials() == 3) {
					CutProceduralMesh_2->SetMaterial(0, GetMesh()->GetMaterial(2));
					CutProceduralMesh_2->SetMaterial(1, GetMesh()->GetMaterial(3));
				}
				else if (CutProceduralMesh_2->GetNumMaterials() == 4) {
					CutProceduralMesh_2->SetMaterial(0, GetMesh()->GetMaterial(1));
					CutProceduralMesh_2->SetMaterial(1, GetMesh()->GetMaterial(2));
					CutProceduralMesh_2->SetMaterial(2, GetMesh()->GetMaterial(3));
				}
				else if (CutProceduralMesh_2->GetNumMaterials() == 5) {
					CutProceduralMesh_2->SetMaterial(0, GetMesh()->GetMaterial(0));
					CutProceduralMesh_2->SetMaterial(1, GetMesh()->GetMaterial(1));
					CutProceduralMesh_2->SetMaterial(2, GetMesh()->GetMaterial(2));
					CutProceduralMesh_2->SetMaterial(3, GetMesh()->GetMaterial(3));
				}
			}

			// 절단 부위 material 설정
			CutProceduralMesh_2->SetMaterial(CutProceduralMesh_2->GetNumMaterials() - 1, Material_Blood);

			CutProceduralMesh_2->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
			CutProceduralMesh_2->SetCollisionProfileName(TEXT("Ragdoll"));
			CutProceduralMesh_2->SetSimulatePhysics(true);


			// Blood Effect rootcomponent(proc mesh) 설정 후 스폰
			if (BloodFX.Num() >= 2) {
				if (BloodFX[0] && BloodFX[1]) {
					BloodFX[0]->ProcMesh = CutProceduralMesh_1;
					BloodFX[1]->ProcMesh = CutProceduralMesh_2;
					BloodFX[0]->spawn_flag = true;
					BloodFX[1]->spawn_flag = true;
				}
			}
			else {
				//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("[Error] BloodFX.Num() < 2 => BloodFX spawn failed!!!")));
				UE_LOG(LogTemp, Error, TEXT("[Error] BloodFX.Num() < 2 => BloodFX spawn failed!!!"));
			}

		}

		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("SliceProceduralmeshTest END")));
	}
}

void ABaseZombie::SetCuttingDeadWithAnim()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, FString::Printf(TEXT("좀비 사망 직접 실행! - cut dead")));
	//UE_LOG(LogTemp, Log, TEXT("좀비 사망 클라 직접 실행! - cut dead"));

	m_bIsCuttingDead = true;
	auto CharacterAnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr != CharacterAnimInstance) {
		CharacterAnimInstance->SetIsCuttingDead(m_bIsCuttingDead);
	}
	GetCapsuleComponent()->SetCollisionProfileName("NoCollision");

	//StartResurrectionTimer();
}

void ABaseZombie::Attack(uint32 PlayerId)
{
	if (m_bIsAttacking) {
		return;
	}

	if (m_bBeAttacked) {
		return;
	}

	auto AnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());

	AnimInstance->PlayAttackMontage();

	m_bIsAttacking = true;

	if (GetZombieName() == "NormalZombie") {
		AZombieAIController* NormalZombieAIController = Cast<AZombieAIController>(GetController());

		NormalZombieAIController->attackPlayerID = PlayerId;
	}
	else if (GetZombieName() == "RunningZombie") {
		ARunningZombieAIController* RunningZombieAIController = Cast<ARunningZombieAIController>(GetController());

		RunningZombieAIController->attackPlayerID = PlayerId;
	}
	else if (GetZombieName() == "ShoutingZombie") {
		AShoutingZombieAIController* ShoutingZombieAIController = Cast<AShoutingZombieAIController>(GetController());

		ShoutingZombieAIController->attackPlayerID = PlayerId;
	}
}

void ABaseZombie::AttackMontageEnded(UAnimMontage* Montage, bool interrup)
{
	m_bIsAttacking = false;

	m_DAttackEnd.Broadcast();
}

// 좀비가 플레이어 공격할 때 충돌체크
void ABaseZombie::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * m_fAttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(m_fAttackRadius),
		Params
	);


	// debug 용(충돌 범위 확인 용)
	//FVector TraceVec = GetActorForwardVector() * m_fAttackRange;
	//FVector Center = GetActorLocation() + TraceVec * 0.5f;
	//float HalfHeight = m_fAttackRange * 0.5f + 50.f;
	//FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	//FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	//float DebugLifeTime = m_fAttackRadius;


	//DrawDebugCapsule(GetWorld(),
	//	Center,
	//	HalfHeight,
	//	m_fAttackRadius,
	//	CapsuleRot,
	//	DrawColor,
	//	false,
	//	DebugLifeTime);

	if (bResult) {

		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Hit Actor")));
		//UE_LOG(LogNet, Display, TEXT("Player #%d got hit - HP: %d"), HitResult.GetActor()->PlayerId, HitResult.GetActor()->GetHp());
		FDamageEvent DamageEvent;
		HitResult.GetActor()->TakeDamage(GetSTR(), DamageEvent, GetController(), this);
	}
}

void ABaseZombie::Shouting()
{
	if (m_bIsShouting) {
		return;
	}

	FVector SpawnOffset = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 5.0f);

	ShoutingFX = GetWorld()->SpawnActor<AShoutingNiagaEffect>(AShoutingNiagaEffect::StaticClass(), SpawnOffset, FRotator::ZeroRotator);

	auto AnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());

	AnimInstance->PlayShoutingMontage();

	AnimInstance->SetCurrentPawnSpeed(0);


	//UWorld* World = GetWorld();
	//FVector Center = GetActorLocation();
	//float DetectRadius = 2000.f;

	//if (nullptr == World) return;
	//TArray<FOverlapResult> OverlapResults;
	//FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);
	//bool bResult = World->OverlapMultiByChannel(
	//	OverlapResults,
	//	Center,
	//	FQuat::Identity,
	//	ECollisionChannel::ECC_GameTraceChannel4,
	//	FCollisionShape::MakeSphere(DetectRadius),
	//	CollisionQueryParam
	//);

	//if (bResult) {

	//	for (const FOverlapResult& OverlapResult : OverlapResults)
	//	{
	//		// ABaseZombie인지 확인
	//		ABaseZombie* OverlappedZombie = Cast<ABaseZombie>(OverlapResult.GetActor());
	//		if (OverlappedZombie)
	//		{
	//			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("HEAR"));
	//			OverlappedZombie->UpdateLastKnownPositionByFootSound(GetActorLocation());
	//		}

	//	}
	//}

	m_bIsShouting = true;


	UE_LOG(LogTemp, Log, TEXT("IsShouted: %s"), IsShouted() ? TEXT("true") : TEXT("false"));
	UE_LOG(LogTemp, Log, TEXT("PLAYSHOUTINHGGGGGGGGGGGGGGGGGGGGGGGGGGG"));
}

void ABaseZombie::ShoutingMontageEnded(UAnimMontage* Montage, bool interrup)
{
	if(m_bIsShouting)
		UE_LOG(LogTemp, Error, TEXT("bIsShouted true"));
	
	m_bIsShouting = false;
	SetShouted(true);
	
	m_DShoutingEnd.Broadcast();

	if (ShoutingFX != NULL) {
		ShoutingFX->EndPlay(EEndPlayReason::Destroyed);
	}

}

void ABaseZombie::BeAttacked()
{
	if (m_bBeAttacked) {
		return;
	}
	auto AnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());

	if (m_bIsAttacking) {
		AnimInstance->Montage_Stop(0.5f);
	}

	AnimInstance->PlayBeAttackedMontage();

	GetCharacterMovement()->MaxWalkSpeed = 1.f;
	m_bBeAttacked = true;
}

void ABaseZombie::BeAttackedMontageEnded(UAnimMontage* Montage, bool interrup)
{
	m_bBeAttacked = false;
	GetCharacterMovement()->MaxWalkSpeed = GetSpeed() * 100.f;

	doAction_takeDamage_onTick = false;

	//if (BloodFX != NULL) {
	//	BloodFX->EndPlay(EEndPlayReason::Destroyed);
	//}
}

void ABaseZombie::SetZombieId(uint32 NewZombieId)
{
	ZombieId = NewZombieId;
}

uint32 ABaseZombie::GetZombieId() const
{
	return ZombieId;
}

void ABaseZombie::UpdateZombieData(FVector Location)
{
	NewLocation = Location;
}

// 사망 후 되살아나기 타이머
void ABaseZombie::StartResurrectionTimer()
{

	if (m_bIsNormalDead) {
		GetWorld()->GetTimerManager().SetTimer(ResurrectionHandle, this, &ABaseZombie::ResurrectionTimerElapsed, 30.0f, false);		// 30초 후 다시 일어나기 시작
	}
	else if (m_bIsCuttingDead) {
		GetWorld()->GetTimerManager().SetTimer(ResurrectionHandle, this, &ABaseZombie::ResurrectionTimerElapsed, 60.0f, false);		// 60초 후 다시 일어나기 시작
	}

}

void ABaseZombie::ResurrectionTimerElapsed()
{
	m_bIsCuttingDead = false;
	m_bIsNormalDead = false;

	USkeletalMeshComponent* Skeleton = GetMesh();
	TArray<FName> AllBoneNames;
	Skeleton->GetBoneNames(AllBoneNames);

	// 본의 경로에서 숨김을 해제함
	for (const FName& PathBoneName : AllBoneNames)
	{
		Skeleton->UnHideBoneByName(PathBoneName);
	}

	m_bIsStanding = true;
	auto CharacterAnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr != CharacterAnimInstance) {
		CharacterAnimInstance->SetIsStanding(m_bIsStanding);
	}
	StartWatiingTimer();
}

// 되살아나기 애니메이션 워이팅 타이머
void ABaseZombie::StartWatiingTimer()
{
	GetWorld()->GetTimerManager().SetTimer(WattingHandle, this, &ABaseZombie::WaittingTimerElapsed, 5.f, false);	// 5초 이후 완전히 다시 살아남
}

void ABaseZombie::WaittingTimerElapsed()
{
	m_bIsStanding = false;
	auto CharacterAnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr != CharacterAnimInstance) {
		CharacterAnimInstance->SetIsNormalDead(m_bIsNormalDead);
		CharacterAnimInstance->SetIsCuttingDead(m_bIsCuttingDead);
		CharacterAnimInstance->SetIsStanding(m_bIsStanding);
	}
	GetCharacterMovement()->MaxWalkSpeed = GetSpeed() * 100.f;
	GetCapsuleComponent()->SetCollisionProfileName("ZombieCol");
	SetHP(GetStartHP());

	SetDie(false);

	doAction_takeDamage_onTick = false;
	doAction_setIsNormalDead_onTick = false;		// 이거 지금 ResurrectionTimerElapsed를 모두 주석해놔서 불릴 일이 없긴함 (즉, 해당 클라가 좀비 직접 죽이면 doAction_setIsNormalDead_onTick 값 영원히 false임)
	doAction_setIsCuttingDead_onTick = false;
	// 그래도 부활하는 걸 고려하면 여기에 설정하는 게 맞음
}


