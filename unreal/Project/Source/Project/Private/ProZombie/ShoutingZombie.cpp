// Fill out your copyright notice in the Description page of Project Settings.


#include "ProZombie/ShoutingZombie.h"
#include "ProZombie/ShoutingZombieAIController.h"
#include "ProZombie/ZombieAnimInstance.h"

AShoutingZombie::AShoutingZombie()
{

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.f), FRotator(0.f, -90.f, 0.f));

	CopyStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CopyStaticMeshS"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("/Game/ZombieAsset/ShoutingZombie/ShoutingZombie.ShoutingZombie"));
	if (SK_MANNEQUIN.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_MANNEQUIN.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> ZOMBIE_ANIM(TEXT("/Game/ZombieAsset/Animation/ShoutingAnimation/BP_ShoutingZombieAnimBlueprint.BP_ShoutingZombieAnimBlueprint_C"));
	if (ZOMBIE_ANIM.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(ZOMBIE_ANIM.Class);
	}

	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SHOUTINGSTATIC(TEXT("/Game/ZombieAsset/ShoutingZombie/ShoutingZombieStaticMesh.ShoutingZombieStaticMesh"));
	if (SM_SHOUTINGSTATIC.Succeeded()) {
		CopyStaticMesh->SetStaticMesh(SM_SHOUTINGSTATIC.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialFinder(TEXT("/Game/ZombieAsset/ShoutingZombie/parasitezombie_Material.parasitezombie_Material"));
	if (MaterialFinder.Succeeded())
	{
		Material = MaterialFinder.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialFinder2(TEXT("/Game/ZombieAsset/ShoutingZombie/parasitezombie_Material1.parasitezombie_Material1"));
	if (MaterialFinder2.Succeeded())
	{
		Material2 = MaterialFinder2.Object;
	}



	SetHP(30);
	SetStartHP(30);
	SetSpeed(3);
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	SetSTR(FMath::RandRange(4, 8)); // 수정 필요 4 ~ 8
	SetSpecialAbility(true);
	SetZombieName("ShoutingZombie");
	SetTurningSpeed(135.f);
}

void AShoutingZombie::BeginPlay()
{
	Super::BeginPlay();
}

void AShoutingZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto CharactorAnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr != CharactorAnimInstance) {
		CharactorAnimInstance->SetCurrentPawnSpeed(GetVelocity().Size());
	}
}


void AShoutingZombie::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}