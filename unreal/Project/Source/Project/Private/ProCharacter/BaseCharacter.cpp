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


// 출혈회복 아이템
#include "ProItem/BleedingHealingItemActor.h"
#include "ProItem/BHBandage.h"
#include "ProItem/BHClothes.h"
#include "ProItem/BHDirtyClothes.h"
#include "ProItem/BHGauze.h"


#include "ProUI/GamePlayerUI.h"
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
	FlashLight->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	PlayerSight->SetupAttachment(Camera);
	PlayerSight->SetRelativeLocation(FVector(150.f, 0.f, 88.f));
	
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.f), FRotator(0.f, -90.f, 0.f));
	SpringArm->TargetArmLength = 400.f;
	SpringArm->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("/Game/SKM_QuinnNew.SKM_QuinnNew"));

	if (SK_MANNEQUIN.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SK_MANNEQUIN.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> CHARACTER_ANIM(TEXT("/Game/ChAnimBlueprint.ChAnimBlueprint_C"));

	if (CHARACTER_ANIM.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(CHARACTER_ANIM.Class);
	}

	static ConstructorHelpers::FClassFinder <UGamePlayerUI> PLAYER_GAMEUI(TEXT("/Game/UI/BP_GamePlayerUI.BP_GamePlayerUI_C"));

	if (PLAYER_GAMEUI.Succeeded()) {
		GameUIClass = PLAYER_GAMEUI.Class;
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
	FlashLight->SetRelativeLocationAndRotation(FVector(7.933304f, -13.981234f, 64.513892f), FRotator( 0.f, 0.f, 0.f));
	// 기타 설정 (색상, 강도 등)
	FlashLight->SetLightColor(FLinearColor::White);
	FlashLight->SetIntensity(1000.f);
	FlashLight->SetOuterConeAngle(30.f);
	FlashLight->SetVisibility(true);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 500.f;

	CurrentWeapon = nullptr;
	CurrentThrowWeapon = nullptr;
	CurrentBleedingHealingItem = nullptr;
	CurrentHealingItem = nullptr;
	CurrentKeyItem = nullptr;

	PlayerId = 0;
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


	}
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	auto CharacterAnimInstance = Cast<UPlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr != CharacterAnimInstance) {
		CharacterAnimInstance->SetCurrentPawnSpeed(GetVelocity().Size());
		CharacterAnimInstance->SetIsPawnRun(m_bRun);
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
		}
		else if (CurrentInvenSlot.ItemClassType == EItemClass::HEALINGITEM) {
			QuickSlot[1].Type = EItemType::ITEM_QUICK_NONE;
			QuickSlot[1].Name = "nullptr";
			QuickSlot[1].ItemClassType = EItemClass::NONE;
			QuickSlot[1].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
			QuickSlot[1].Count = 0;
		}
		else if (CurrentInvenSlot.ItemClassType == EItemClass::THROWINGWEAPON) {
			QuickSlot[2].Type = EItemType::ITEM_QUICK_NONE;
			QuickSlot[2].Name = "nullptr";
			QuickSlot[2].ItemClassType = EItemClass::NONE;
			QuickSlot[2].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
			QuickSlot[2].Count = 0;
		}
		else if (CurrentInvenSlot.ItemClassType == EItemClass::KEYITEM) {
			QuickSlot[3].Type = EItemType::ITEM_QUICK_NONE;
			QuickSlot[3].Name = "nullptr";
			QuickSlot[3].ItemClassType = EItemClass::NONE;
			QuickSlot[3].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
			QuickSlot[3].Count = 0;
		}
		else if (CurrentInvenSlot.ItemClassType == EItemClass::NORMALWEAPON) {
			QuickSlot[4].Type = EItemType::ITEM_QUICK_NONE;
			QuickSlot[4].Name = "nullptr";
			QuickSlot[4].ItemClassType = EItemClass::NONE;
			QuickSlot[4].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
			QuickSlot[4].Count = 0;
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



void ABaseCharacter::MoveForward(float NewAxisValue)
{
	if (m_bRun) {
		SetSpeed(500.f);
		
	}
	else {
		SetSpeed(200.f);
	}

	GetCharacterMovement()->MaxWalkSpeed = m_fSpeed;
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue * m_fSpeed * GetWorld()->DeltaTimeSeconds);
	UE_LOG(LogTemp, Warning, TEXT("MoveFoward"));

}

