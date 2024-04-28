// Fill out your copyright notice in the Description page of Project Settings.


#include "ProCharacter/BaseCharacter.h"
#include "ProCharacter/PlayerCharacterAnimInstance.h"
#include "ProItem/ItemActor.h"
#include "ProItem/ItemBoxActor.h"
#include "ProCharacter/PlayerSight.h"

// 노말 무기
#include "ProItem/NormalWeaponActor.h"
#include "ProItem/NWBook.h"
#include "ProItem/NWBottle.h"
#include "ProItem/NWButchersKnife.h"
#include "ProItem/NWFireAxe.h"
#include "ProItem/NWFireExtinguisher.h"
#include "ProItem/NWFryingPan.h"
#include "ProItem/NWGolfClub.h"
#include "ProItem/NWIron.h"
#include "ProItem/NWMagicStick.h"
#include "ProItem/NWMannequinArm.h"
#include "ProItem/NWMannequinLeg.h"
#include "ProItem/NWPipe.h"
#include "ProItem/NWPlunger.h"
#include "ProItem/NWSashimiKnife.h"
#include "ProItem/NWScissors.h"
#include "ProItem/NWShovels.h"
#include "ProItem/NWSquareWood.h"
#include "ProItem/NWWoodenBat.h"

// 투척 무기
#include "ProItem/ThrowWeaponActor.h"

// 상처회복 아이템
#include "ProItem/HealingItemActor.h"
#include "ProItem/HCannedTuna.h"
#include "ProItem/HDisinfectant.h"
#include "ProItem/HDrink.h"
#include "ProItem/HOintment.h"
#include "ProItem/HSmoke.h"
#include "ProItem/HSnack.h"
#include "ProItem/HWater.h"

// 키 아이템
#include "ProItem/KeyActor.h"
#include "ProItem/KCarkey1.h"
#include "ProItem/KCarkey2.h"
#include "ProItem/KCarkey3.h"
#include "ProItem/KRoofKey1.h"
#include "ProItem/KRoofKey2.h"


// 출혈회복 아이템
#include "ProItem/BleedingHealingItemActor.h"
#include "ProItem/BHBandage.h"
#include "ProItem/BHClothes.h"
#include "ProItem/BHDirtyClothes.h"
#include "ProItem/BHGauze.h"


#include "ProUI/GamePlayerUI.h"
#include "ProUI/ConditionUI.h"
#include "ProCharacter/PlayerCharacterController.h"





// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	PlayerSight = CreateDefaultSubobject<UPlayerSight>(TEXT("PLAYERSIGHT"));
	CurrentWeapon = CreateDefaultSubobject<ANormalWeaponActor>(TEXT("NORMALWEAPON"));
	FlashLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("FLASHLIGHT"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	
	Camera->SetupAttachment(SpringArm);
	FlashLight->SetupAttachment(SpringArm);
	PlayerSight->SetupAttachment(Camera);
	PlayerSight->SetRelativeLocation(FVector(150.f, 0.f, 88.f));
	
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.f), FRotator(0.f, -90.f, 0.f));
	SpringArm->TargetArmLength = 400.f;
	SpringArm->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("/Game/CharacterAsset/EmployeeCharacter.EmployeeCharacter"));

	if (SK_MANNEQUIN.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_MANNEQUIN.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> CHARACTER_ANIM(TEXT("/Game/CharacterAsset/Animation/BP_EmployeeAnimBlueprint.BP_EmployeeAnimBlueprint_C"));

	if (CHARACTER_ANIM.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(CHARACTER_ANIM.Class);
	}

	static ConstructorHelpers::FClassFinder <UGamePlayerUI> PLAYER_GAMEUI(TEXT("/Game/UI/BP_GamePlayerUI.BP_GamePlayerUI_C"));

	if (PLAYER_GAMEUI.Succeeded()) {
		GameUIClass = PLAYER_GAMEUI.Class;
	}


	static ConstructorHelpers::FClassFinder <UConditionUI> PLAYER_CONDITIONUI(TEXT("/Game/UI/Condition.Condition_C"));

	if (PLAYER_CONDITIONUI.Succeeded()) {
		ConditionUIClass = PLAYER_CONDITIONUI.Class;
	}

	SpringArm->TargetArmLength = 300.f;
	SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bDoCollisionTest = true;
	bUseControllerRotationYaw = false;

	// 스포트 라이트의 속성 설정 (위치, 회전 등)
	FlashLight->SetRelativeLocationAndRotation(FVector(303.f, -24.f, 117.f), FRotator( 0.f, 0.f, 0.f));
	// 기타 설정 (색상, 강도 등)
	FlashLight->SetLightColor(FLinearColor::White);
	FlashLight->SetIntensity(5000.f);
	FlashLight->SetOuterConeAngle(60.f);
	FlashLight->SetVisibility(true);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 500.f;

	CurrentWeapon = nullptr;
	CurrentThrowWeapon = nullptr;
	CurrentBleedingHealingItem = nullptr;
	CurrentHealingItem = nullptr;
	CurrentKeyItem = nullptr;

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();


	if (GameUIClass != nullptr) {

		APlayerCharacterController* controller = Cast<APlayerCharacterController>(this->GetController());
		if (controller == nullptr) {
			return;
		}
		GameUIWidget = CreateWidget<UGamePlayerUI>(controller, GameUIClass);

		if (!GameUIWidget) {
			return;
		}

		GameUIWidget->Character = this;
		GameUIWidget->Init();
		GameUIWidget->AddToViewport();
		GameUIWidget->SetVisibility(ESlateVisibility::Hidden);

	}

	if (ConditionUIClass != nullptr) {

		APlayerCharacterController* controller = Cast<APlayerCharacterController>(this->GetController());
		if (controller == nullptr) {
			return;
		}
		
		ConditionUIWidget = CreateWidget<UConditionUI>(controller, ConditionUIClass);

		if (!ConditionUIWidget) {
			return;
		}

		ConditionUIWidget->Character = this;
		ConditionUIWidget->AddToViewport();
	}


	auto AnimInstance = Cast<UPlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());

	AnimInstance->OnMontageEnded.AddDynamic(this, &ABaseCharacter::AttackMontageEnded);

	AnimInstance->OnAttackStartCheck.AddLambda([this]() -> void {
		if (CurrentWeapon != nullptr) {
			CurrentWeapon->BoxComponent->SetCollisionProfileName(TEXT("WeaponItem"));
		}
		});

	AnimInstance->OnAttackStartCheck.AddLambda([this]() -> void {
		if (CurrentWeapon != nullptr) {
			CurrentWeapon->BoxComponent->SetCollisionProfileName(TEXT("NoCollision"));
		}
		});



}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!GetVelocity().Size()) {
		if (OldLocation != FVector(0.0f, 0.0f, 0.0f)) {
			float DistanceMoved = FVector::Dist(OldLocation, NewLocation);
			Speed = (DeltaTime > 0) ? (DistanceMoved / DeltaTime) : 0;
		}

		// 애니메이션 인스턴스에 속도 파라미터 설정
		if ((Speed != 0 && PreviousSpeed == 0) || (Speed == 0 && PreviousSpeed != 0))
		{
			auto AnimInstance = Cast<UPlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());
			if (AnimInstance) {
				AnimInstance->SetCurrentPawnSpeed(Speed);
			}
		}

		PreviousSpeed = Speed;
		OldLocation = NewLocation;
	}

	else {
		auto CharacterAnimInstance = Cast<UPlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());
		if (nullptr != CharacterAnimInstance) {
			CharacterAnimInstance->SetCurrentPawnSpeed(GetVelocity().Size());
			CharacterAnimInstance->SetIsPawnRun(m_bRun);
		}

		PreviousSpeed = GetVelocity().Size();
	}


}

void ABaseCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	SetHP(GetHP() - Damage);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Hit Character")));
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("HP %f"), GetHP()));
	
	if (GetHP() <= 0) {
		SetDead(true);
		auto CharacterAnimInstance = Cast<UPlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());
		if (nullptr != CharacterAnimInstance) {
			CharacterAnimInstance->SetCurrentPawnSpeed(GetVelocity().Size());
			CharacterAnimInstance->SetIsDead(IsDead());
		}
		APlayerCharacterController* controller = Cast<APlayerCharacterController>(this->GetController());
		if (controller != nullptr) {
			DisableInput(controller); // 이래도 움직임 좀비 죽었을때 회전이랑 캐릭터 움직이는 거 수정해야 할듯
		}
	}


	return Damage;
}

