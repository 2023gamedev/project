// Fill out your copyright notice in the Description page of Project Settings.


#include "ProItem/NormalWeaponActor.h"
#include "ProZombie/BaseZombie.h"
#include "Engine/DamageEvents.h" 
#include "ProCharacter/BaseCharacter.h"

ANormalWeaponActor::ANormalWeaponActor()
{
	//PrimaryActorTick.bCanEverTick = false;
}

void ANormalWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ANormalWeaponActor::WeaponBeginOverlap);
}

void ANormalWeaponActor::Tick(float DeltaTime)
{

}

void ANormalWeaponActor::PlaceItem()
{
}

void ANormalWeaponActor::WeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABaseZombie* Zombie = Cast<ABaseZombie>(OtherActor);
	if(Zombie){
		FDamageEvent DamageEvent;
		Zombie->TakeDamage(m_fCharacterSTR * m_fWeaponSTR, DamageEvent, GetInstigatorController(), this);
		BoxComponent->SetCollisionProfileName(TEXT("NoCollision"));
		--m_fWeaponDurability;
		if (m_fWeaponDurability <= 0) {
			OwnerCharacter->DestroyNormalWepaonItemSlot();
		}
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Damaged!"));
	}
}



