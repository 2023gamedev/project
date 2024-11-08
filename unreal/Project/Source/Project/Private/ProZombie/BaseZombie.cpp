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

#include "Rendering/SkeletalMeshRenderData.h"
#include "Rendering/SkeletalMeshLODRenderData.h"
#include "Rendering/SkeletalMeshLODModel.h"
#include "KismetProceduralMeshLibrary.h"
#include "Materials/MaterialInterface.h"
#include "Kismet/GameplayStatics.h"
#include "ProceduralMeshComponent.h"

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

}

// Called when the game starts or when spawned
void ABaseZombie::BeginPlay()
{
	Super::BeginPlay();

	CachedAnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());

	GameInstance = Cast<UProGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	//GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ABaseZombie::OnZombieHit);
}

// Called every frame
void ABaseZombie::Tick(float DeltaTime)
{
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

	//auto CharacterAnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());
	//if (nullptr != CharacterAnimInstance) {
	//	CharacterAnimInstance->SetCurrentPawnSpeed(GetVelocity().Size());
	//}

	//if (OldLocation != FVector(0.0f, 0.0f, 0.0f)) {
	//	float DistanceMoved = FVector::Dist(OldLocation, NewLocation);
	//	Speed = (DeltaTime > 0) ? (DistanceMoved / DeltaTime) : 0;
	//	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("zombie speed: %f"), Speed));
	//}
	//
	//// 애니메이션 인스턴스에 속도 파라미터 설정
	//if ((Speed != 0 && PreviousSpeed == 0) || (Speed == 0 && PreviousSpeed != 0))
	//{
	//	if (CachedAnimInstance) {
	//		CachedAnimInstance->SetCurrentPawnSpeed(Speed);
	//	}
	//}
	//
	//PreviousSpeed = Speed;
	//OldLocation = NewLocation;
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
	BloodFX = GetWorld()->SpawnActor<ABloodNiagaEffect>(ABloodNiagaEffect::StaticClass(), this->GetActorLocation(), FRotator::ZeroRotator);
	BloodFX->OwnerZombie = this;

	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("HP %f"), GetHP()));
	SetHP(GetHP() - Damage);
	BeAttacked();

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("HP %f"), GetHP()));
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

	StartResurrectionTimer();

	StopAITree();
}

void ABaseZombie::StopAITree()
{
	if (GetZombieName() == "NormalZombie") {
		AZombieAIController* NormalZombieAIController = Cast<AZombieAIController>(GetController());

		//NormalZombieAIController->StopAI();

	}
	else if (GetZombieName() == "RunningZombie") {
		ARunningZombieAIController* RunningZombieAIController = Cast<ARunningZombieAIController>(GetController());

		//RunningZombieAIController->StopAI();
	}
	else if (GetZombieName() == "ShoutingZombie") {
		AShoutingZombieAIController* ShoutingZombieAIController = Cast<AShoutingZombieAIController>(GetController());

		//ShoutingZombieAIController->StopAI();
	}

}

void ABaseZombie::CutZombie(FName bonename)
{
	//FName TestBone = "Head";
	USkeletalMeshComponent* Skeleton = GetMesh();
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("CutBone!"));
	if (Skeleton) {
		FString BoneNameS = bonename.ToString();
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("CutBone: %s"), *BoneNameS));

		// ProceduralMeshComponent 생성 및 설정
		CutProceduralMesh = NewObject<UProceduralMeshComponent>(this);
		if (!CutProceduralMesh) return;

		CreativeProceduralMesh(bonename);
		// CreateProceduralMeshFromBoneVertices(GetMesh(), bonename, CutProceduralMesh);

		Skeleton->HideBoneByName(bonename, EPhysBodyOp::PBO_None);

	}
}

