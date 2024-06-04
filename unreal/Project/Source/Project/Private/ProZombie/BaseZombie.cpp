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

#include "Rendering/SkeletalMeshRenderData.h"
#include "Rendering/SkeletalMeshLODRenderData.h"
#include "Rendering/SkeletalMeshLODModel.h"
#include "Kismet/GameplayStatics.h"
#include "ProceduralMeshComponent.h"

#include "ProZombie/ZombieAnimInstance.h"

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

	ZombieId = 0;
	
	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceConsiderationRadius = 400.f;
}

// Called when the game starts or when spawned
void ABaseZombie::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto CharacterAnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr != CharacterAnimInstance) {
		CharacterAnimInstance->SetCurrentPawnSpeed(GetVelocity().Size());
	}


}

void ABaseZombie::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// ���� �ʿ� ������ animinstance�� �ʿ��� �� ����.
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

float ABaseZombie::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
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

		NormalZombieAIController->StopAI();

	}
	else if (GetZombieName() == "RunningZombie") {
		ARunningZombieAIController* RunningZombieAIController = Cast<ARunningZombieAIController>(GetController());

		RunningZombieAIController->StopAI();
	}
	else if (GetZombieName() == "ShoutingZombie") {
		AShoutingZombieAIController* ShoutingZombieAIController = Cast<AShoutingZombieAIController>(GetController());

		ShoutingZombieAIController->StopAI();
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

		// ProceduralMeshComponent ���� �� ����
		CutProceduralMesh = NewObject<UProceduralMeshComponent>(this);
		if (!CutProceduralMesh) return;

		// CreativeProceduralMesh(bonename);
		// CreateProceduralMeshFromBoneVertices(GetMesh(), bonename, CutProceduralMesh);

		Skeleton->HideBoneByName(bonename, EPhysBodyOp::PBO_None);

	}
}