bool ABaseCharacter::DraggingSwap(int from, ESlotType fromtype, int to, ESlotType totype)
{
	if (fromtype == ESlotType::SLOT_ITEM && (fromtype == totype))
		return SwapInven(from, to);

	return true;
}

bool ABaseCharacter::SwapInven(int from, int to)
{
	if (from < 0 || to < 0) return false;
	
	Inventory.Swap(from, to);
	GameUIWidget->RefreshInventory(from);
	GameUIWidget->RefreshInventory(to);

	return true;
}

void ABaseCharacter::SpawnOnGround(int slotindex)
{

	auto CurrentInvenSlot = this->Inventory[slotindex];
	if (CurrentInvenSlot.Type == EItemType::ITEM_EQUIPMENT) {
		if (CurrentInvenSlot.ItemClassType == EItemClass::BLEEDINGHEALINGITEM) {
			QuickSlot[0].Type = EItemType::ITEM_QUICK_NONE;
			QuickSlot[0].Name = "nullptr";
			QuickSlot[0].ItemClassType = EItemClass::NONE;
			QuickSlot[0].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
			QuickSlot[0].Count = 0;
			QuickSlot[0].SlotReference = -1;
		}
		else if (CurrentInvenSlot.ItemClassType == EItemClass::HEALINGITEM) {
			QuickSlot[1].Type = EItemType::ITEM_QUICK_NONE;
			QuickSlot[1].Name = "nullptr";
			QuickSlot[1].ItemClassType = EItemClass::NONE;
			QuickSlot[1].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
			QuickSlot[1].Count = 0;
			QuickSlot[1].SlotReference = -1;
		}
		else if (CurrentInvenSlot.ItemClassType == EItemClass::THROWINGWEAPON) {
			QuickSlot[2].Type = EItemType::ITEM_QUICK_NONE;
			QuickSlot[2].Name = "nullptr";
			QuickSlot[2].ItemClassType = EItemClass::NONE;
			QuickSlot[2].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
			QuickSlot[2].Count = 0;
			QuickSlot[2].SlotReference = -1;
		}
		else if (CurrentInvenSlot.ItemClassType == EItemClass::KEYITEM) {
			QuickSlot[3].Type = EItemType::ITEM_QUICK_NONE;
			QuickSlot[3].Name = "nullptr";
			QuickSlot[3].ItemClassType = EItemClass::NONE;
			QuickSlot[3].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
			QuickSlot[3].Count = 0;
			QuickSlot[3].SlotReference = -1;
		}
		else if (CurrentInvenSlot.ItemClassType == EItemClass::NORMALWEAPON) {
			QuickSlot[4].Type = EItemType::ITEM_QUICK_NONE;
			QuickSlot[4].Name = "nullptr";
			QuickSlot[4].ItemClassType = EItemClass::NONE;
			QuickSlot[4].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
			QuickSlot[4].Count = 0;
			QuickSlot[4].SlotReference = -1;
		}

	}
	Inventory[slotindex].Type = EItemType::ITEM_NONE;
	Inventory[slotindex].Name = "nullptr";
	Inventory[slotindex].ItemClassType = EItemClass::NONE;
	Inventory[slotindex].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
	Inventory[slotindex].Count = 0;

	GameUIUpdate();

	ThrowOnGround.ExecuteIfBound(CurrentInvenSlot.Name, CurrentInvenSlot.ItemClassType, CurrentInvenSlot.Texture, CurrentInvenSlot.Count);
}




void ABaseCharacter::AttackMontageEnded(UAnimMontage* Montage, bool interrup)
{
	m_bIsAttacking = false;
	m_DAttackEnd.Broadcast();

	m_bIsPickUping = false;
	m_DPickUpEnd.Broadcast();
}



void ABaseCharacter::MoveForward(FVector RotateYaw, float NewAxisValue)
{
	if (m_bRun) {
		SetSpeed(500.f);
		
	}
	else {
		SetSpeed(200.f);
	}

	GetCharacterMovement()->MaxWalkSpeed = m_fSpeed;
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue * m_fSpeed * GetWorld()->DeltaTimeSeconds);
	AddMovementInput(RotateYaw, NewAxisValue * m_fSpeed * GetWorld()->DeltaTimeSeconds);

}

