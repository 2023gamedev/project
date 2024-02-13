// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBoxActor.h"
#include "NormalWeaponActor.h"
#include "BaseCharacter.h"

// Sets default values
AItemBoxActor::AItemBoxActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));

	RootComponent = Trigger;
	Box->SetupAttachment(RootComponent);

	Trigger->SetBoxExtent(FVector(30.f, 42.f, 30.f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BOX(TEXT("/Game/StarterContent/Shapes/Shape_Pipe.Shape_Pipe"));
	if (SM_BOX.Succeeded()) {
		Box->SetStaticMesh(SM_BOX.Object);
	}

	Box->SetRelativeLocation(FVector(0.f, -3.5, -30.f));

	Trigger->SetCollisionProfileName(TEXT("ItemBoxPS"));

	NormalWeaponItemClass = ANormalWeaponActor::StaticClass();
	m_sItemID = "None";
}

// Called when the game starts or when spawned
void AItemBoxActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemBoxActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AItemBoxActor::OnCharacterOverlap);

	UE_LOG(LogTemp, Error, TEXT("AItemBoxActor::PostInitializeComponents()"));
}

// Called every frame
void AItemBoxActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemBoxActor::OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto BaseCharacter = Cast<ABaseCharacter>(OtherActor);

	UE_LOG(LogTemp, Error, TEXT("AItemBoxActor::OnCharacterOverlap()"));
	if (BaseCharacter != nullptr && NormalWeaponItemClass != nullptr) {
		auto NewWeapon = GetWorld()->SpawnActor<ANormalWeaponActor>(NormalWeaponItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
		BaseCharacter->SetWeapon(NewWeapon);
		UE_LOG(LogTemp, Error, TEXT("wEAPONZononononojnZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ"));
	}
	else {

	}
}

