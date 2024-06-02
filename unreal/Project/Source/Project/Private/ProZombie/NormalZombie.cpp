// Fill out your copyright notice in the Description page of Project Settings.


#include "ProZombie/NormalZombie.h"
#include "ProZombie/ZombieAIController.h"
#include "ProZombie/ZombieAnimInstance.h"


ANormalZombie::ANormalZombie()
{
	
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.f), FRotator(0.f, -90.f, 0.f));

	CopyStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CopyStaticMesh"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("/Game/ZombieAsset/NormalZombie/NormalZombie.NormalZombie"));
	if (SK_MANNEQUIN.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_MANNEQUIN.Object);
	}



	static ConstructorHelpers::FClassFinder<UAnimInstance> ZOMBIE_ANIM(TEXT("/Game/ZombieAsset/Animation/BP_NormalZombieAnimBlueprint.BP_NormalZombieAnimBlueprint_C"));
	if (ZOMBIE_ANIM.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(ZOMBIE_ANIM.Class);
	}
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> SM_NORMALSTATIC(TEXT("/Game/ZombieAsset/NormalZombie/NormalZombieStaticMesh.NormalZombieStaticMesh"));
	if (SM_NORMALSTATIC.Succeeded()) {
		CopyStaticMesh->SetStaticMesh(SM_NORMALSTATIC.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialFinder(TEXT("/Game/ZombieAsset/NormalZombie/ZombieGirl_body_Material.ZombieGirl_body_Material"));
	if (MaterialFinder.Succeeded())
	{
		Material = MaterialFinder.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialFinder2(TEXT("/Game/ZombieAsset/NormalZombie/ZombieGirl_body_Material.ZombieGirl_body_Material"));
	if (MaterialFinder2.Succeeded())
	{
		Material2 = MaterialFinder2.Object;
	} 
	
	static ConstructorHelpers::FObjectFinder<UMaterial> MaterialFinder3(TEXT("/Game/ZombieAsset/NormalZombie/ZombieGirl_Material_ncl1_1.ZombieGirl_Material_ncl1_1"));
	if (MaterialFinder3.Succeeded())
	{
		Material3 = MaterialFinder3.Object;
	}


	SetHP(20);
	SetStartHP(20);
	SetSpeed(2);
	GetCharacterMovement()->MaxWalkSpeed = 200.f;
	SetSTR(FMath::RandRange(4, 8));
	SetSpecialAbility(false);
	SetZombieName("NormalZombie");
}

void ANormalZombie::BeginPlay()
{
	Super::BeginPlay();
}

void ANormalZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto CharactorAnimInstance = Cast<UZombieAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr != CharactorAnimInstance) {
		CharactorAnimInstance->SetCurrentPawnSpeed(GetVelocity().Size());
	}
}

void ANormalZombie::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

}