void ABaseZombie::CreativeProceduralMesh(FName bonename)
{

	USkeletalMeshComponent* Skeleton = GetMesh();
	if (!Skeleton) return;

	int32 BoneIndex = Skeleton->GetBoneIndex(bonename);
	if (BoneIndex == INDEX_NONE) return; // ������ bone�� ���� ��� �ߴ�

	// ProceduralMeshComponent ���� �� ����
	CutProceduralMesh = NewObject<UProceduralMeshComponent>(this);
	if (!CutProceduralMesh) return;

	CutProceduralMesh->AttachToComponent(Skeleton, FAttachmentTransformRules::KeepRelativeTransform);
	CutProceduralMesh->bUseComplexAsSimpleCollision = false;
	CutProceduralMesh->SetSimulatePhysics(true);
	CutProceduralMesh->SetEnableGravity(true);
	CutProceduralMesh->AddForce(FVector(0.f, 0.f, 1000.f));
	CutProceduralMesh->SetCollisionProfileName("BlockAllDynamic");
	CutProceduralMesh->RegisterComponent();

	// StaticMesh ������ ����
	if (!CopyStaticMesh)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("CopyStaticMesh = nullptr"));
		return;
	}

	UStaticMesh* StaticMesh = CopyStaticMesh->GetStaticMesh();
	if (!StaticMesh) return;

	const FStaticMeshLODResources& LODResource = StaticMesh->GetRenderData()->LODResources[0];

	// ���ܵ� �κ� Ȯ��
	FVector CutLocation = Skeleton->GetBoneLocation(bonename);
	FVector CutDirection = FVector(0.0f, 0.0f, 1.0f); // x�� �������� ���ܵ� �κи� ����
	FVector CutLocation2 = Skeleton->GetBoneLocation(bonename, EBoneSpaces::ComponentSpace);

	// ���ؽ� ������ ����
	TArray<FVector> Vertices;

	uint32 LODResourcePosNumVetices = LODResource.VertexBuffers.PositionVertexBuffer.GetNumVertices();

	if (LODResourcePosNumVetices > 0) {
		for (uint32 Index = 0; Index < LODResourcePosNumVetices; ++Index)
		{
			FVector3f VP3f = LODResource.VertexBuffers.PositionVertexBuffer.VertexPosition(Index);
			FVector VertexPosition = FVector(VP3f.X, VP3f.Y, VP3f.Z);

			// ���ܵ� �κ� Ȯ��
			if (FVector::DotProduct(VertexPosition - CutLocation2, CutDirection) > 0)
			{
				Vertices.Add(VertexPosition);
			}
		}
	}



	//// ���ʰ� ������ ���ܵ� �κ��� ���ؽ� �и�
	//TArray<FVector> LeftVertices;
	//TArray<FVector> RightVertices;
	//for (const FVector& Vertex : Vertices)
	//{
	//	if (Vertex.X < CutLocation2.X)
	//	{
	//		LeftVertices.Add(Vertex);
	//	}
	//	else
	//	{
	//		RightVertices.Add(Vertex);
	//	}
	//}

	//// ���ܵ� �κ��� ���ʰ� ������ ���ؽ��� ���� �̾ ����մϴ�.
	//Vertices = LeftVertices;
	//Vertices.Append(RightVertices);

	// �ε��� ������ ����
	TArray<int32> Triangles;
	const int32 NumIndices = LODResource.IndexBuffer.GetListIndex();
	if (NumIndices > 0) {
		for (int32 Index = 0; Index < NumIndices; ++Index)
		{
			// ���ؽ��� �ε����� �����ɴϴ�.
			int32 VertexIndex = LODResource.IndexBuffer.GetIndex(Index);

			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Invalid VertexIndex: %d, Total Vertices: %d"), VertexIndex, LODResource.VertexBuffers.PositionVertexBuffer.GetListIndex()));

			// �ε����� ������ ����� �ʴ��� Ȯ��
			if (VertexIndex < 0 || VertexIndex >= static_cast<int32>(LODResource.VertexBuffers.PositionVertexBuffer.GetListIndex()))
			{
				// ��ȿ���� ���� �ε��� ���� �� �α� ���
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Invalid VertexIndex: %d, Total Vertices: %d"), VertexIndex, LODResource.VertexBuffers.PositionVertexBuffer.GetListIndex()));
				break;
			}

			// �ش� ���ؽ��� ��ġ�� �����ɴϴ�.
			FVector3f VP3f = LODResource.VertexBuffers.PositionVertexBuffer.VertexPosition(VertexIndex);
			FVector VertexPosition = FVector(VP3f.X, VP3f.Y, VP3f.Z);

			// ���ؽ� �迭���� �ش� ���ؽ��� �ε����� ã���ϴ�.
			for (int32 VertIndex = 0; VertIndex < Vertices.Num(); ++VertIndex)
			{
				// ���ؽ��� ��ġ�� �ε��� ���ۿ� �ִ� ��ġ�� ���ٸ� �ش� �ε����� �߰��մϴ�.
				if (Vertices[VertIndex].Equals(VertexPosition, KINDA_SMALL_NUMBER))
				{
					Triangles.Add(VertIndex);
				}
			}
		}
	}


	//// �ε��� ������ ����
	//TArray<int32> Triangles;

	//// ���ؽ� �迭�� ����Ͽ� �ﰢ���� �����մϴ�.
	const int32 NumVertices = Vertices.Num();
	//if (NumVertices >= 3) {
	//	for (int32 Index = 0; Index < NumVertices - 2; ++Index)
	//	{
	//		// ù ��° ���ؽ��� �ε���
	//		Triangles.Add(Index);
	//		// �� ��° ���ؽ��� �ε���
	//		Triangles.Add(Index + 1);
	//		// �� ��° ���ؽ��� �ε���
	//		Triangles.Add(Index + 2);
	//	}

	//	// ���� �� ���� ���ؽ��� ������ �ﰢ���� �����մϴ�.
	//	if (NumVertices % 3 != 0) {
	//		Triangles.Add(NumVertices - 3);
	//		Triangles.Add(NumVertices - 2);
	//		Triangles.Add(NumVertices - 1);
	//	}
	//}

	// Normal �� UV ������ ����
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	const FStaticMeshVertexBuffer& VertexBuffer = LODResource.VertexBuffers.StaticMeshVertexBuffer;

	for (uint32 Index = 0; Index < (uint32)NumVertices; ++Index)
	{
		FVector4f Normal4f = VertexBuffer.VertexTangentZ(Index);
		Normals.Add(FVector(Normal4f.X, Normal4f.Y, Normal4f.Z));
	}

	for (uint32 Index = 0; Index < (uint32)NumVertices; ++Index)
	{
		FVector2f UVf = VertexBuffer.GetVertexUV(Index, 0);
		UVs.Add(FVector2D(UVf.X, UVf.Y));
	}




	if (Vertices.Num() > 0 && Triangles.Num() > 0)
	{

		// Mesh ����
		CutProceduralMesh->CreateMeshSection(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), true);
		//CutProceduralMesh->CreateMeshSection(1, Vertices, Triangles, Normals, UVs, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
		//CutProceduralMesh->CreateMeshSection(2, Vertices, Triangles, Normals, UVs, TArray<FColor>(), TArray<FProcMeshTangent>(), true);

		// ���͸��� �ν��Ͻ� ���� �� ����
		if (Material)
		{
			UMaterialInstanceDynamic* MaterialInstance = UMaterialInstanceDynamic::Create(Material, this);
			CutProceduralMesh->SetMaterial(0, MaterialInstance);
		}

		if (Material2)
		{
			UMaterialInstanceDynamic* MaterialInstance2 = UMaterialInstanceDynamic::Create(Material2, this);
			CutProceduralMesh->SetMaterial(1, MaterialInstance2);
		}

		if (Material3)
		{
			UMaterialInstanceDynamic* MaterialInstance3 = UMaterialInstanceDynamic::Create(Material3, this);
			CutProceduralMesh->SetMaterial(2, MaterialInstance3);
		}

	}
	//float Mass = 1.0f; // ���÷� 100.0f�� ����
	//CutProceduralMesh->GetMass();
	//CutProceduralMesh->SetMassOverrideInKg(NAME_None, Mass, true);

	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("GetMass = %f"), CutProceduralMesh->GetMass()));


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


			// ����� �޽����� X ��ǥ ���
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

		NormalZombieAIController->StartAI();

	}
	else if (GetZombieName() == "RunningZombie") {
		ARunningZombieAIController* RunningZombieAIController = Cast<ARunningZombieAIController>(GetController());

		RunningZombieAIController->StartAI();
	}
	else if (GetZombieName() == "ShoutingZombie") {
		AShoutingZombieAIController* ShoutingZombieAIController = Cast<AShoutingZombieAIController>(GetController());

		ShoutingZombieAIController->StartAI();
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

void ABaseZombie::Attack()
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


	if (!IsDie()) {
		StopAITree();
	}

}

void ABaseZombie::AttackMontageEnded(UAnimMontage* Montage, bool interrup)
{
	m_bIsAttacking = false;

	m_DAttackEnd.Broadcast();

	if (!IsDie()) {
		StartAITree();
	}
}

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


	// debug ��(�浹 ���� Ȯ�� ��)
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
		FDamageEvent DamageEvent;
		HitResult.GetActor()->TakeDamage(GetSTR(), DamageEvent, GetController(), this);
	}
}

void ABaseZombie::Shouting()
{
	if (m_bIsShouting) {
		return;
	}
	auto AnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());


	AnimInstance->PlayShoutingMontage();


	UWorld* World = GetWorld();
	FVector Center = GetActorLocation();
	float DetectRadius = 2000.f;

	if (nullptr == World) return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	if (bResult) {

		for (const FOverlapResult& OverlapResult : OverlapResults)
		{
			// ABaseZombie���� Ȯ��
			ABaseZombie* OverlappedZombie = Cast<ABaseZombie>(OverlapResult.GetActor());
			if (OverlappedZombie)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("HEAR"));
				OverlappedZombie->UpdateLastKnownPositionByFootSound(GetActorLocation());
			}

		}
	}

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

	// ���� ��ο��� ������ ������
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





