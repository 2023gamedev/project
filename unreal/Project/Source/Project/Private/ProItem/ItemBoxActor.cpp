// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/ItemBoxActor.h"
#include "ProItem/NormalWeaponActor.h"
#include "ProCharacter/BaseCharacter.h"

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
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BOX(TEXT("/Game/Mesh/SM_ItemBox.SM_ItemBox"));
	if (SM_BOX.Succeeded()) {
		Box->SetStaticMesh(SM_BOX.Object);
	}

	Box->SetRelativeLocation(FVector(0.f, -3.5, -30.f));

	Trigger->SetCollisionProfileName(TEXT("ItemBoxPS"));
	Box->SetCollisionProfileName(TEXT("NoCollision"));

	//NormalWeaponItemClass = ANormalWeaponActor::StaticClass();
	//m_sItemID = "None";
}

void AItemBoxActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemBoxActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void AItemBoxActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemBoxActor::OnChracterOvelapNew(ABaseCharacter* character)
{
	auto BaseCharacter = Cast<ABaseCharacter>(character);

	UE_LOG(LogTemp, Error, TEXT("AItemBoxActor::OnCharacterOverlap()"));
}

void AItemBoxActor::SetItemBoxId(uint32 index)
{
	ItemBoxId = index;
}

uint32 AItemBoxActor::GetItemBoxId()
{
	return ItemBoxId;
}