// 프로시저럴 메쉬 생성되는 부분
void ABaseZombie::CreativeProceduralMesh(FName bonename)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("CreativeProceduralMesh")));
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("CreativeProceduralMesh")));
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("CreativeProceduralMesh")));
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("CreativeProceduralMesh")));
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("CreativeProceduralMesh")));



	USkeletalMeshComponent* Skeleton = GetMesh();




	FSkeletalMeshRenderData* SkMeshRenderData = Skeleton->GetSkeletalMeshRenderData();
	const FSkeletalMeshLODRenderData& DataArray = SkMeshRenderData->LODRenderData[0];
	FSkinWeightVertexBuffer& SkinWeights = *Skeleton->GetSkinWeightBuffer(0);

	TArray<FVector> Vertices;
	TArray<FVector> Normals;
	TArray<FVector2D> UV;
	TArray<FColor> Colors;
	TArray<FProcMeshTangent> Tangents;

	for (int32 j = 0; j < DataArray.RenderSections.Num(); j++)
	{
		const int32 NumSourceVertices = DataArray.RenderSections[j].NumVertices;
		const int32 BaseVertexIndex = DataArray.RenderSections[j].BaseVertexIndex;

		for (int32 i = 0; i < NumSourceVertices; i++)
		{
			const int32 VertexIndex = i + BaseVertexIndex;
			const FVector3f SkinnedVectorPos = USkeletalMeshComponent::GetSkinnedVertexPosition(Skeleton, VertexIndex, DataArray, SkinWeights);
			
			Vertices.Add(FVector(SkinnedVectorPos.X, SkinnedVectorPos.Y, SkinnedVectorPos.Z));

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

		CutProceduralMesh->AttachToComponent(Skeleton, FAttachmentTransformRules::KeepRelativeTransform);
		CutProceduralMesh->CreateMeshSection(j, Vertices, Triangles, Normals, UV, Colors, Tangents, true);
		CutProceduralMesh->SetMaterial(0, Material);
		CutProceduralMesh->SetMaterial(1, Material2);
		CutProceduralMesh->SetMaterial(2, Material3);
		CutProceduralMesh->SetVisibility(true);
		CutProceduralMesh->SetHiddenInGame(false);
		CutProceduralMesh->RegisterComponent();

	}
	// 기존 SkeletalMesh 안 보이게 설정
	GetMesh()->SetVisibility(false);
	UE_LOG(LogTemp, Log, TEXT("RenderSections.Num (int) %d"), DataArray.RenderSections.Num());




	//TArray<FVector> Vertices;
	//TArray<int32> Triangles;

	//USkeletalMesh* SkeletalMesh = GetMesh()->SkeletalMesh/* 스켈레탈 메쉬 객체 */;
	//const TArray<FTransform>& BoneTransforms = GetMesh()->GetComponentSpaceTransforms();
	//if (SkeletalMesh && SkeletalMesh->GetResourceForRendering())
	//{
	//	const FSkeletalMeshRenderData* RenderData = SkeletalMesh->GetResourceForRendering();
	//	const FSkeletalMeshLODRenderData& LODRenderData = RenderData->LODRenderData[0]; // LOD 0을 가져옴

	//	const FSkinWeightVertexBuffer& SkinWeights = LODRenderData.SkinWeightVertexBuffer;

	//	FString Message = FString::Printf(TEXT("Number of Vertices: %d"), LODRenderData.GetNumVertices());
	//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, Message);

	//	for (uint32 VertexIndex = 0; VertexIndex < LODRenderData.GetNumVertices(); ++VertexIndex)
	//	{

	//		uint32 VertexWeightOffset = 0;
	//		uint32 VertexInfluenceCount = 0;
	//		SkinWeights.GetVertexInfluenceOffsetCount(VertexIndex, VertexWeightOffset, VertexInfluenceCount);
	//		FVector3f OriginalVertexPosition3f = LODRenderData.StaticVertexBuffers.PositionVertexBuffer.VertexPosition(VertexIndex);
	//		FVector OriginalVertexPosition = FVector(OriginalVertexPosition3f.X, OriginalVertexPosition3f.Y, OriginalVertexPosition3f.Z);
	//		FVector TransformedVertexPosition = FVector::ZeroVector;

	//		// 버텍스에 영향을 미치는 본들을 기반으로 변형 계산
	//		for (uint32 InfluenceIndex = 0; InfluenceIndex < VertexInfluenceCount; ++InfluenceIndex)
	//		{
	//			uint8 BoneIndex = SkinWeights.GetBoneIndex(VertexIndex, InfluenceIndex);
	//			float BoneWeight = (float)(SkinWeights.GetBoneWeight(VertexIndex, InfluenceIndex) / 255.0f);
	//			float BoneWeightOrigin = (float)(SkinWeights.GetBoneWeight(VertexIndex, InfluenceIndex));
	//			UE_LOG(LogTemp, Log, TEXT("Weight (int) %d"),SkinWeights.GetBoneWeight(VertexIndex, InfluenceIndex));

	//			//UE_LOG(LogTemp, Log, TEXT("Vertex %d, Bone %d: Weight %.2f, Weight2 %.2f"), VertexIndex, BoneIndex, BoneWeight, BoneWeightOrigin);

	//			if (BoneIndex < BoneTransforms.Num())
	//			{
	//				// 본의 변환 행렬 적용
	//				FVector SkinnedPosition = BoneTransforms[BoneIndex].TransformPosition(OriginalVertexPosition);
	//				TransformedVertexPosition += SkinnedPosition * BoneWeight;
	//			}
	//		}

	//		// 결과 출력
	//		Vertices.Add(FVector(TransformedVertexPosition.X, TransformedVertexPosition.Y, TransformedVertexPosition.Z));
	//		UE_LOG(LogTemp, Log, TEXT("Transformed Vertex %d: Position (%f, %f, %f)"), VertexIndex, TransformedVertexPosition.X, TransformedVertexPosition.Y, TransformedVertexPosition.Z);
	//	}
	//}

	//const FSkeletalMeshRenderData* RenderData = Skeleton->GetSkeletalMeshRenderData();
	//if (RenderData == nullptr)
	//{
	//	return;
	//}
	//const FSkeletalMeshLODRenderData& LODData = RenderData->LODRenderData[0]; // LOD0 사용

	////const int32 NumVertices = LODData.StaticVertexBuffers.PositionVertexBuffer.GetNumVertices();
	////for (int32 i = 0; i < NumVertices; ++i)
	////{
	////	// PositionVertexBuffer의 버텍스 위치를 가져와 Vertices 배열에 추가합니다.
	////	FVector3f VertexPos = LODData.StaticVertexBuffers.PositionVertexBuffer.VertexPosition(i);
	////	Vertices.Add(FVector(VertexPos.X, VertexPos.Y, VertexPos.Z));
	////}

	//const FRawStaticIndexBuffer16or32Interface* IndexBuffer = LODData.MultiSizeIndexContainer.GetIndexBuffer();
	//if (!IndexBuffer)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("IndexBuffer is nullptr"));
	//	return;
	//}
	//const int32 NumIndices = IndexBuffer->Num();
	//for (int32 i = 0; i < NumIndices; i += 3)
	//{
	//	int32 Index0 = IndexBuffer->Get(i);
	//	int32 Index1 = IndexBuffer->Get(i + 1);
	//	int32 Index2 = IndexBuffer->Get(i + 2);

	//	Triangles.Add(Index0);
	//	Triangles.Add(Index1);
	//	Triangles.Add(Index2);
	//}
	//

	////const FStaticMeshVertexBuffer& VertexBuffer = LODData.StaticVertexBuffers.StaticMeshVertexBuffer;
	////// 모든 본을 순회
	////for (int32 BoneIndex = 0; BoneIndex < Skeleton->GetNumBones(); ++BoneIndex)
	////{
	////	
	////	// 각 본의 트랜스폼을 가져옴
	////	FTransform BoneTransform = Skeleton->GetBoneTransform(BoneIndex);
	////	FVector BoneLocation = BoneTransform.GetLocation();
	////	FQuat BoneRotation = BoneTransform.GetRotation();
	////	UE_LOG(LogTemp, Warning, TEXT("Bone %d Location: %s"), BoneIndex, *BoneLocation.ToString());
	////	for (int32 VertexIndex = 0; VertexIndex < NumVertices; ++VertexIndex)
	////	{


	////		FVector UpdatedPosition = BoneLocation + BoneRotation.RotateVector(Vertices[VertexIndex]);
	////		Vertices[VertexIndex] = UpdatedPosition;

	////	}
	////}
	//
	//// 2. 추출한 데이터로 프로시저럴 메쉬 생성
	////TArray<FVector> Normals;
	//TArray<FVector2D> UVs;
	//TArray<FLinearColor> VertexColors;
	////TArray<FProcMeshTangent> Tangents;

	//// 기본 값을 설정
	////Normals.Init(FVector(0, 0, 1), Vertices.Num());
	//UVs.Init(FVector2D(0, 0), Vertices.Num());
	//VertexColors.Init(FLinearColor::Black, Vertices.Num());

	//UE_LOG(LogTemp, Warning, TEXT("Vertices count: %d"), Vertices.Num());
	//UE_LOG(LogTemp, Warning, TEXT("Triangles count: %d"), Triangles.Num());
	//TArray<FVector> Normals;
	//TArray<FProcMeshTangent> Tangents;
	//UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UVs, Normals, Tangents);
	//CutProceduralMesh->AttachToComponent(Skeleton, FAttachmentTransformRules::KeepRelativeTransform);
	//CutProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, true);
	////CutProceduralMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
	//CutProceduralMesh->SetMaterial(0, Material);
	//CutProceduralMesh->SetMaterial(1, Material2);
	//CutProceduralMesh->SetMaterial(2, Material3);
	//CutProceduralMesh->SetVisibility(true);
	//CutProceduralMesh->SetHiddenInGame(false);
	//CutProceduralMesh->RegisterComponent();

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("CreativeProceduralMesh END")));
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("CreativeProceduralMesh END")));
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("CreativeProceduralMesh END")));
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("CreativeProceduralMesh END")));
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("CreativeProceduralMesh END")));


}

