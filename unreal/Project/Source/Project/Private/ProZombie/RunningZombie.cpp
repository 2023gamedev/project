// Fill out your copyright notice in the Description page of Project Settings.


#include "ProZombie/RunningZombie.h"
#include "ProZombie/RunningZombieAIController.h"
#include "ProZombie/ZombieAnimInstance.h"

ARunningZombie::ARunningZombie()
{

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.f), FRotator(0.f, -90.f, 0.f));

	CopyStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CopyStaticMeshR"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("/Game/ZombieAsset/RunningZombie/RunningZombie.RunningZombie"));
	if (SK_MANNEQUIN.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_MANNEQUIN.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> ZOMBIE_ANIM(TEXT("/Game/ZombieAsset/Animation/RunningZombieAnimation/BP_RunningZombieAnimBlueprint.BP_RunningZombieAnimBlueprint_C"));
	if (ZOMBIE_ANIM.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(ZOMBIE_ANIM.Class);
	}
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_RUNNINGSTATIC(TEXT("/Game/ZombieAsset/RunningZombie/RunningZombieStaticMesh.RunningZombieStaticMesh"));
	if (SM_RUNNINGSTATIC.Succeeded()) {
		CopyStaticMesh->SetStaticMesh(SM_RUNNINGSTATIC.Object);
	}

	
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialFinder(TEXT("/Game/ZombieAsset/RunningZombie/body_M.body_M"));
	if (MaterialFinder.Succeeded())
	{
		Material = MaterialFinder.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialFinder2(TEXT("/Game/ZombieAsset/RunningZombie/body_M1.body_M1"));
	if (MaterialFinder2.Succeeded())
	{
		Material2 = MaterialFinder2.Object;
	}




	SetHP(20);
	SetStartHP(20);
	SetSpeed(4);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	SetSTR(FMath::RandRange(4, 8)); 
	SetSpecialAbility(true);
	SetZombieName("RunningZombie");
	SetTurningSpeed(90.f);
}

void ARunningZombie::BeginPlay()
{
	Super::BeginPlay();

	CachedAnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());
}

void ARunningZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//auto CharactorAnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());
	//if (nullptr != CharactorAnimInstance) {
	//	CharactorAnimInstance->SetCurrentPawnSpeed(GetVelocity().Size());
	//}
}


void ARunningZombie::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}