void ABaseCharacter::MoveLeft(FVector RotateYaw, float NewAxisValue)
{
	if (m_bRun) {
		SetSpeed(500.f);
	}
	else {
		SetSpeed(200.f);
	}

	GetCharacterMovement()->MaxWalkSpeed = m_fSpeed;
	//AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue * m_fSpeed * GetWorld()->DeltaTimeSeconds);
	AddMovementInput(RotateYaw, NewAxisValue * m_fSpeed * GetWorld()->DeltaTimeSeconds);
}

void ABaseCharacter::Run()
{
	if (m_bRun) {
		m_bRun = false;
	}
	else {
		m_bRun = true;
	}
}


void ABaseCharacter::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
}

void ABaseCharacter::Turn(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue);
}
// 수정 필요
void ABaseCharacter::GetItem()
{
	if (PlayerSight->GetIsHit()) {
		
		auto itembox = Cast<AItemBoxActor>(PlayerSight->GetHitActor());
		PickUp();
		// 아이템박스에 있는 아이템에 대한 정보를 가져온다.
		for (int i = 0; i < 20; ++i) {
			if (Inventory[i].Type == EItemType::ITEM_NONE) {
				Inventory[i].Type = EItemType::ITEM_USEABLE;
				Inventory[i].Name = itembox->ItemName;
				Inventory[i].ItemClassType = itembox->ItemClassType;
				Inventory[i].Texture = itembox->Texture;
				Inventory[i].Count = itembox->Count;
				GameUIWidget->Update();
				break;
			}
		}

		PlayerSight->GetHitActor()->Destroy();
	}

	UE_LOG(LogTemp, Warning, TEXT("GetItem"));
}

void ABaseCharacter::LightOnOff()
{
	if (IsSpotLight()) {
		FlashLight->SetVisibility(false);
		SetSpotLight(false);
	}
	else {
		FlashLight->SetVisibility(true);
		SetSpotLight(true);
	}
}

void ABaseCharacter::InventoryOnOff()
{
	// 작성 필요
	UE_LOG(LogTemp, Warning, TEXT("InvenOpen"));
	if (GameUIWidget != nullptr) {
		if (IsInventory()) {
			SetInventory(false);
			GameUIWidget->SetVisibility(ESlateVisibility::Hidden);

			APlayerCharacterController* controller = Cast<APlayerCharacterController>(this->GetController());
			controller->bShowMouseCursor = false;

			FInputModeGameOnly InputModeGameOnly;
			controller->SetInputMode(InputModeGameOnly);
		}
		else {
			SetInventory(true);
			GameUIWidget->SetVisibility(ESlateVisibility::Visible);
			APlayerCharacterController* controller = Cast<APlayerCharacterController>(this->GetController());
			controller->bShowMouseCursor = true;

			FInputModeGameAndUI InputModeUIOnly;
			//FInputModeUIOnly InputModeUIOnly;
			controller->SetInputMode(InputModeUIOnly);

		}
	}

}

void ABaseCharacter::AttackCheck()
{
	//FHitResult HitResult;
	//FCollisionQueryParams Params(NAME_None, false, this);
	//bool bResult = GetWorld()->SweepSingleByChannel(
	//	HitResult,
	//	GetActorLocation(),
	//	GetActorLocation() + GetActorForwardVector() * 200.f,
	//	FQuat::Identity,
	//	ECollisionChannel::ECC_GameTraceChannel13,
	//	FCollisionShape::MakeSphere(30.f),
	//	Params
	//);

	//if (bResult) {

	//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Hit Actor")));
	//	FDamageEvent DamageEvent;
	//	HitResult.GetActor()->TakeDamage(GetSTR() * CurrentWeapon->m_fWeaponSTR, DamageEvent, GetController(), this);
	//}
}

void ABaseCharacter::Attack() // 다른 함수 둬서 어떤 무기 들었을때는 attack 힐링 아이템은 먹는 동작 이런것들 함수 호출하도록 하면 될듯
{
	if (m_bIsAttacking) {
		return;
	}
	auto AnimInstance = Cast<UPlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());

	AnimInstance->PlayAttackMontage();
	m_bIsAttacking = true;

	m_DAttackEnd.AddLambda([this]() -> void {
		m_bIsAttacking = false;
		});
}

void ABaseCharacter::PickUp()
{
	if (m_bIsPickUping) {
		return;
	}
	auto AnimInstance = Cast<UPlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());

	AnimInstance->PlayPickUpMontage();
	m_bIsPickUping = true;

	m_DPickUpEnd.AddLambda([this]() -> void {
		m_bIsPickUping = false;
		});
}

