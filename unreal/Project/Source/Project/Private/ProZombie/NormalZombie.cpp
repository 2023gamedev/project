// Fill out your copyright notice in the Description page of Project Settings.


#include "ProZombie/NormalZombie.h"
#include "ProZombie/ZombieAIController.h"
#include "ProZombie/ZombieAnimInstance.h"


ANormalZombie::ANormalZombie()
{
	
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.f), FRotator(0.f, -90.f, 0.f));

	//CopyStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CopyStaticMesh"));
	
	
	// 위에 FObjectFinder는 기존것 밑에는 새롭게 애니메이션한 부분
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("/Game/ZombieAsset/NormalZombie/NormalZombie.NormalZombie"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("/Game/ZombieAsset/TNormalZombie/NormalZombie.NormalZombie"));
	if (SK_MANNEQUIN.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_MANNEQUIN.Object);
	}



	//static ConstructorHelpers::FClassFinder<UAnimInstance> ZOMBIE_ANIM(TEXT("/Game/ZombieAsset/Animation/BP_NormalZombieAnimBlueprint.BP_NormalZombieAnimBlueprint_C"));
	static ConstructorHelpers::FClassFinder<UAnimInstance> ZOMBIE_ANIM(TEXT("/Game/ZombieAsset/TAnimation/BP_NormalZombieAnimBlueprintT.BP_NormalZombieAnimBlueprintT_C"));
	if (ZOMBIE_ANIM.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(ZOMBIE_ANIM.Class);
	}
	
	//ConstructorHelpers::FObjectFinder<UStaticMesh> SM_NORMALSTATIC(TEXT("/Game/ZombieAsset/NormalZombie/NormalZombieStaticMesh.NormalZombieStaticMesh"));
	//if (SM_NORMALSTATIC.Succeeded()) {
	//	CopyStaticMesh->SetStaticMesh(SM_NORMALSTATIC.Object);
	//}
	//


	SetHP(NormalZombieStartHP);
	m_fHP_Prev = NormalZombieStartHP;
	SetStartHP(NormalZombieStartHP);
	float speed = NormalZombieSpeed;
	SetSpeed(speed);
	GetCharacterMovement()->MaxWalkSpeed = NormalZombieSpeed;
	SetSTR(FMath::RandRange(3, 6));
	SetSpecialAbility(false);
	SetZombieName("NormalZombie");
	SetTurningSpeed(GetTurningSpeed());
	targetType = TARGET::PATROL;

	// 좀비 크기(scale) 재설정
	float zombieScale = 0.97f;
	SetActorScale3D(FVector(zombieScale, zombieScale, zombieScale));
	// 기존 캡슐 크기를 가져오기
	float CurrentRadius = GetCapsuleComponent()->GetUnscaledCapsuleRadius();
	float CurrentHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	//  캡슐 크기도 동일한 비율로 변경
	GetCapsuleComponent()->SetCapsuleSize(CurrentRadius * zombieScale, CurrentHalfHeight * zombieScale);
}

void ANormalZombie::BeginPlay()
{
	Super::BeginPlay();

	CachedAnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());
}

void ANormalZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//auto CharactorAnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());
	//if (nullptr != CharactorAnimInstance) {
	//	CharactorAnimInstance->SetCurrentPawnSpeed(GetVelocity().Size());
	//}

	//if (OldLocation != FVector(0.0f, 0.0f, 0.0f)) {
	//	float DistanceMoved = FVector::Dist(OldLocation, NewLocation);
	//	Speed = (DeltaTime > 0) ? (DistanceMoved / DeltaTime) : 0;
	//	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("zombie speed: %f"), Speed));
	//}

	// 애니메이션 인스턴스에 속도 파라미터 설정
	//if ((Speed != 0 && PreviousSpeed == 0) || (Speed == 0 && PreviousSpeed != 0))
	//{
	//	if (CachedAnimInstance) {
	//		CachedAnimInstance->SetCurrentPawnSpeed(Speed);
	//	}
	//}

	//PreviousSpeed = Speed;
	//OldLocation = NewLocation;

	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("zombie #%d's speed: %f"), this->GetZombieId(), Speed));
}

void ANormalZombie::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

}