void ABaseZombie::GetBoneInfluencedVertices(USkeletalMeshComponent* SkeletalMeshComp, FName BoneName, TArray<int32>& OutVertexIndices)
{
	//if (!SkeletalMeshComp || !SkeletalMeshComp->SkeletalMesh) return;

	//const int32 BoneIndex = SkeletalMeshComp->GetBoneIndex(BoneName);
	//if (BoneIndex == INDEX_NONE) return;

	//FSkeletalMeshRenderData* RenderData = SkeletalMeshComp->GetSkeletalMeshRenderData();
	//if (!RenderData) return;

	//for (int32 LODIndex = 0; LODIndex < 1; ++LODIndex)
	//{
	//	const FSkeletalMeshLODRenderData& LODRenderData = RenderData->LODRenderData[LODIndex];
	//	const TArray<FSkelMeshRenderSection>& Sections = LODRenderData.RenderSections;

	//	for (const FSkelMeshRenderSection& Section : Sections)
	//	{
	//		const int32 NumVertices = Section.GetNumVertices();
	//		
	//		for (int32 VertexIndex = 0; VertexIndex < NumVertices; ++VertexIndex)
	//		{
	//			const int32 VertexBufferIndex = Section.BaseVertexIndex + VertexIndex;

	//			// Ensure BoneMap access is within bounds
	//			const int32 NumInfluences = FMath::Min(Section.BoneMap.Num(), MAX_TOTAL_INFLUENCES);

	//			for (int32 InfluenceIdx = 0; InfluenceIdx < NumInfluences; ++InfluenceIdx)
	//			{
	//				const int32 BoneMapIndex = Section.BoneMap[InfluenceIdx];
	//				if (BoneMapIndex == BoneIndex)
	//				{
	//					OutVertexIndices.Add(VertexBufferIndex);
	//					break;
	//				}
	//			}
	//		}
	//	}
	//}

	//if (!SkeletalMeshComp || !SkeletalMeshComp->SkeletalMesh) return;

	//const int32 BoneIndex = SkeletalMeshComp->GetBoneIndex(BoneName);
	//if (BoneIndex == INDEX_NONE) return;

	//FSkeletalMeshRenderData* RenderData = SkeletalMeshComp->GetSkeletalMeshRenderData();
	//if (!RenderData) return;

	//int32 NumVerticesAdded = 0;

	//for (int32 LODIndex = 0; LODIndex < 1; ++LODIndex)
	//{
	//	const FSkeletalMeshLODRenderData& LODRenderData = RenderData->LODRenderData[LODIndex];
	//	const TArray<FSkelMeshRenderSection>& Sections = LODRenderData.RenderSections;

	//	for (const FSkelMeshRenderSection& Section : Sections)
	//	{
	//		const int32 NumVertices = Section.GetNumVertices();

	//		for (int32 VertexIndex = 0; VertexIndex < NumVertices; ++VertexIndex)
	//		{
	//			if (NumVerticesAdded >= 500)
	//			{
	//				// Reached the maximum number of vertices to add, exit the loop
	//				break;
	//			}

	//			const int32 VertexBufferIndex = Section.BaseVertexIndex + VertexIndex;

	//			// Ensure BoneMap access is within bounds
	//			const int32 NumInfluences = FMath::Min(Section.BoneMap.Num(), MAX_TOTAL_INFLUENCES);

	//			for (int32 InfluenceIdx = 0; InfluenceIdx < NumInfluences; ++InfluenceIdx)
	//			{
	//				const int32 BoneMapIndex = Section.BoneMap[InfluenceIdx];
	//				if (BoneMapIndex == BoneIndex)
	//				{
	//					OutVertexIndices.Add(VertexBufferIndex);
	//					NumVerticesAdded++;

	//					// Break out of the inner loop once the vertex is added
	//					break;
	//				}
	//			}
	//		}

	//		if (NumVerticesAdded >= 500)
	//		{
	//			// Reached the maximum number of vertices to add, exit the outer loop
	//			break;
	//		}
	//	}

	//	if (NumVerticesAdded >= 500)
	//	{
	//		// Reached the maximum number of vertices to add, exit the outer loop
	//		break;
	//	}
	//}


	if (!SkeletalMeshComp || !SkeletalMeshComp->SkeletalMesh)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("SkeletalMeshComp or SkeletalMesh is invalid!"));
		return;
	}

	const int32 BoneIndex = SkeletalMeshComp->GetBoneIndex(BoneName);
	if (BoneIndex == INDEX_NONE)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Bone index is invalid!"));
		return;
	}

	// Get the location of the bone
	FVector BoneLocation = SkeletalMeshComp->GetBoneLocation(BoneName);

	FVector BoneLocation2 = SkeletalMeshComp->GetBoneLocation(BoneName, EBoneSpaces::ComponentSpace);

	FSkeletalMeshRenderData* RenderData = SkeletalMeshComp->GetSkeletalMeshRenderData();
	if (!RenderData)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Render data is invalid!"));
		return;
	}

	for (int32 LODIndex = 0; LODIndex < 1; ++LODIndex)
	{
		const FSkeletalMeshLODRenderData& LODRenderData = RenderData->LODRenderData[LODIndex];
		const FPositionVertexBuffer& PositionVertexBuffer = LODRenderData.StaticVertexBuffers.PositionVertexBuffer;

		for (int32 VertexIndex = 0; VertexIndex < (int32)(PositionVertexBuffer.GetNumVertices()); ++VertexIndex)
		{
			FVector3f VL3f = PositionVertexBuffer.VertexPosition(VertexIndex);
			FVector VertexLocation = FVector(VL3f.X, VL3f.Y, VL3f.Z);


			// 디버그 메시지로 X 좌표 출력
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Vertex X: %f"), VertexLocation.X));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Bone X: %f"), BoneLocation2.X));

			// Check if the vertex is on the right side of the bone
			if (VertexLocation.X > BoneLocation2.X)
			{
				OutVertexIndices.Add(VertexIndex);
			}
		}
	}


}