void ABaseCharacter::QuickNWeapon()
{

	if (IsBHHandIn()) {
		CurrentBleedingHealingItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentBleedingHealingItem->SetActorLocation(FVector(0.f, 0.f, 0.f));
		SetBHHandIn(false);
	}
	else if (IsHealHandIn()) {
		CurrentHealingItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentHealingItem->SetActorLocation(FVector(0.f, 0.f, 0.f));
		SetHealHandIn(false);
	}
	else if (IsKeyHandIn()) {
		CurrentKeyItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentKeyItem->SetActorLocation(FVector(0.f, 0.f, 0.f));
		SetKeyHandIn(false);
	}
	else if (IsThrowWHandIn()) {
		CurrentThrowWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentThrowWeapon->SetActorLocation(FVector(0.f, 0.f, 0.f));
		SetThrowWHandIn(false);
	}

	if (IsBringCurrentWeapon()) {
		FName WeaponSocket = TEXT("RightHandSocket");
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
		CurrentWeapon->SetActorRelativeRotation(CurrentWeapon->ItemHandRot);
		SetNWHandIn(true);
	}
}

void ABaseCharacter::QuickBHItem()
{
	if (IsNWHandIn()) {
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeapon->SetActorLocation(FVector(0.f, 0.f, 0.f));
		SetNWHandIn(false);
	}
	else if (IsHealHandIn()) {
		CurrentHealingItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentHealingItem->SetActorLocation(FVector(0.f, 0.f, 0.f));
		SetHealHandIn(false);
	}
	else if (IsKeyHandIn()) {
		CurrentKeyItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentKeyItem->SetActorLocation(FVector(0.f, 0.f, 0.f));
		SetKeyHandIn(false);
	}
	else if (IsThrowWHandIn()) {
		CurrentThrowWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentThrowWeapon->SetActorLocation(FVector(0.f, 0.f, 0.f));
		SetThrowWHandIn(false);
	}

	if (IsBringCurrentBleedingHealingItem()) {
		FName Socket = TEXT("RightHandSocket");
		CurrentBleedingHealingItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, Socket);
		//CurrentBleedingHealingItem->SetActorRelativeRotation(CurrentBleedingHealingItem->ItemHandRot);
		SetBHHandIn(true);
	}
}

void ABaseCharacter::QuickHItem()
{
	if (IsNWHandIn()) {
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeapon->SetActorLocation(FVector(0.f, 0.f, 0.f));
		SetNWHandIn(false);
	}
	else if (IsBHHandIn()) {
		CurrentBleedingHealingItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentBleedingHealingItem->SetActorLocation(FVector(0.f, 0.f, 0.f));
		SetBHHandIn(false);
	}
	else if (IsKeyHandIn()) {
		CurrentKeyItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentKeyItem->SetActorLocation(FVector(0.f, 0.f, 0.f));
		SetKeyHandIn(false);
	}
	else if (IsThrowWHandIn()) {
		CurrentThrowWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentThrowWeapon->SetActorLocation(FVector(0.f, 0.f, 0.f));
		SetThrowWHandIn(false);
	}

	if (IsBringCurrentHealingItem()) {
		FName Socket = TEXT("RightHandSocket");
		CurrentHealingItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, Socket);
		//CurrentBleedingHealingItem->SetActorRelativeRotation(CurrentHealingItem->ItemHandRot);
		SetHealHandIn(true);
	}
}

void ABaseCharacter::QuickTWeapon()
{
	//if (IsNWHandIn()) {
	//	CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	//	CurrentWeapon->SetActorLocation(FVector(0.f, 0.f, 0.f));
	//  SetNWHandIn(false);
	//}
	//else if (IsBHHandIn()) {
	//	CurrentBleedingHealingItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	//	CurrentBleedingHealingItem->SetActorLocation(FVector(0.f, 0.f, 0.f));
	//  SetBHHandIn(false);
	//}
	//else if (IsKeyHandIn()) {
	//	CurrentKeyItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	//	CurrentKeyItem->SetActorLocation(FVector(0.f, 0.f, 0.f));
	//  SetKeyHandIn(false);
	//}
	//else if (IsHealHandIn()) {
	//	CurrentHealingItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	//	CurrentHealingItem->SetActorLocation(FVector(0.f, 0.f, 0.f));
	// 	SetHealHandIn(false);
	//}

	if (IsBringCurrentThrowWeapon()) {
		//FName Socket = TEXT("RightHandSocket");
		//CurrentThrowWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, Socket);
		//CurrentThrowWeapon->SetActorRelativeRotation(CurrentThrowWeapon->ItemHandRot);
		//SetThrowWHandIn(true);
	}
}

