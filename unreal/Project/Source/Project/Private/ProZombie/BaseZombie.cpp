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
#include "ProItem/NormalWeaponActor.h"

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

	BloodFX = CreateDefaultSubobject<ABloodNiagaEffect>(TEXT("BloodFX"));

	BloodFX = nullptr;

	ShoutingFX = CreateDefaultSubobject<AShoutingNiagaEffect>(TEXT("ShoutingFX"));

	ShoutingFX = nullptr;

	ConstructorHelpers::FObjectFinder<UMaterial> MaterialFinder(TEXT("/Engine/EngineDebugMaterials/VertexColorViewMode_RedOnly.VertexColorViewMode_RedOnly"));
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
	if (CutProceduralMesh_1) {
		if (procMesh_AddImpulse_1 == false) {
			CutProceduralMesh_1->AddImpulseAtLocation(FVector(0.f, 0.f, 10000.0f), CutProceduralMesh_1->K2_GetComponentLocation());
			procMesh_AddImpulse_1 = true;
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
		if (procMesh_AddImpulse_2 == false) {
			CutProceduralMesh_2->AddImpulseAtLocation(FVector(10000.f, 10000.f, 0.f), CutProceduralMesh_2->K2_GetComponentLocation());
			procMesh_AddImpulse_2 = true;
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

	Super::Tick(DeltaTime);

}

void ABaseZombie::OnZombieHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != nullptr && OtherActor != this) {
		UE_LOG(LogTemp, Warning, TEXT("Zombie collided with %s"), *OtherActor->GetName());
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

// 좀비가 공격을 받았을 때
float ABaseZombie::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("HP %f"), GetHP()));

	SetHP(GetHP() - Damage);
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("HP %f"), GetHP()));

	ANormalWeaponActor* Weapon = Cast<ANormalWeaponActor>(DamageCauser);

	if (Weapon == nullptr) {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("[ERROR] TakeDamage - Weapon is nullptr!!! No Damage!")));
		return 0;
	}

	UE_LOG(LogTemp, Log, TEXT("Weapon Location: %s"), *Weapon->GetActorLocation().ToString());
	UE_LOG(LogTemp, Log, TEXT("Weapon Rotation: %s"), *Weapon->GetActorRotation().ToString());

	
	BloodFX = GetWorld()->SpawnActor<ABloodNiagaEffect>(ABloodNiagaEffect::StaticClass(), Weapon->GetActorLocation(), Weapon->GetActorRotation());
	UE_LOG(LogTemp, Log, TEXT("BloodFX SpawnActor"));
	if (GetHP() <= 0) {	// 죽을때(절단 될 때)는 피가 더 많이 튀도록
		BloodFX->blood_spawncount = FMath::RandRange(300, 400);
	}
	else {
		BloodFX->blood_spawncount = FMath::RandRange(80, 100);
	}
	UE_LOG(LogTemp, Log, TEXT("BloodFX SpawnCounter Set - %d"), BloodFX->blood_spawncount);
	BloodFX->spawn_flag = true;
	UE_LOG(LogTemp, Log, TEXT("BloodFX SpawnFlag True"));

	BeAttacked();

	return Damage;
}

void ABaseZombie::SetNormalDeadWithAnim()
{
	m_bIsNormalDead = true;
	auto CharacterAnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr != CharacterAnimInstance) {
		CharacterAnimInstance->SetIsNormalDead(m_bIsNormalDead);
	}
	GetCapsuleComponent()->SetCollisionProfileName("NoCollision");

	//StartResurrectionTimer();

}

void ABaseZombie::CutZombie(FVector planeposition, FVector planenormal)
{
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

	UE_LOG(LogTemp, Log, TEXT("RenderSections.Num (int) %d"), DataArray.RenderSections.Num());

	for (int32 j = 0; j < DataArray.RenderSections.Num(); j++)
	{
		const int32 NumSourceVertices = DataArray.RenderSections[j].NumVertices;
		const int32 BaseVertexIndex = DataArray.RenderSections[j].BaseVertexIndex;

		UE_LOG(LogTemp, Log, TEXT("RenderSections[%d] - NumSourceVertices.Num (int) %d"), j, DataArray.RenderSections[j].NumVertices);

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


	GetMesh()->SetCollisionProfileName("NoCollision");
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
				CutProceduralMesh_2->SetMaterial(0, GetMesh()->GetMaterial(2));
			}

			// 절단 부위 material 설정
			CutProceduralMesh_2->SetMaterial(CutProceduralMesh_2->GetNumMaterials() - 1, Material_Blood);

			CutProceduralMesh_2->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
			CutProceduralMesh_2->SetCollisionProfileName(TEXT("Ragdoll"));
			CutProceduralMesh_2->SetSimulatePhysics(true);

		}

		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("SliceProceduralmeshTest END")));
	}
}

void ABaseZombie::SetCuttingDeadWithAnim()
{
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
	ShoutingFX->OwnerZombie = this;

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


	UE_LOG(LogTemp, Log, TEXT("Character is Die :: %s"), IsShouted() ? TEXT("true") : TEXT("false"));
	UE_LOG(LogTemp, Error, TEXT("PLAYSHOUTINHGGGGGGGGGGGGGGGGGGGGGGGGGGG"));
}

void ABaseZombie::ShoutingMontageEnded(UAnimMontage* Montage, bool interrup)
{
	m_bIsShouting = false;
	SetShouted(true);
	UE_LOG(LogTemp, Error, TEXT("bIsShouted true"));
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

	if (BloodFX != NULL) {
		BloodFX->EndPlay(EEndPlayReason::Destroyed);
	}
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

void ABaseZombie::StartResurrectionTimer()
{

	if (m_bIsNormalDead) {
		GetWorld()->GetTimerManager().SetTimer(ResurrectionHandle, this, &ABaseZombie::ResurrectionTimerElapsed, 30.0f, false);
	}
	else if (m_bIsCuttingDead) {
		GetWorld()->GetTimerManager().SetTimer(ResurrectionHandle, this, &ABaseZombie::ResurrectionTimerElapsed, 60.0f, false);
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

void ABaseZombie::StartWatiingTimer()
{
	GetWorld()->GetTimerManager().SetTimer(WattingHandle, this, &ABaseZombie::WaittingTimerElapsed, 5.f, false);
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

	SetDie(true);
}