void ABaseZombie::CreateProceduralMeshFromBoneVertices(USkeletalMeshComponent* SkeletalMeshComp, FName BoneName, UProceduralMeshComponent* ProceduralMeshComp)
{
	TArray<int32> InfluencedVertices;
	ProceduralMeshComp->AttachToComponent(SkeletalMeshComp, FAttachmentTransformRules::KeepRelativeTransform);
	ProceduralMeshComp->RegisterComponent();

	GetBoneInfluencedVertices(SkeletalMeshComp, BoneName, InfluencedVertices);

	if (InfluencedVertices.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No vertices influenced by bone: %s"), *BoneName.ToString());
		return;
	}

	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;

	FSkeletalMeshRenderData* RenderData = SkeletalMeshComp->GetSkeletalMeshRenderData();
	if (!RenderData) return;

	const FSkeletalMeshLODRenderData& LODRenderData = RenderData->LODRenderData[0];
	const FPositionVertexBuffer& PositionVertexBuffer = LODRenderData.StaticVertexBuffers.PositionVertexBuffer;
	const FStaticMeshVertexBuffer& StaticMeshVertexBuffer = LODRenderData.StaticVertexBuffers.StaticMeshVertexBuffer;

	for (int32 VertexIdx : InfluencedVertices)
	{
		auto Position = PositionVertexBuffer.VertexPosition(VertexIdx);
		Vertices.Add(FVector(Position.X, Position.Y, Position.Z));

		FVector4f Normal4f = LODRenderData.StaticVertexBuffers.StaticMeshVertexBuffer.VertexTangentZ(VertexIdx);
		FVector Normal(Normal4f.X, Normal4f.Y, Normal4f.Z);
		Normals.Add(Normal);

		FVector2f UVf = LODRenderData.StaticVertexBuffers.StaticMeshVertexBuffer.GetVertexUV(VertexIdx, 0);
		UVs.Add(FVector2D(UVf.X, UVf.Y));
	}

	// Create triangles (indices)
	for (int32 i = 0; i < Vertices.Num() - 2; i += 3)
	{
		Triangles.Add(i);
		Triangles.Add(i + 1);
		Triangles.Add(i + 2);
	}

	ProceduralMeshComp->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
	ProceduralMeshComp->SetSimulatePhysics(true);
}