void ABaseCharacter::MoveLeft(float NewAxisValue)
{
	if (m_bRun) {
		SetSpeed(500.f);
	}
	else {
		SetSpeed(200.f);
	}

	GetCharacterMovement()->MaxWalkSpeed = m_fSpeed;
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue * m_fSpeed * GetWorld()->DeltaTimeSeconds);
	UE_LOG(LogTemp, Warning, TEXT("MoveLeft"));
}

void ABaseCharacter::Run()
{
	if (m_bRun) {
		m_bRun = false;
	}
	else {
		m_bRun = true;
	}
	UE_LOG(LogTemp, Warning, TEXT("Run"));
}


void ABaseCharacter::LookUp(float NewAxisValue)
{
	AddControllerPitchInput(NewAxisValue);
	UE_LOG(LogTemp, Warning, TEXT("LookUp"));
}

void ABaseCharacter::Turn(float NewAxisValue)
{

	AddControllerYawInput(NewAxisValue);
	UE_LOG(LogTemp, Warning, TEXT("Turn"));
}
// 수정 필요
void ABaseCharacter::GetItem()
{
	if (PlayerSight->GetIsHit()) {
		
		auto itembox = Cast<AItemBoxActor>(PlayerSight->GetHitActor());

		//FString itemid = Itembox->GetInBoxItemId();

		//if (itemid == "SquareWood") {

		//}

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


		
		//수정 필요 Datatable 받아서 아이템에 넣어줘야 하나
		//itembox->OnChracterOvelapNew(this);
		
		PlayerSight->GetHitActor()->Destroy();
	}

	UE_LOG(LogTemp, Warning, TEXT("GetItem"));
}


// 추가할 부분 인벤토리에서 우클릭 시 퀵슬롯에 넣기, 그리고 아이템 생성, 그리고 퀵슬롯에 맞게 1,2,3,4번 누르면 손에 부착되게 할 예정 

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
	UE_LOG(LogTemp, Warning, TEXT("LifgtOnOff"));
}

void ABaseCharacter::InventoryOnOff()
{
	// 작성 필요
	UE_LOG(LogTemp, Warning, TEXT("InvenOpen"));
	if (GameUIWidget != nullptr) {
		if (IsInventory()) {
			SetInventory(false);
			GameUIWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		else {
			SetInventory(true);
			GameUIWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}

}

void ABaseCharacter::QuickNWeapon()
{
	if (IsBringCurrentWeapon()) {
		FName WeaponSocket = TEXT("middle_01_rSocket");
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
		CurrentWeapon->SetActorRelativeRotation(FRotator(0.f, 110.f, 0.f));
		SetHandIn(true);
	}
}

void ABaseCharacter::QuickBHItem()
{
	if (IsBringCurrentBleedingHealingItem()) {

	}
}

void ABaseCharacter::QuickHItem()
{
	if (IsBringCurrentHealingItem()) {

	}
}

void ABaseCharacter::QuickTWeapon()
{
	if (IsBringCurrentThrowWeapon()) {

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
		SetHandIn(true);

        
		UE_LOG(LogTemp, Warning, TEXT("wEAPONZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ"));
	}
}

void ABaseCharacter::GameUIUpdate()
{
	GameUIWidget->Update();
}

//IsValid()로 다 수정하기 오늘 할 일
void ABaseCharacter::SpawnNormalWeapon()
{
	if (CurrentWeapon == nullptr) {
		if (QuickSlot[4].Name == "SquareWood") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWSquareWood>(ANWSquareWood::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator); // 어디 퀵슬롯 아이템들은안보이는데에 모아두는 것이 좋을듯
		}
	}
	SetBringCurrentWeapon(true);
}


void ABaseCharacter::SpawnThrowWeapon()
{
}

void ABaseCharacter::SpawnHealingItem()
{
}

void ABaseCharacter::SpawnBleddingHealingItem()
{
}

void ABaseCharacter::SpawnKeyItem()
{
}

void ABaseCharacter::DestroyNormalWeapon()
{

}

void ABaseCharacter::DestroyThrowWeapon()
{
}

void ABaseCharacter::DestroyHealingItem()
{
}

void ABaseCharacter::DestroyBleddingHealingItem()
{
}

void ABaseCharacter::DestroyKeyItem()
{
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