void ABaseCharacter::QuickKeyItem()
{
	if (IsNWHandIn()) {
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeapon->SetActorLocation(FVector(0.f, 0.f, 0.f));
		SetNWHandIn(false);
	}
	else if (IsBHHandIn()) {
		CurrentBleedingHealingItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentBleedingHealingItem->SetActorLocation(FVector(0.f, 0.f, 0.f));
		SetBHHandIn(false);
	}
	else if (IsThrowWHandIn()) {
		CurrentThrowWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentThrowWeapon->SetActorLocation(FVector(0.f, 0.f, 0.f));
		SetThrowWHandIn(false);
	}
	else if (IsHealHandIn()) {
		CurrentHealingItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentHealingItem->SetActorLocation(FVector(0.f, 0.f, 0.f));
		SetHealHandIn(false);
	}

	if (IsBringCurrentKeyItem()) {
		FName Socket = TEXT("RightHandSocket");
		CurrentKeyItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, Socket);
		//CurrentBleedingHealingItem->SetActorRelativeRotation(CurrentHealingItem->ItemHandRot);
		SetKeyHandIn(true);
	}
}


bool ABaseCharacter::CanSetWeapon()
{
	return (CurrentWeapon == nullptr);
}

void ABaseCharacter::SetWeapon(ANormalWeaponActor* NewWeapon)
{
	if (NewWeapon != nullptr) {
		FName WeaponSocket = TEXT("middle_01_rSocket");
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket); 
		NewWeapon->SetOwner(this);
		CurrentWeapon = NewWeapon;
		CurrentWeapon->SetActorRelativeRotation(FRotator(0.f, 110.f, 0.f));
		//SetHandIn(true);

	}
}

void ABaseCharacter::GameUIUpdate()
{
	GameUIWidget->Update();
}

void ABaseCharacter::SpawnNormalWeapon()
{
	if (CurrentWeapon != nullptr) {
		DestroyNormalWeapon();
	}
	if (CurrentWeapon == nullptr) {
		if (QuickSlot[4].Name == "Book") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWBook>(ANWBook::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandRot = FRotator(-20.000723f, 0.000053f, -0.000076f);
		}

		if (QuickSlot[4].Name == "Bottle") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWBottle>(ANWBottle::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandRot = FRotator(79.999289f, -0.000289f, 39.999591f);
		}

		if (QuickSlot[4].Name == "ButchersKnife") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWButchersKnife>(ANWButchersKnife::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandRot = FRotator(8.322683f, -51.518626f, -165.872939f);
		}

		if (QuickSlot[4].Name == "FireAxe") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWFireAxe>(ANWFireAxe::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandRot = FRotator(75.953103f, 30.525240f, -115.695654f);
		}

		if (QuickSlot[4].Name == "FireExtinguisher") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWFireExtinguisher>(ANWFireExtinguisher::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandRot = FRotator(42.626677f, -112.078009f, 94.180152f);
		}

		if (QuickSlot[4].Name == "FryingPan") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWFryingPan>(ANWFryingPan::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandRot = FRotator(64.077023f, 42.787571f, 169.643854f);
		}

		if (QuickSlot[4].Name == "GolfClub") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWGolfClub>(ANWGolfClub::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandRot = FRotator(64.077023f, 42.787571f, -169.643854f);
		}


		if (QuickSlot[4].Name == "Iron") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWIron>(ANWIron::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandRot = FRotator(-0.154879f, 132.467340f, -115.923267f);
		}
		

		if (QuickSlot[4].Name == "MagicStick") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWMagicStick>(ANWMagicStick::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandRot = FRotator(-8.453113f, -29.414131f, 34.598360f);
		}

		if (QuickSlot[4].Name == "MannequinArm") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWMannequinArm>(ANWMannequinArm::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandRot = FRotator(32.305092f, 130.958105f, 128.354786f);
		}

		if (QuickSlot[4].Name == "MannequinLeg") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWMannequinLeg>(ANWMannequinLeg::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandRot = FRotator(47.547310f, 37.725483f, 92.547263f);
		}

		if (QuickSlot[4].Name == "Pipe") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWPipe>(ANWPipe::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandRot = FRotator(-11.327502f, -32.779877f, -11.015485f);
		}


		if (QuickSlot[4].Name == "Plunger") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWPlunger>(ANWPlunger::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandRot = FRotator(-43.480796f, 60.009216f, 77.994586f);
		}


		if (QuickSlot[4].Name == "SashimiKnife") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWSashimiKnife>(ANWSashimiKnife::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandRot = FRotator(-15.715587f, -73.732937f, 178.967734f);
		}

		if (QuickSlot[4].Name == "Scissors") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWScissors>(ANWScissors::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandRot = FRotator(-10.799827f, -124.969209f, -178.468275f);

		}

		if (QuickSlot[4].Name == "Shovels") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWShovels>(ANWShovels::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandRot = FRotator(10.799129f, 55.030660f, 88.467021f);
		}

		if (QuickSlot[4].Name == "SquareWood") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWSquareWood>(ANWSquareWood::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator); 
			// CurrentWeapon->ItemHandPos();
			CurrentWeapon->ItemHandRot = FRotator(-79.094971f, 63.174097f, -97.998577f);
		}
		

		if (QuickSlot[4].Name == "WoodenBat") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWWoodenBat>(ANWWoodenBat::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator); 
			//CurrentWeapon->ItemHandPos();
			CurrentWeapon->ItemHandRot = FRotator(10.798949f, 55.030625f, -11.533303f);
		}

		CurrentWeapon->OwnerCharacter = this;
		CurrentWeapon->m_fCharacterSTR = m_fSTR;
	}
	
	SetBringCurrentWeapon(true);
}