//void ABaseZombie::CreativeProceduralMesh(FName bonename)
//{
//	USkeletalMeshComponent* Skeleton = GetMesh();
//	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("CreativeProceduralMesh: 1")));
//	if (!Skeleton) {
//		return;
//	}
//	
//
//	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("CreativeProceduralMesh: 2")));
//
//	int32 BoneIndex = Skeleton->GetBoneIndex(bonename);
//
//	CutProceduralMesh = NewObject<UProceduralMeshComponent>(this);
//	if (!CutProceduralMesh) {
//		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("CreativeProceduralMesh: 0")));
//		return;
//	}
//
//	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("CreativeProceduralMesh: 3")));
//	CutProceduralMesh->AttachToComponent(Skeleton, FAttachmentTransformRules::KeepRelativeTransform);
//
//
//	TArray<FVector> Vertices;
//	//TArray<int32> Indices;
//	TArray<int32> Triangles;
//
//	if (!CopyStaticMesh) {
//		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("CopyStaticMesh = nullptr")));
//		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("CopyStaticMesh = nullptr")));
//		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("CopyStaticMesh = nullptr")));
//
//		return;
//	}
//	UStaticMesh* StaticMesh = CopyStaticMesh->GetStaticMesh();
//
//	if (!StaticMesh) {
//		return;
//	}
//
//	const FStaticMeshLODResources& LODResource = StaticMesh->GetRenderData()->LODResources[0];
//
//	for (uint32 Index = 0; Index < LODResource.VertexBuffers.PositionVertexBuffer.GetNumVertices(); ++Index)
//	{
//		FVector3f vecf3 = LODResource.VertexBuffers.PositionVertexBuffer.VertexPosition(Index);
//		FVector EachVector;
//
//		EachVector.X = vecf3.X;
//		EachVector.Y = vecf3.Y;
//		EachVector.Z = vecf3.Z;
//
//		Vertices.Add(EachVector);
//	}
//
//
//	//for (int32 Index = 0; Index < StaticMesh->GetRenderData()->LODResources[0].IndexBuffer.GetNumIndices(); Index += 3)
//	//{
//	//    int32 Index0 = StaticMesh->GetRenderData()->LODResources[0].IndexBuffer.GetIndex(Index);
//	//    int32 Index1 = StaticMesh->GetRenderData()->LODResources[0].IndexBuffer.GetIndex(Index + 1);
//	//    int32 Index2 = StaticMesh->GetRenderData()->LODResources[0].IndexBuffer.GetIndex(Index + 2);
//
//	//    Triangles.Add(Index0);
//	//    Triangles.Add(Index1);
//	//    Triangles.Add(Index2);
//	//}
//
//	for (int32 Index = 0; Index < LODResource.IndexBuffer.GetNumIndices(); ++Index)
//	{
//		auto index = LODResource.IndexBuffer.GetIndex(Index);
//		int32 Eachint32;
//		Eachint32 = index;
//		Triangles.Add(Eachint32);
//	}
//
//
//	TArray<FVector> Normals;
//	TArray<FVector2D> UVs;
//
//
//
//
//
//	const FStaticMeshVertexBuffer& VertexBuffer = LODResource.VertexBuffers.StaticMeshVertexBuffer;
//
//	for (uint32 Index = 0; Index < VertexBuffer.GetNumVertices(); ++Index)
//	{
//		FVector4f Normal4f = VertexBuffer.VertexTangentZ(Index);
//		FVector Normal(Normal4f.X, Normal4f.Y, Normal4f.Z);
//		Normals.Add(Normal);
//	}
//
//	for (uint32 Index = 0; Index < VertexBuffer.GetNumVertices(); ++Index)
//	{
//		FVector2f UVf = VertexBuffer.GetVertexUV(Index, 0); 
//
//		FVector2D UV;
//
//		UV.X = UVf.X;
//		UV.Y = UVf.Y;
//
//		UVs.Add(UV);
//	}
//
//
//
//	CutProceduralMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
//
//	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("CreativeProceduralMesh: 4")));
//	CutProceduralMesh->SetSimulatePhysics(true);
//
//	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("CreativeProceduralMesh: 5")));
//}


