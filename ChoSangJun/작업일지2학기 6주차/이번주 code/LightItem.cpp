// Fill out your copyright notice in the Description page of Project Settings.


#include "LightItem.h"

// Sets default values
ALightItem::ALightItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));

	RootComponent = Trigger;
	Mesh->SetupAttachment(Trigger);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BOX(TEXT("/Game/Safe_House/meshes/SM_auto_repair_03_compiled_04.SM_auto_repair_03_compiled_04"));

	if (SM_BOX.Succeeded()) {
		Mesh->SetStaticMesh(SM_BOX.Object);
	}

	Mesh->SetRelativeLocation(FVector(0.0f, -3.5f, -30.f));

}

// Called when the game starts or when spawned
void ALightItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALightItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