void ABaseCharacter::SpawnThrowWeapon()
{

}

void ABaseCharacter::SpawnHealingItem()
{
	if (CurrentHealingItem != nullptr) {
		DestroyHealingItem();
	}
	if (CurrentHealingItem == nullptr) {
		CurrentHealingItem = GetWorld()->SpawnActor<AHDrink>(AHDrink::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		//CurrentHealingItem->ItemHandRot = FRotator(64.077023f, 42.787571f, -169.643854f);

		CurrentHealingItem->OwnerCharacter = this;
	}

	SetBringCurrentHealingItem(true);
}

void ABaseCharacter::SpawnBleedingHealingItem()
{
	if (CurrentBleedingHealingItem != nullptr) {
		DestroyHealingItem();
	}
	if (CurrentBleedingHealingItem == nullptr) {
		CurrentBleedingHealingItem = GetWorld()->SpawnActor<ABHBandage>(ABHBandage::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		//CurrentHealingItem->ItemHandRot = FRotator(64.077023f, 42.787571f, -169.643854f);

		CurrentBleedingHealingItem->OwnerCharacter = this;
	}

	SetBringCurrentBleedingHealingItem(true);
}

void ABaseCharacter::SpawnKeyItem()
{
	if (CurrentKeyItem != nullptr) {
		DestroyHealingItem();
	}
	if (CurrentKeyItem == nullptr) {
		CurrentKeyItem = GetWorld()->SpawnActor<AKCarkey1>(AKCarkey1::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		//CurrentHealingItem->ItemHandRot = FRotator(64.077023f, 42.787571f, -169.643854f);
	}

	SetBringCurrentKeyItem(true);
}

void ABaseCharacter::DestroyNormalWeapon()
{
	CurrentWeapon->Destroy();
	CurrentWeapon = nullptr;
}

void ABaseCharacter::DestroyThrowWeapon()
{
	CurrentThrowWeapon->Destroy();
	CurrentThrowWeapon = nullptr;
}

void ABaseCharacter::DestroyHealingItem()
{
	CurrentHealingItem->Destroy();
	CurrentHealingItem = nullptr;
}

void ABaseCharacter::DestroyBleedingHealingItem()
{
	CurrentBleedingHealingItem->Destroy();
	CurrentBleedingHealingItem = nullptr;
}

void ABaseCharacter::DestroyKeyItem()
{
	CurrentKeyItem->Destroy();
	CurrentKeyItem = nullptr;
}

void ABaseCharacter::DestroyNormalWepaonItemSlot()
{
	DestroyNormalWeapon();

	QuickSlot[4].Type = EItemType::ITEM_QUICK_NONE;
	QuickSlot[4].Name = "nullptr";
	QuickSlot[4].ItemClassType = EItemClass::NONE;
	QuickSlot[4].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
	QuickSlot[4].Count = 0;

	int slotindex = QuickSlot[4].SlotReference;
	QuickSlot[4].SlotReference = -1;

	Inventory[slotindex].Type = EItemType::ITEM_NONE;
	Inventory[slotindex].Name = "nullptr";
	Inventory[slotindex].ItemClassType = EItemClass::NONE;
	Inventory[slotindex].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
	Inventory[slotindex].Count = 0;

	GameUIUpdate();
}

void ABaseCharacter::DestroyThrowWeaponItemSlot()
{
	DestroyHealingItem();

	QuickSlot[2].Type = EItemType::ITEM_QUICK_NONE;
	QuickSlot[2].Name = "nullptr";
	QuickSlot[2].ItemClassType = EItemClass::NONE;
	QuickSlot[2].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
	QuickSlot[2].Count = 0;

	int slotindex = QuickSlot[2].SlotReference;
	QuickSlot[2].SlotReference = -1;

	Inventory[slotindex].Type = EItemType::ITEM_NONE;
	Inventory[slotindex].Name = "nullptr";
	Inventory[slotindex].ItemClassType = EItemClass::NONE;
	Inventory[slotindex].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
	Inventory[slotindex].Count = 0;

	GameUIUpdate();
}

void ABaseCharacter::DestroyHealingItemSlot()
{
	DestroyHealingItem();

	QuickSlot[1].Type = EItemType::ITEM_QUICK_NONE;
	QuickSlot[1].Name = "nullptr";
	QuickSlot[1].ItemClassType = EItemClass::NONE;
	QuickSlot[1].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
	QuickSlot[1].Count = 0;

	int slotindex = QuickSlot[1].SlotReference;
	QuickSlot[1].SlotReference = -1;

	Inventory[slotindex].Type = EItemType::ITEM_NONE;
	Inventory[slotindex].Name = "nullptr";
	Inventory[slotindex].ItemClassType = EItemClass::NONE;
	Inventory[slotindex].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
	Inventory[slotindex].Count = 0;

	GameUIUpdate();
}

void ABaseCharacter::DestroyBleedingHealingItemSlot()
{
	DestroyBleedingHealingItem();

	QuickSlot[0].Type = EItemType::ITEM_QUICK_NONE;
	QuickSlot[0].Name = "nullptr";
	QuickSlot[0].ItemClassType = EItemClass::NONE;
	QuickSlot[0].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
	QuickSlot[0].Count = 0;

	int slotindex = QuickSlot[0].SlotReference;
	QuickSlot[0].SlotReference = -1;

	Inventory[slotindex].Type = EItemType::ITEM_NONE;
	Inventory[slotindex].Name = "nullptr";
	Inventory[slotindex].ItemClassType = EItemClass::NONE;
	Inventory[slotindex].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
	Inventory[slotindex].Count = 0;

	GameUIUpdate();
}

void ABaseCharacter::DestroyKeyItemSlot()
{
	DestroyKeyItem();

	QuickSlot[3].Type = EItemType::ITEM_QUICK_NONE;
	QuickSlot[3].Name = "nullptr";
	QuickSlot[3].ItemClassType = EItemClass::NONE;
	QuickSlot[3].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
	QuickSlot[3].Count = 0;

	int slotindex = QuickSlot[3].SlotReference;
	QuickSlot[3].SlotReference = -1;

	Inventory[slotindex].Type = EItemType::ITEM_NONE;
	Inventory[slotindex].Name = "nullptr";
	Inventory[slotindex].ItemClassType = EItemClass::NONE;
	Inventory[slotindex].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
	Inventory[slotindex].Count = 0;

	GameUIUpdate();
}

uint32 ABaseCharacter::GetPlayerId() const
{
	return PlayerId;
}

void ABaseCharacter::SetPlayerId(uint32 NewPlayerId)
{
	PlayerId = NewPlayerId;
}

void ABaseCharacter::UpdatePlayerData(FVector Location)
{
	NewLocation = Location;
}

void ABaseCharacter::SetAttack(bool bAttack)
{
	b_attack = bAttack;
	if (b_attack)
	{
		Attack();
	}
}

bool ABaseCharacter::GetAttack()
{
	return b_attack;
}