void ABaseZombie::StartAITree()
{
	if (GetZombieName() == "NormalZombie") {
		AZombieAIController* NormalZombieAIController = Cast<AZombieAIController>(GetController());

		//NormalZombieAIController->StartAI();
	}
	else if (GetZombieName() == "RunningZombie") {
		ARunningZombieAIController* RunningZombieAIController = Cast<ARunningZombieAIController>(GetController());

		//RunningZombieAIController->StartAI();
	}
	else if (GetZombieName() == "ShoutingZombie") {
		AShoutingZombieAIController* ShoutingZombieAIController = Cast<AShoutingZombieAIController>(GetController());

		//ShoutingZombieAIController->StartAI();
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

	StartResurrectionTimer();

	StopAITree();
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

	//if (!IsDie()) {
	//	StopAITree();
	//}
}

void ABaseZombie::AttackMontageEnded(UAnimMontage* Montage, bool interrup)
{
	m_bIsAttacking = false;

	m_DAttackEnd.Broadcast();

	if (!IsDie()) {
		StartAITree();
	}
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

		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Hit Actor")));
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

	FVector SpawnOffset = FVector (GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 5.0f);
	
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
	GetCapsuleComponent()->SetCollisionProfileName("Zombie");
	SetHP(GetStartHP());

	SetDie(true);
	StartAITree();
}

void ABaseZombie::StartAttackedStunHandle()
{
	StopAITree();
	GetWorld()->GetTimerManager().SetTimer(AttakcedStunHandle, this, &ABaseZombie::AttackedStunTimerElapsed, 2.0f, false);
}

void ABaseZombie::AttackedStunTimerElapsed()
{
	StartAITree();
}

void ABaseZombie::UpdateLastKnownPositionByFootSound(FVector playerlocation)
{
	if (GetController()) {
		if (GetZombieName() == "NormalZombie") {
			AZombieAIController* AIZombieController = Cast<AZombieAIController>(GetController());
			if (AIZombieController) {
				AIZombieController->UpdateLastKnownPositionByFootSound(playerlocation);
			}
		}
		else if (GetZombieName() == "RunningZombie") {
			ARunningZombieAIController* AIRunningZombieController = Cast<ARunningZombieAIController>(GetController());
			if (AIRunningZombieController) {
				AIRunningZombieController->UpdateLastKnownPositionByFootSound(playerlocation);
			}
		}
		else if (GetZombieName() == "ShoutingZombie") {
			AShoutingZombieAIController* AIShoutingZombieController = Cast<AShoutingZombieAIController>(GetController());
			if (AIShoutingZombieController) {
				AIShoutingZombieController->UpdateLastKnownPositionByFootSound(playerlocation);
			}
		}
	}


}

void ABaseZombie::UpdateLastKnownPositionByShoutingSound(FVector playerlocation)
{
	if (GetController()) {
		if (GetZombieName() == "NormalZombie") {
			AZombieAIController* AIZombieController = Cast<AZombieAIController>(GetController());
			if (AIZombieController) {
				AIZombieController->UpdateLastKnownPositionByFootSound(playerlocation);
			}
		}
		else if (GetZombieName() == "RunningZombie") {
			ARunningZombieAIController* AIRunningZombieController = Cast<ARunningZombieAIController>(GetController());
			if (AIRunningZombieController) {
				AIRunningZombieController->UpdateLastKnownPositionByFootSound(playerlocation);
			}
		}
		else if (GetZombieName() == "ShoutingZombie") {
			AShoutingZombieAIController* AIShoutingZombieController = Cast<AShoutingZombieAIController>(GetController());
			if (AIShoutingZombieController) {
				AIShoutingZombieController->UpdateLastKnownPositionByFootSound(playerlocation);
			}
		}
	}
}





