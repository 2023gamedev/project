// Fill out your copyright notice in the Description page of Project Settings.


#include "ProCharacter/BaseCharacter.h"
#include "ProCharacter/PlayerCharacterAnimInstance.h"
#include "ProItem/ItemActor.h"
#include "ProItem/ItemBoxActor.h"
#include "ProCharacter/PlayerSight.h"
#include "Components/TextBlock.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

#include "ProZombie/BaseZombie.h"

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
#include "ProItem/KCarKey4.h"
#include "ProItem/KRoofKey1.h"
#include "ProItem/KRoofKey2.h"


// 출혈회복 아이템
#include "ProItem/BleedingHealingItemActor.h"
#include "ProItem/BHBandage.h"
#include "ProItem/BHClothes.h"
#include "ProItem/BHDirtyClothes.h"
#include "ProItem/BHGauze.h"

#include "ProItem/InterActor.h"
#include "ProItem/CarActor.h"
#include "ProItem/RoofTopDoorActor.h"

#include "ProUI/GamePlayerUI.h"
#include "ProUI/ConditionUI.h"
#include "ProUI/ProGameClearUI.h"
#include "ProUI/GameTimerUI.h"
#include "ProUI/PickUpUI.h"
#include "ProUI/CircularPB_UI.h"
#include "ProUI/TextMissionUI.h"
#include "ProUI/OtherPlayerUI.h"
#include "ProCharacter/PlayerCharacterController.h"
#include "ProNiagaFX/HealingNiagaEffect.h"

#define default_circularPB_widget_anim_playtime 5.f
#define default_healing_anim_playtime 4.57f
#define default_bleedhealing_anim_playtime 9.f
#define default_playkey_anim_playtime 5.23f
#define playtime_8_sec 8.f			// 키 사용시간
#define playtime_4_sec 4.f			// 참치캔, 소독약, 연고 사용시간
#define playtime_3_5_sec 3.5f		// 과자 사용시간
#define playtime_3_sec 3.f			// 음료수, 담배, 물 사용시 & 출혈 회복용 아이템 사용시간






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

	HealingFX = CreateDefaultSubobject<AHealingNiagaEffect>(TEXT("HealingFX"));

	SpringArm->SetupAttachment(GetCapsuleComponent());

	Camera->SetupAttachment(SpringArm);
	Camera->SetRelativeLocation(FVector(44.f, 0.f, 25.f));
	FlashLight->SetupAttachment(SpringArm);
	PlayerSight->SetupAttachment(Camera);
	PlayerSight->SetRelativeLocation(FVector(150.f, 0.f, 88.f));

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.f), FRotator(0.f, -90.f, 0.f));
	SpringArm->TargetArmLength = 300.f;
	SpringArm->SetRelativeRotation(FRotator(-30.f, 0.f, 0.f));
	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 30.f));



	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNEQUIN(TEXT("/Game/CharacterAsset/Employee/EmployeeCharacter_UE.EmployeeCharacter_UE"));

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

	static ConstructorHelpers::FClassFinder <UProGameClearUI> PLAYER_GAMECLEARUI(TEXT("/Game/UI/BP_ClearUI.BP_ClearUI_C"));

	if (PLAYER_GAMECLEARUI.Succeeded()) {
		ProGameClearUIClass = PLAYER_GAMECLEARUI.Class;
	}

	static ConstructorHelpers::FClassFinder <UGameTimerUI> PLAYER_GAMETIMERUI(TEXT("/Game/UI/BP_GameTimerUI.BP_GameTimerUI_C"));

	if (PLAYER_GAMETIMERUI.Succeeded()) {
		GameTimerUIClass = PLAYER_GAMETIMERUI.Class;
	}


	static ConstructorHelpers::FClassFinder <UPickUpUI> PLAYER_PICKUPUI(TEXT("/Game/UI/BP_PickUpUI.BP_PickUpUI_C"));

	if (PLAYER_PICKUPUI.Succeeded()) {
		PickUpUIClass = PLAYER_PICKUPUI.Class;
	}

	static ConstructorHelpers::FClassFinder <UCircularPB_UI> PLAYER_CIRCULARPB_UI(TEXT("/Game/UI/Widget_CicularPB.Widget_CicularPB_C"));

	if (PLAYER_CIRCULARPB_UI.Succeeded()) {
		CircularPB_Class = PLAYER_CIRCULARPB_UI.Class;
	}

	static ConstructorHelpers::FClassFinder <UTextMissionUI> PLAYER_TEXTMISSIONUI(TEXT("/Game/UI/BP_TextMissionUI.BP_TextMissionUI_C"));

	if (PLAYER_TEXTMISSIONUI.Succeeded()) {
		TextMissionUIClass = PLAYER_TEXTMISSIONUI.Class;
	}

	static ConstructorHelpers::FClassFinder <UOtherPlayerUI> PLAYER_OTHERPLAYERUI(TEXT("/Game/UI/BP_OtherPlayerUI.BP_OtherPlayerUI_C"));

	if (PLAYER_OTHERPLAYERUI.Succeeded()) {
		OtherPlayerUIClass = PLAYER_OTHERPLAYERUI.Class;
	}

	static ConstructorHelpers::FClassFinder <UOtherPlayerUI> PLAYER_OTHERPLAYER2UI(TEXT("/Game/UI/BP_OtherPlayerUI.BP_OtherPlayerUI_C"));

	if (PLAYER_OTHERPLAYER2UI.Succeeded()) {
		OtherPlayer2UIClass = PLAYER_OTHERPLAYER2UI.Class;
	}

	static ConstructorHelpers::FClassFinder <UOtherPlayerUI> PLAYER_OTHERPLAYER3UI(TEXT("/Game/UI/BP_OtherPlayerUI.BP_OtherPlayerUI_C"));

	if (PLAYER_OTHERPLAYER3UI.Succeeded()) {
		OtherPlayer3UIClass = PLAYER_OTHERPLAYER3UI.Class;
	}

	//SpringArm->TargetArmLength = 300.f;
	SpringArm->TargetArmLength = 170.f;
	SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bDoCollisionTest = true;
	bUseControllerRotationYaw = false;

	GetMesh()->SetCollisionProfileName("NoCollision");

	// 스포트 라이트의 속성 설정 (위치, 회전 등)
	FlashLight->SetRelativeLocationAndRotation(FVector(250.f, 0.f, 47.f), FRotator(0.f, 0.f, 0.f));
	// 기타 설정 (색상, 강도 등)
	FlashLight->SetLightColor(FLinearColor::White);
	FlashLight->SetIntensity(5000.f);
	FlashLight->SetOuterConeAngle(100.f);
	FlashLight->SetVisibility(true);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 500.f;

	CurrentWeapon = nullptr;
	CurrentThrowWeapon = nullptr;
	CurrentBleedingHealingItem = nullptr;
	CurrentHealingItem = nullptr;
	CurrentKeyItem = nullptr;

	HealingFX = nullptr;

}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<UProGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));


	if (TextMissionUIClass != nullptr) {

		APlayerCharacterController* controller = Cast<APlayerCharacterController>(this->GetController());
		if (controller == nullptr) {
			return;
		}

		TextMissionUIWidget = CreateWidget<UTextMissionUI>(controller, TextMissionUIClass);

		if (!TextMissionUIWidget) {
			return;
		}

		// 시작시 Text
		FText KText = FText::FromString(TEXT("이 건물에서 탈출하라"));
		ShowActionText(KText, FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f)), 5.f);


		// 시작시 오른쪽 MissionText
		FText KMissionText1 = FText::FromString(TEXT("옥상으로 탈출하라"));
		ShowMissionText(KMissionText1, FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f)), 1);

		FText KMissionText2 = FText::FromString(TEXT("- 옥상열쇠1을 구하라"));
		ShowMissionText(KMissionText2, FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f)), 2);

		FText KMissionText3 = FText::FromString(TEXT("- 옥상열쇠2를 구하라"));
		ShowMissionText(KMissionText3, FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f)), 3);

		FText KMissionText4 = FText::FromString(TEXT("지하로 탈출하라"));
		ShowMissionText(KMissionText4, FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f)), 4);

		FText KMissionText5 = FText::FromString(TEXT("- 차 키를 구하라"));
		ShowMissionText(KMissionText5, FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f)), 5);

		TextMissionUIWidget->AddToViewport();
	}

	if (OtherPlayerUIClass != nullptr) {

		APlayerCharacterController* controller = Cast<APlayerCharacterController>(this->GetController());
		if (controller == nullptr) {
			return;
		}
		OtherPlayerUIWidget = CreateWidget<UOtherPlayerUI>(controller, OtherPlayerUIClass);

		if (!OtherPlayerUIWidget) {
			return;
		}

		OtherPlayerUIWidget->Character = this;
		OtherPlayerUIWidget->m_iOtherPlayerUIIndex = 1;
		OtherPlayerUIWidget->Init();
		OtherPlayerUIWidget->AddToViewport();
		OtherPlayerUIWidget->SetVisibility(ESlateVisibility::Visible);

		FVector2D Offset(0, 300); 
		OtherPlayerUIWidget->SetRenderTranslation(Offset);
	}

	if (OtherPlayer2UIClass != nullptr) {

		APlayerCharacterController* controller = Cast<APlayerCharacterController>(this->GetController());
		if (controller == nullptr) {
			return;
		}
		OtherPlayer2UIWidget = CreateWidget<UOtherPlayerUI>(controller, OtherPlayer2UIClass);

		if (!OtherPlayer2UIWidget) {
			return;
		}

		OtherPlayer2UIWidget->Character = this;
		OtherPlayer2UIWidget->m_iOtherPlayerUIIndex = 2;
		OtherPlayer2UIWidget->Init();
		OtherPlayer2UIWidget->AddToViewport();
		OtherPlayer2UIWidget->SetVisibility(ESlateVisibility::Visible);

		FVector2D Offset(0, 500); 
		OtherPlayer2UIWidget->SetRenderTranslation(Offset);
	}

	if (OtherPlayer3UIClass != nullptr) {

		APlayerCharacterController* controller = Cast<APlayerCharacterController>(this->GetController());
		if (controller == nullptr) {
			return;
		}
		OtherPlayer3UIWidget = CreateWidget<UOtherPlayerUI>(controller, OtherPlayer3UIClass);

		if (!OtherPlayer3UIWidget) {
			return;
		}

		OtherPlayer3UIWidget->Character = this;
		OtherPlayer3UIWidget->m_iOtherPlayerUIIndex = 3;
		OtherPlayer3UIWidget->Init();
		OtherPlayer3UIWidget->AddToViewport();
		OtherPlayer3UIWidget->SetVisibility(ESlateVisibility::Visible);

		FVector2D Offset(0, 700); 
		OtherPlayer3UIWidget->SetRenderTranslation(Offset);

		OtherPlayerUIOffset(GameInstance->ClientSocketPtr->GetMyPlayerId());
	}

	


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

	if (GameTimerUIClass != nullptr) {

		APlayerCharacterController* controller = Cast<APlayerCharacterController>(this->GetController());
		if (controller == nullptr) {
			return;
		}

		GameTimerUIWidget = CreateWidget<UGameTimerUI>(controller, GameTimerUIClass);

		if (!GameTimerUIWidget) {
			return;
		}

		GameTimerUIWidget->Character = this;
		GameTimerUIWidget->AddToViewport();
	}

	if (PickUpUIClass != nullptr) {

		APlayerCharacterController* controller = Cast<APlayerCharacterController>(this->GetController());
		if (controller == nullptr) {
			return;
		}

		PickUpUIWidget = CreateWidget<UPickUpUI>(controller, PickUpUIClass);

		if (!PickUpUIWidget) {
			return;
		}

		PickUpUIWidget->Character = this;
		PickUpUIWidget->Init();
		PickUpUIWidget->AddToViewport();
		PickUpUIWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (CircularPB_Class != nullptr) {

		APlayerCharacterController* controller = Cast<APlayerCharacterController>(this->GetController());
		if (controller == nullptr) {
			return;
		}

		CircularPB_Widget = CreateWidget<UCircularPB_UI>(controller, CircularPB_Class);

		if (!CircularPB_Widget) {
			return;
		}

		CircularPB_Widget->AddToViewport();
		CircularPB_Widget->SetVisibility(ESlateVisibility::Hidden);
	}



	auto AnimInstance = Cast<UPlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());

	AnimInstance->OnMontageEnded.AddDynamic(this, &ABaseCharacter::AttackMontageEnded);

	AnimInstance->OnAttackStartCheck.AddLambda([this]() -> void {
		if (CurrentWeapon != nullptr) {
			CurrentWeapon->BoxComponent->SetCollisionProfileName(TEXT("WeaponItem"));
		}
		});

	AnimInstance->OnAttackEndCheck.AddLambda([this]() -> void {
		if (CurrentWeapon != nullptr) {
			CurrentWeapon->BoxComponent->SetCollisionProfileName(TEXT("NoCollision"));
		}
		});

	AnimInstance->OnFootSoundCheck.AddLambda([this]() -> void {
		FootSound();
		});


	//// Slice 용 Weapon - TEST
	//if (CurrentWeapon == nullptr) { 

	//	CurrentWeapon = GetWorld()->SpawnActor<ANWButchersKnife>(ANWButchersKnife::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
	//	//CurrentWeapon->ItemHandPos = FVector(-1.538658f, 1.908217f, 0.224630f);
	//	//CurrentWeapon->ItemHandRot = FRotator(4.949407f, -31.214014f, -172.213653f);

	//	CurrentWeapon->ItemHandPos = FVector(-0.757521f, 1.883819f, 1.937180f);
	//	CurrentWeapon->ItemHandRot = FRotator(-12.172822f, -34.649834f, -158.006864f);


	//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("IsBringCurrentWeapon"));
	//	FName WeaponSocket = TEXT("RightHandSocket");
	//	CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
	//	CurrentWeapon->SetActorRelativeRotation(CurrentWeapon->ItemHandRot);
	//	CurrentWeapon->SetActorRelativeLocation(CurrentWeapon->ItemHandPos);
	//	CurrentWeapon->OwnerCharacter = this;
	//	CurrentWeapon->m_fCharacterSTR = m_fSTR;
	//	CurrentWeapon->m_fWeaponDurability = 50.f;
	//	SetNWHandIn(true);

	//}


}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 플레이어 floor 계산
	if (PlayerId == 99) {	// 자기 자신에 대해서만 계산
		if (GetActorLocation().Z < 800.f) {
			floor = FLOOR::B2;
		}
		else if (GetActorLocation().Z < 1800.f) {
			floor = FLOOR::B1;
		}
		else if (GetActorLocation().Z < 2500.f) {
			floor = FLOOR::F1;
		}
		else if (GetActorLocation().Z < 3600.f) {
			floor = FLOOR::F2;
		}
		else {
			floor = FLOOR::F3;
		}
	}

	



	auto AnimInstance = Cast<UPlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());

	if (!GetVelocity().Size()) {
		if (OldLocation != FVector(0.0f, 0.0f, 0.0f)) {
			float DistanceMoved = FVector::Dist(OldLocation, NewLocation);
			Speed = (DeltaTime > 0) ? (DistanceMoved / DeltaTime) : 0;
		}
		
		// 애니메이션 인스턴스에 속도 파라미터 설정
		if ((Speed != 0 && PreviousSpeed == 0) || (Speed == 0 && PreviousSpeed != 0))
		{
			if (AnimInstance) {
				AnimInstance->SetCurrentPawnSpeed(Speed);
			}
		}

		if (m_bJump == true) {
			if (AnimInstance)
			{
				AnimInstance->PlayJumpMontage();
			}
			m_bJump = false;
		}

		AnimInstance->SetIsPawnRun(m_bRun);


		PreviousSpeed = Speed;
		OldLocation = NewLocation;
	}

	else {
		if (nullptr != AnimInstance) {
			AnimInstance->SetCurrentPawnSpeed(GetVelocity().Size());
			AnimInstance->SetIsPawnRun(m_bRun);
		}

		PreviousSpeed = GetVelocity().Size();
	}

	if (ConditionUIWidget) {
		ConditionUIWidget->UpdateBar();
	}

	if (GameUIWidget) {
		if (GameTimerUIWidget->IsTimeUp()) {
			ProStartGameTimerEnd();
		}
		GameTimerUIWidget->UpdateTimer();
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

// 플레이어가 피격 (공격 받았을때)
float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);


	if (GetHP() > 0) {

		SetHP(GetHP() - Damage);
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Hit Character")));
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("HP %f"), GetHP()));
	}
	else if (GetHP() <= 0 && !IsDeadPlay()) {
		SetDeadPlay(true);
	}


	if (IsDeadPlay() && !IsDead()) {
		PlayDead();
	}



	if (!m_bBleeding) {
		m_bBleeding = RandomBleeding();

		if (m_bBleeding) {
			StartBleedingTimer();
		}
	}


	return Damage;
}

void ABaseCharacter::PlayDead()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("DEAD")));
	SetDead(true);
	auto CharacterAnimInstance = Cast<UPlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (nullptr != CharacterAnimInstance) {
		CharacterAnimInstance->SetCurrentPawnSpeed(0.f);
		CharacterAnimInstance->SetIsDead(IsDead());
	}

	FText KText = FText::FromString(TEXT("당신은 죽었습니다."));
	ShowActionText(KText, FSlateColor(FLinearColor(1.0f, 0.0f, 0.0f)), 5.f);
	
	GetCapsuleComponent()->SetCollisionProfileName("NoCollision");
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);


	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);

	APlayerCharacterController* controller = Cast<APlayerCharacterController>(this->GetController());
	if (controller != nullptr) {
		controller->DisabledControllerInput();
	}

	

	ProStartGameDeadEnd();
}

void ABaseCharacter::OtherPlayerUIOffset(uint32 playerid)
{
	if (playerid == 1) {
		OtherPlayerUIWidget->m_iOtherPlayerUINumber = 2;
		OtherPlayer2UIWidget->m_iOtherPlayerUINumber = 3;
		OtherPlayer3UIWidget->m_iOtherPlayerUINumber = 4;
	}
	if (playerid == 2) {
		OtherPlayerUIWidget->m_iOtherPlayerUINumber = 1;
		OtherPlayer2UIWidget->m_iOtherPlayerUINumber = 3;
		OtherPlayer3UIWidget->m_iOtherPlayerUINumber = 4;
	}
	else if (playerid == 3) {
		OtherPlayerUIWidget->m_iOtherPlayerUINumber = 1;
		OtherPlayer2UIWidget->m_iOtherPlayerUINumber = 2;
		OtherPlayer3UIWidget->m_iOtherPlayerUINumber = 4;
	}
	else if (playerid == 4) {
		OtherPlayerUIWidget->m_iOtherPlayerUINumber = 1;
		OtherPlayer2UIWidget->m_iOtherPlayerUINumber = 2;
		OtherPlayer3UIWidget->m_iOtherPlayerUINumber = 3;
	}

}

void ABaseCharacter::UpdateOtherPlayerUI(uint32 playerid, float hp , uint32 charactertype, std::string Playername) // playername 추가 필요
{
	if (OtherPlayerUIWidget->m_iOtherPlayerUINumber == playerid) {
		OtherPlayerUIWidget->UpdateOtherPlayerUI(hp, charactertype,Playername);
	}
	else if (OtherPlayer2UIWidget->m_iOtherPlayerUINumber == playerid) {
		OtherPlayer2UIWidget->UpdateOtherPlayerUI(hp, charactertype,Playername);
	}
	else if (OtherPlayer3UIWidget->m_iOtherPlayerUINumber == playerid) {
		OtherPlayer3UIWidget->UpdateOtherPlayerUI(hp, charactertype, Playername);
	}
}

void ABaseCharacter::UpdatePickUpKey(uint32 keyid, uint32 playerid)
{
	// 옥상 키였을때는 5분 지났는지 timerwidget 확인 플레이어id 활용해서 해당 ui로 이동
	if (keyid == 1) {
		++(TextMissionUIWidget->m_iFindCarKey);
	}
	else if (keyid == 2) {
		++(TextMissionUIWidget->m_iFindRoofKey);
	}

	if (OtherPlayerUIWidget->m_iOtherPlayerUINumber == playerid) {
		if (keyid == 1) {
			if (OtherPlayerInven[0].Count == 0) {
				OtherPlayerInven[0].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenCarKey1.InvenCarKey1"));
				OtherPlayerInven[0].Count = 1;
			}
			else {
				++(OtherPlayerInven[0].Count);
			}
		}
		else if (keyid == 2) {
			if (OtherPlayerInven[1].Count == 0) {
				OtherPlayerInven[1].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenRoofKey1.InvenRoofKey1"));
				OtherPlayerInven[1].Count = 1;
			}
			else {
				++(OtherPlayerInven[1].Count);
			}
		}
		OtherPlayerUIWidget->Update();
	}
	else if (OtherPlayer2UIWidget->m_iOtherPlayerUINumber == playerid) {
		if (keyid == 1) {
			if (OtherPlayer2Inven[0].Count == 0) {
				OtherPlayer2Inven[0].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenCarKey1.InvenCarKey1"));
				OtherPlayer2Inven[0].Count = 1;
			}
			else {
				++(OtherPlayer2Inven[0].Count);
			}


		}
		else if (keyid == 2) {
			if (OtherPlayer2Inven[1].Count == 0) {
				OtherPlayer2Inven[1].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenRoofKey1.InvenRoofKey1"));
				OtherPlayer2Inven[1].Count = 1;
			}
			else {
				++(OtherPlayer2Inven[1].Count);
			}
		}
		OtherPlayer2UIWidget->Update();
	}
	else if (OtherPlayer3UIWidget->m_iOtherPlayerUINumber == playerid) {
		if (keyid == 1) {
			if (OtherPlayer3Inven[0].Count == 0) {
				OtherPlayer3Inven[0].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenCarKey1.InvenCarKey1"));
				OtherPlayer3Inven[0].Count = 1;
			}
			else {
				++(OtherPlayer3Inven[0].Count);
			}


		}
		else if (keyid == 2) {
			if (OtherPlayer3Inven[1].Count == 0) {
				OtherPlayer3Inven[1].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Game/InvenPng/InvenRoofKey1.InvenRoofKey1"));
				OtherPlayer3Inven[1].Count = 1;
			}
			else {
				++(OtherPlayer3Inven[keyid].Count);
			}
		}
		OtherPlayer3UIWidget->Update();
	}

	if (keyid == 1 && TextMissionUIWidget->m_iFindCarKey == 1) {
		if (!(GameTimerUIWidget->RoofTopClose)) {
			FText KText = FText::FromString(TEXT("차 키를 찾았습니다."));
			USoundBase* Sound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Sound/ChartFindKey.ChartFindKey")); // 에셋 경로
			PlaySoundForPlayer(Sound);
			ShowActionText(KText, FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f)), 5.f);
			Cast<UTextMissionUI>(TextMissionUIWidget)->PlayFadeOutMT2();
		}
	}
	else if (keyid == 2 && TextMissionUIWidget->m_iFindRoofKey == 1) {
		FText KText = FText::FromString(TEXT("옥상 키를 찾았습니다."));
		USoundBase* Sound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Sound/ChartFindKey.ChartFindKey")); // 에셋 경로
		PlaySoundForPlayer(Sound);
		ShowActionText(KText, FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f)), 5.f);
		Cast<UTextMissionUI>(TextMissionUIWidget)->PlayFadeOutMT1();
	}
	else if (keyid == 2 && TextMissionUIWidget->m_iFindRoofKey == 2) {
		FText KText = FText::FromString(TEXT("옥상 키를 모두 찾았습니다."));
		USoundBase* Sound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Sound/ChartFindKey.ChartFindKey")); // 에셋 경로
		PlaySoundForPlayer(Sound);
		ShowActionText(KText, FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f)), 5.f);
		Cast<UTextMissionUI>(TextMissionUIWidget)->PlayFadeOutMT11();
	}

}

void ABaseCharacter::ProGameClear(uint32 root, uint32 alive_players, uint32 dead_players, const FString& open_player, uint32 my_kill_count, uint32 best_kill_count, const FString& best_kill_player)
{
	if (ProGameClearUIClass != nullptr) {

		APlayerCharacterController* PlayerController = Cast<APlayerCharacterController>(this->GetController());
		if (PlayerController)
		{
			ProGameClearUIWidget = CreateWidget<UProGameClearUI>(PlayerController, ProGameClearUIClass);
			if (ProGameClearUIWidget)
			{
				ProGameClearUIWidget->AddToViewport();

				// EndingUI
				if (root == 1) {
					FString HowToEscape = " Escape::Car ";
					ProGameClearUIWidget->SetMessage(1, HowToEscape);
				}
				else if (root == 2) {
					FString HowToEscape = " Escape::Roof ";
					ProGameClearUIWidget->SetMessage(1, HowToEscape);
				}

				FString TimerString = FString::Printf(TEXT("%d:%02d"), GameTimerUIWidget->m_iMinites, GameTimerUIWidget->m_iSeconds);
				ProGameClearUIWidget->SetMessage(2, TimerString);

				FString AlivePlayersString = FString::Printf(TEXT(" %d "), alive_players);
				ProGameClearUIWidget->SetMessage(3, AlivePlayersString);

				FString DeadPlayersString = FString::Printf(TEXT(" %d "), dead_players);
				ProGameClearUIWidget->SetMessage(32, DeadPlayersString);

				ProGameClearUIWidget->SetMessage(4, open_player);

				FString MyKillCountString = FString::Printf(TEXT(" %d "), my_kill_count);
				ProGameClearUIWidget->SetMessage(5, MyKillCountString);

				// 합친 문자열 생성
				FString BestKillString = FString::Printf(TEXT("%d kills by %s"), best_kill_count, *best_kill_player);
				ProGameClearUIWidget->SetMessage(52, BestKillString);

				//ProStartGameEnd();
			}
		}
	}

}

void ABaseCharacter::PlaySoundAtLocationForPlayer(USoundBase* Sound, FVector Location)
{
	APlayerCharacterController* controller = Cast<APlayerCharacterController>(this->GetController());
	if (controller && controller->IsLocalController() && Sound)
	{
		UGameplayStatics::PlaySoundAtLocation(controller, Sound, Location);
	}
}

void ABaseCharacter::PlaySoundForPlayer(USoundBase* Sound)
{
	APlayerCharacterController* controller = Cast<APlayerCharacterController>(this->GetController());
	if (controller && controller->IsLocalController() && Sound)
	{
		UGameplayStatics::PlaySound2D(controller, Sound);
	}
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
			DestroyBleedingHealingItem();
			QuickSlot[0].Type = EItemType::ITEM_QUICK_NONE;
			QuickSlot[0].Name = "nullptr";
			QuickSlot[0].ItemClassType = EItemClass::NONE;
			QuickSlot[0].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
			QuickSlot[0].Count = 0;
			QuickSlot[0].SlotReference = -1;

		}
		else if (CurrentInvenSlot.ItemClassType == EItemClass::HEALINGITEM) {
			DestroyHealingItem();
			QuickSlot[1].Type = EItemType::ITEM_QUICK_NONE;
			QuickSlot[1].Name = "nullptr";
			QuickSlot[1].ItemClassType = EItemClass::NONE;
			QuickSlot[1].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
			QuickSlot[1].Count = 0;
			QuickSlot[1].SlotReference = -1;

		}
		else if (CurrentInvenSlot.ItemClassType == EItemClass::THROWINGWEAPON) {
			DestroyThrowWeapon();
			QuickSlot[2].Type = EItemType::ITEM_QUICK_NONE;
			QuickSlot[2].Name = "nullptr";
			QuickSlot[2].ItemClassType = EItemClass::NONE;
			QuickSlot[2].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
			QuickSlot[2].Count = 0;
			QuickSlot[2].SlotReference = -1;

		}
		else if (CurrentInvenSlot.ItemClassType == EItemClass::KEYITEM) {
			DestroyKeyItem();

			QuickSlot[3].Type = EItemType::ITEM_QUICK_NONE;
			QuickSlot[3].Name = "nullptr";
			QuickSlot[3].ItemClassType = EItemClass::NONE;
			QuickSlot[3].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
			QuickSlot[3].Count = 0;
			QuickSlot[3].SlotReference = -1;

		}
		else if (CurrentInvenSlot.ItemClassType == EItemClass::NORMALWEAPON) {
			DestroyNormalWeapon();

			QuickSlot[4].Type = EItemType::ITEM_QUICK_NONE;
			QuickSlot[4].Name = "nullptr";
			QuickSlot[4].ItemClassType = EItemClass::NONE;
			QuickSlot[4].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
			QuickSlot[4].Count = 0;
			QuickSlot[4].SlotReference = -1;

		}

		Inventory[slotindex].Type = EItemType::ITEM_NONE;
		Inventory[slotindex].Name = "nullptr";
		Inventory[slotindex].ItemClassType = EItemClass::NONE;
		Inventory[slotindex].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
		Inventory[slotindex].Count = 0;

		GameUIUpdate();
		ThrowOnGround.ExecuteIfBound(CurrentInvenSlot.Name, CurrentInvenSlot.ItemClassType, CurrentInvenSlot.Texture, CurrentInvenSlot.Count);
	}
	else if (CurrentInvenSlot.Type == EItemType::ITEM_USEABLE) {
		Inventory[slotindex].Type = EItemType::ITEM_NONE;
		Inventory[slotindex].Name = "nullptr";
		Inventory[slotindex].ItemClassType = EItemClass::NONE;
		Inventory[slotindex].Texture = LoadObject<UTexture2D>(NULL, TEXT("/Engine/ArtTools/RenderToTexture/Textures/127grey.127grey"));
		Inventory[slotindex].Count = 0;

		GameUIUpdate();
		ThrowOnGround.ExecuteIfBound(CurrentInvenSlot.Name, CurrentInvenSlot.ItemClassType, CurrentInvenSlot.Texture, CurrentInvenSlot.Count);
	}



	//if (CurrentInvenSlot.Type == EItemType::ITEM_EQUIPMENT || CurrentInvenSlot.Type == EItemType::ITEM_USEABLE) {
	//	if (CurrentInvenSlot.ItemClassType == EItemClass::BLEEDINGHEALINGITEM) {


	//		DestroyBleedingHealingItemSlot();
	//	}
	//	else if (CurrentInvenSlot.ItemClassType == EItemClass::HEALINGITEM) {


	//		DestroyHealingItemSlot();
	//	}
	//	else if (CurrentInvenSlot.ItemClassType == EItemClass::THROWINGWEAPON) {


	//		DestroyThrowWeaponItemSlot();
	//	}
	//	else if (CurrentInvenSlot.ItemClassType == EItemClass::KEYITEM) {

	//		DestroyKeyItemSlot();
	//	}
	//	else if (CurrentInvenSlot.ItemClassType == EItemClass::NORMALWEAPON) {


	//		DestroyNormalWepaonItemSlot();
	//	}

	//}


}






void ABaseCharacter::AttackMontageEnded(UAnimMontage* Montage, bool interrup)
{
	m_bIsAttacking = false;
	m_DAttackEnd.Broadcast();

	m_bIsPickUping = false;
	m_DPickUpEnd.Broadcast();

	m_bIsBleedHealing = false;
	m_DBleedingHealingEnd.Broadcast();
}



void ABaseCharacter::MoveForward(FVector RotateYaw, float NewAxisValue)
{
	if (m_bRun) {
		SetSpeed(GetBasicSpeed() * 100.f);

	}
	else {
		SetSpeed(GetBasicSpeed() / 2 * 100.f);
	}

	GetCharacterMovement()->MaxWalkSpeed = m_fSpeed;
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue * m_fSpeed * GetWorld()->DeltaTimeSeconds);
	AddMovementInput(RotateYaw, NewAxisValue * m_fSpeed * GetWorld()->DeltaTimeSeconds);

}

void ABaseCharacter::MoveLeft(FVector RotateYaw, float NewAxisValue)
{
	if (m_bRun) {
		SetSpeed(GetBasicSpeed() * 100.f);
	}
	else {
		SetSpeed(GetBasicSpeed() / 2 * 100.f);
	}

	GetCharacterMovement()->MaxWalkSpeed = m_fSpeed;
	//AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue * m_fSpeed * GetWorld()->DeltaTimeSeconds);
	AddMovementInput(RotateYaw, NewAxisValue * m_fSpeed * GetWorld()->DeltaTimeSeconds);
}

void ABaseCharacter::Run()
{
	if (m_bRun) {
		m_bRun = false;
		HealingStamina();
		GetWorld()->GetTimerManager().ClearTimer(UseStaminaHandle);
	}
	else {
		if (!m_bZeroStamina) {
			m_bRun = true;
			UseStamina();
			GetWorld()->GetTimerManager().ClearTimer(HealingStaminaHandle);
		}
	}
}

void ABaseCharacter::Other_Run(bool mbrun)
{
	if(m_bRun != mbrun){
	m_bRun = mbrun;
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
		if (itembox) {

			// 아이템박스에 있는 아이템에 대한 정보를 가져온다.
			for (int i = 0; i < 20; ++i) {
				if (Inventory[i].Type == EItemType::ITEM_NONE) {
					if (i >= GetInvenSize()) {
						//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "InvenSizeMAX!!!");
						return;
					}
					else {
						break;
					}
				}
			}

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

			PickUpSlot[0].Type = EItemType::ITEM_USEABLE;
			PickUpSlot[0].Name = itembox->ItemName;
			PickUpSlot[0].ItemClassType = itembox->ItemClassType;
			PickUpSlot[0].Texture = itembox->Texture;
			PickUpSlot[0].Count = itembox->Count;
			PickUpUIWidget->Update();
			OnPickUPUISlot();

			if (itembox->ItemClassType == EItemClass::KEYITEM) {
				if (itembox->ItemName.ToString().Contains("Car")) {
					Send_GetKey(1);
				}
				else if (itembox->ItemName.ToString().Contains("Roof")) {
					Send_GetKey(2);
				}
			}

			ItemBoxId = itembox->GetItemBoxId();
			PlayerSight->GetHitActor()->Destroy();

			Send_Destroy(ItemBoxId);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("GetItem"));
}

void ABaseCharacter::ShowActionText(FText Text, const FSlateColor& Color, float DisplayTime)
{
	if (TextMissionUIWidget)
	{
		// 텍스트 위젯을 찾고 설정 (이름이 ActionText인 TextBlock 위젯을 찾음)
		UTextBlock* ActionTextBlock = Cast<UTextBlock>(TextMissionUIWidget->GetWidgetFromName("ActionText"));
		if (ActionTextBlock)
		{
			// 텍스트 설정
			ActionTextBlock->SetText(Text);
			ActionTextBlock->SetColorAndOpacity(Color);

			// 텍스트 보이기
			TextMissionUIWidget->SetVisibility(ESlateVisibility::Visible);


			Cast<UTextMissionUI>(TextMissionUIWidget)->PlayFadeOutAnimation();

			//// 일정 시간이 지나면 숨기기 위한 타이머 설정
			//FTimerHandle TimerHandle;
			//GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() {
			//	if (TextMissionUIWidget)
			//	{
			//		TextMissionUIWidget->SetVisibility(ESlateVisibility::Collapsed);
			//	}
			//	}, DisplayTime, false);
		}
	}

}

void ABaseCharacter::ShowMissionText(FText Text, const FSlateColor& Color, int TextNumber)
{
	if (TextMissionUIWidget)
	{
		UTextBlock* MissionTextBlock;
		if (TextNumber == 1) {
			MissionTextBlock = Cast<UTextBlock>(TextMissionUIWidget->GetWidgetFromName("MissionText1"));
		}
		else if (TextNumber == 2) {
			MissionTextBlock = Cast<UTextBlock>(TextMissionUIWidget->GetWidgetFromName("MissionText2"));
		}
		else if (TextNumber == 3) {
			MissionTextBlock = Cast<UTextBlock>(TextMissionUIWidget->GetWidgetFromName("MissionText3"));
		}
		else if (TextNumber == 4) {
			MissionTextBlock = Cast<UTextBlock>(TextMissionUIWidget->GetWidgetFromName("MissionText4"));
		}
		else if (TextNumber == 5) {
			MissionTextBlock = Cast<UTextBlock>(TextMissionUIWidget->GetWidgetFromName("MissionText5"));
		}
		else {
			return;
		}

		if (MissionTextBlock)
		{
			// 텍스트 설정
			MissionTextBlock->SetText(Text);
			MissionTextBlock->SetColorAndOpacity(Color);
		}
	}
}

void ABaseCharacter::OnPickUPUISlot()
{
	PickUpUIWidget->SetVisibility(ESlateVisibility::Visible);
	GetWorld()->GetTimerManager().SetTimer(PickUpUIHandle, this, &ABaseCharacter::ProStartPickUpUI, 1.5f, false);
}

void ABaseCharacter::ProStartPickUpUI()
{
	PickUpUIWidget->SetVisibility(ESlateVisibility::Hidden);
}

void ABaseCharacter::OtherGetItem()
{
	if (PlayerSight->GetIsHit()) {

		auto itembox = Cast<AItemBoxActor>(PlayerSight->GetHitActor());
		PickUp();
		ItemBoxId = itembox->GetItemBoxId();
		PlayerSight->GetHitActor()->Destroy();
	}

	UE_LOG(LogTemp, Warning, TEXT("OtherGetItem"));
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
			USoundBase* Sound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Sound/invenopen.invenopen")); // 에셋 경로
			PlaySoundForPlayer(Sound);
		}
		else {
			SetInventory(true);
			GameUIWidget->SetVisibility(ESlateVisibility::Visible);
			APlayerCharacterController* controller = Cast<APlayerCharacterController>(this->GetController());
			controller->bShowMouseCursor = true;

			FInputModeGameAndUI InputModeUIOnly;
			//FInputModeUIOnly InputModeUIOnly;
			controller->SetInputMode(InputModeUIOnly);
			USoundBase* Sound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Sound/invenclose.invenclose")); // 에셋 경로
			PlaySoundForPlayer(Sound);

		}
	}

}
// 아마도 안쓸예정
//void ABaseCharacter::AttackCheck()
//{
//	
//
//	//FHitResult HitResult;
//	//FCollisionQueryParams Params(NAME_None, false, this);
//	//bool bResult = GetWorld()->SweepSingleByChannel(
//	//	HitResult,
//	//	GetActorLocation(),
//	//	GetActorLocation() + GetActorForwardVector() * 200.f,
//	//	FQuat::Identity,
//	//	ECollisionChannel::ECC_GameTraceChannel13,
//	//	FCollisionShape::MakeSphere(30.f),
//	//	Params
//	//);
//
//	//if (bResult) {
//
//	//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Hit Actor")));
//	//	FDamageEvent DamageEvent;
//	//	HitResult.GetActor()->TakeDamage(GetSTR() * CurrentWeapon->m_fWeaponSTR, DamageEvent, GetController(), this);
//	//}
//}

void ABaseCharacter::Attack() // 다른 함수 둬서 어떤 무기 들었을때는 attack 힐링 아이템은 먹는 동작 이런것들 함수 호출하도록 하면 될듯
{
	UE_LOG(LogTemp, Warning, TEXT("GetMyPlayerId: %u"), GameInstance->ClientSocketPtr->GetMyPlayerId());
	
	if (m_bIsAttacking) {
		return;
	}
	auto AnimInstance = Cast<UPlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());

	AnimInstance->PlayAttackMontage();
	m_bIsAttacking = true;

	m_DAttackEnd.AddLambda([this]() -> void {
		m_bIsAttacking = false;
		UE_LOG(LogTemp, Warning, TEXT("AttackEnd: %d"), PlayerId);
		});
	m_bIsAttacking = false;
}

void ABaseCharacter::Healing()
{
	if (m_bIsHealingTime) {
		return;
	}

	if (CurrentHealingItem != nullptr) {
		m_bIsHealingTime = true;

		CircularPB_Widget->SetVisibility(ESlateVisibility::Visible);

		auto AnimInstance = Cast<UPlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());

		float WidgetPlaySpeed = 1.f;
		float AnimPlaySpeed = 1.f;

		if (CurrentHealingItem->HName == "CannedTuna" || CurrentHealingItem->HName == "Ointment" || CurrentHealingItem->HName == "Disinfectant") {
			WidgetPlaySpeed = default_circularPB_widget_anim_playtime / playtime_4_sec;
			CircularPB_Widget->StartVisibleAnimation(WidgetPlaySpeed);

			AnimPlaySpeed = default_healing_anim_playtime / playtime_4_sec;
			AnimInstance->PlayHealingMontage(AnimPlaySpeed);
		}
		else if (CurrentHealingItem->HName == "Snack") {
			WidgetPlaySpeed = default_circularPB_widget_anim_playtime / playtime_3_5_sec;
			CircularPB_Widget->StartVisibleAnimation(WidgetPlaySpeed);

			AnimPlaySpeed = default_healing_anim_playtime / playtime_3_5_sec;
			AnimInstance->PlayHealingMontage(AnimPlaySpeed);
		}
		else if (CurrentHealingItem->HName == "Drink" || CurrentHealingItem->HName == "Smoke" || CurrentHealingItem->HName == "Water") {
			WidgetPlaySpeed = default_circularPB_widget_anim_playtime / playtime_3_sec;
			CircularPB_Widget->StartVisibleAnimation(WidgetPlaySpeed);

			AnimPlaySpeed = default_healing_anim_playtime / playtime_3_sec;
			AnimInstance->PlayHealingMontage(AnimPlaySpeed);
		}

		if (m_iHealingMontageFlag == 0) {
			++m_iHealingMontageFlag;
			AnimInstance->OnMontageEnded.AddDynamic(this, &ABaseCharacter::HealingMontageEnded);
		}
	}
}

void ABaseCharacter::HealingMontageEnded(UAnimMontage* Montage, bool interrup)
{
	auto AnimInstance = Cast<UPlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());

	if (AnimInstance) {
		if (Montage != AnimInstance->GetHealingMontage()) {
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "NOT HealingMontage");
			return;
		}
	}
	else {
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "HealingMontageEnd->AnimInstance NULL");
		return;
	}

	
	if (QuickSlot[1].Count <= 0) { // 이상하게 힐링 아이템 다 먹고 나서 다른 아이템 먹으면 이 함수가 호출되는 문제가 있어서 예외처리
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "HealingMontageEnd->   QuickSlot[1].Count <= 0!!!!!!!!!!!!!!!!!!!!!!!!!!");
		return;
	}

	CircularPB_Widget->SetVisibility(ESlateVisibility::Hidden);
	
	HealingFX = GetWorld()->SpawnActor<AHealingNiagaEffect>(AHealingNiagaEffect::StaticClass(), this->GetActorLocation(), FRotator::ZeroRotator);
	HealingFX->OwnerChar = this;

	if (CurrentHealingItem != nullptr) {
		StartHealingTimer(CurrentHealingItem->m_fHealingSpeed, CurrentHealingItem->m_fHealingDuration);
	}
	if (CurrentHealingItem->HName == "Smoke") {
		Smoking();
	}

	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "HealingMontageEnd!!!!!!!!!!!!!!!!!!!!!!!!!!");
	UpdateHealingSlot();
}

void ABaseCharacter::BleedHealing()
{
	if (m_bIsBleedHealing || !m_bBleeding) {
		return;
	}

	if (CurrentBleedingHealingItem != nullptr) {
		m_bIsBleedHealing = true;

		CircularPB_Widget->SetVisibility(ESlateVisibility::Visible);

		auto AnimInstance = Cast<UPlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());

		float WidgetPlaySpeed = 1.f;
		float AnimPlaySpeed = 1.f;

		WidgetPlaySpeed = default_circularPB_widget_anim_playtime / playtime_3_sec;
		CircularPB_Widget->StartVisibleAnimation(WidgetPlaySpeed);

		AnimPlaySpeed = default_bleedhealing_anim_playtime / playtime_3_sec;
		AnimInstance->PlayBleedHealingMontage(AnimPlaySpeed);

		if (m_iBleedHealingMontageFlag == 0) {
			AnimInstance->OnMontageEnded.AddDynamic(this, &ABaseCharacter::BleedHealingMontageEnded);
			++m_iBleedHealingMontageFlag;
		}

	}
}

void ABaseCharacter::BleedHealingMontageEnded(UAnimMontage* Montage, bool interrup)
{
	CircularPB_Widget->SetVisibility(ESlateVisibility::Hidden);

	auto AnimInstance = Cast<UPlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());

	if (AnimInstance) {
		if (Montage != AnimInstance->GetBleedingMontage()) {
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "NOT BleedingMontage");
			return;
		}
	}
	else {
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "HealingMontageEnd->AnimInstance NULL");
		return;
	}

	if (QuickSlot[0].Count <= 0) { // 이쪽도 예외처리
		return;
	}

	m_DBleedingHealingEnd.AddLambda([this]() -> void {
		m_bIsBleedHealing = false;

		if (CurrentBleedingHealingItem != nullptr) {
			m_bBleeding = RandomBleedHealing(CurrentBleedingHealingItem->m_fHealingSuccessProbability);
		}

		UpdateBHealingSlot();
		});
}


void ABaseCharacter::PlayKey()
{
	if (bIsKeyPlaying) { // 이미 키 애니메이션이 진행 중이면 실행 방지
		return;
	}

	if (PlayerSight->GetIsHit()) {
		AInterActor* InterActor = Cast<AInterActor>(PlayerSight->GetHitActor());
		if (InterActor) {
			bIsKeyPlaying = true; // 애니메이션 상태 플래그 설정
			CurrentInterActor = InterActor; // 상호작용 대상 저장

			CircularPB_Widget->SetVisibility(ESlateVisibility::Visible);

			float WidgetPlaySpeed = default_circularPB_widget_anim_playtime /default_playkey_anim_playtime;
			CircularPB_Widget->StartVisibleAnimation(WidgetPlaySpeed);

			PlayKeyAnim(); // 애니메이션 시작
		}
	}
}

void ABaseCharacter::PlayKeyAnim()
{
	auto AnimInstance = Cast<UPlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (!AnimInstance) {
		bIsKeyPlaying = false; // 애니메이션 실패 시 상태 초기화
		return;
	}

	float AnimPlaySpeed = 1.f;
	AnimInstance->PlayKeyMontage(AnimPlaySpeed);

	// 첫 연결만 이벤트 바인딩
	if (KeyMontageFlag == 0) {
		AnimInstance->OnMontageEnded.AddDynamic(this, &ABaseCharacter::KeyMontageEnded);
		++KeyMontageFlag;
	}
}

void ABaseCharacter::UpdateOpenKey(uint32 keyindex)
{
	if (keyindex == 1) {
		FText KText = FText::FromString(TEXT("풀렸다.지하로 탈출하라"));
		ShowActionText(KText, FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f)), 5.f);
	}
	else if (keyindex == 2) {
		++m_iOpenRoofKey;
		if (m_iOpenRoofKey == 1) {
			FText KText = FText::FromString(TEXT("옥상문 잠금이 하나 풀렸다."));
			ShowActionText(KText, FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f)), 5.f);
		}
		else if (m_iOpenRoofKey == 2) {
			FText KText = FText::FromString(TEXT("옥상이 풀렸다. 탈출하라"));
			ShowActionText(KText, FSlateColor(FLinearColor(1.0f, 1.0f, 1.0f)), 5.f);
		}
	}
}

void ABaseCharacter::KeyMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	CircularPB_Widget->SetVisibility(ESlateVisibility::Hidden); // 위젯 숨기기

	bIsKeyPlaying = false; // 애니메이션 상태 초기화

	auto AnimInstance = Cast<UPlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	if (!AnimInstance || Montage != AnimInstance->GetOpenKeyMontage()) {
		return; 
	}

	if (!CurrentInterActor)
		return;

	if (CurrentInterActor->InterActorName == "CarActor") {
		ACarActor* CarActor = Cast<ACarActor>(CurrentInterActor);
		if (CarActor && CurrentKeyItem->KeyName == CarActor->CarKeyName) {
			CarActor->UnLock();
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Key UNLOCK!!!");
			USoundBase* Sound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Sound/UnLockDoor.UnLockDoor")); // 에셋 경로
			PlaySoundForPlayer(Sound);
			UpdateKeySlot();
			Send_OpenRoot(1);
		}
		else {
			USoundBase* Sound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Sound/unlockfailed.unlockfailed")); // 에셋 경로
			PlaySoundForPlayer(Sound);
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Key NOT SAME!!!!");
		}
	}
	else if (CurrentInterActor->InterActorName == "RoofTopDoorActor") {
		ARoofTopDoorActor* RoofTopDoorActor = Cast<ARoofTopDoorActor>(CurrentInterActor);
		if (RoofTopDoorActor) {
			if (CurrentKeyItem->KeyName == "RoofKey1") {
				RoofTopDoorActor->UnlockKey1();
				//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "ROOFKEY1 UnLock");
				USoundBase* Sound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Sound/UnLockDoor.UnLockDoor")); // 에셋 경로
				PlaySoundForPlayer(Sound);
				UpdateKeySlot();
				Send_OpenRoot(2);
			}
			else if (CurrentKeyItem->KeyName == "RoofKey2") {
				RoofTopDoorActor->UnlockKey2();
				//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "ROOFKey2 Unlock");
				USoundBase* Sound = LoadObject<USoundBase>(nullptr, TEXT("/Game/Sound/UnLockDoor.UnLockDoor")); // 에셋 경로
				PlaySoundForPlayer(Sound);
				UpdateKeySlot();
				Send_OpenRoot(2);
			}
		}
	}
}

void ABaseCharacter::Throw() // throwweapon 생성 시 작성 필요
{

}

// 아이템을 먹고 아이템이 사라졌을때 다른 아이템을 주웠을때 인덱스 오류가 발생하는듯 수정 예정

void ABaseCharacter::UpdateHealingSlot()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "UpdateHealingSlot");
	if (QuickSlot[1].Count > 0) {
		QuickSlot[1].Count = QuickSlot[1].Count - 1;
		int slotindex = QuickSlot[1].SlotReference;
		Inventory[slotindex].Count = Inventory[slotindex].Count - 1;
	}


	if (QuickSlot[1].Count == 0) {
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "UpdateHealing Count == 0!!!!!!");
		DestroyHealingItemSlot();
	}
	else {
		GameUIUpdate();
	}
}

void ABaseCharacter::UpdateBHealingSlot()
{
	QuickSlot[0].Count = QuickSlot[0].Count - 1;
	int slotindex = QuickSlot[0].SlotReference;
	Inventory[slotindex].Count = Inventory[slotindex].Count - 1;
	if (QuickSlot[0].Count <= 0) {
		DestroyBleedingHealingItemSlot();
	}
	else {
		GameUIUpdate();
	}
}

void ABaseCharacter::UpdateKeySlot()
{
	QuickSlot[3].Count = QuickSlot[3].Count - 1;
	int slotindex = QuickSlot[3].SlotReference;
	Inventory[slotindex].Count = Inventory[slotindex].Count - 1;
	if (QuickSlot[3].Count <= 0) {
		DestroyKeyItemSlot();
	}
	else {
		GameUIUpdate();
	}
}

void ABaseCharacter::UpdateThrowWSlot() // throwweapon 생성 시 작성 필요
{
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

	if (IsBringCurrentWeapon() && CurrentWeapon) {
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("IsBringCurrentWeapon"));
		FName WeaponSocket = TEXT("RightHandSocket");
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
		CurrentWeapon->SetActorRelativeRotation(CurrentWeapon->ItemHandRot);
		CurrentWeapon->SetActorRelativeLocation(CurrentWeapon->ItemHandPos);
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

	if (IsBringCurrentBleedingHealingItem() && CurrentBleedingHealingItem) {
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("IsBringCurrentBleedingHealingItem"));
		FName Socket = TEXT("RightHandSocket");
		CurrentBleedingHealingItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, Socket);
		CurrentBleedingHealingItem->SetActorRelativeRotation(CurrentBleedingHealingItem->ItemHandRot);
		CurrentBleedingHealingItem->SetActorRelativeLocation(CurrentBleedingHealingItem->ItemHandPos);
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

	if (IsBringCurrentHealingItem() && CurrentHealingItem) {
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("IsBringCurrentHealingItem"));
		FName Socket = TEXT("RightHandSocket");
		CurrentHealingItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, Socket);
		CurrentHealingItem->SetActorRelativeRotation(CurrentHealingItem->ItemHandRot);
		CurrentHealingItem->SetActorRelativeLocation(CurrentHealingItem->ItemHandPos);
		SetHealHandIn(true);
	}
}

void ABaseCharacter::QuickTWeapon() // throwweapon 생성 시 작성 필요
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

	if (IsBringCurrentThrowWeapon() && CurrentThrowWeapon) {
		//FName Socket = TEXT("RightHandSocket");
		//CurrentThrowWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, Socket);
		//CurrentThrowWeapon->SetActorRelativeRotation(CurrentThrowWeapon->ItemHandRot);
		//CurrentThrowWeapon->SetActorRelativeLocation(CurrentThrowWeapon->ItemHandPos);
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

	if (IsBringCurrentKeyItem() && CurrentKeyItem) {
		FName Socket = TEXT("RightHandSocket");
		CurrentKeyItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, Socket);
		CurrentKeyItem->SetActorRelativeRotation(CurrentKeyItem->ItemHandRot);
		CurrentKeyItem->SetActorRelativeLocation(CurrentKeyItem->ItemHandPos);
		SetKeyHandIn(true);
	}
}


bool ABaseCharacter::CanSetWeapon()
{
	return (CurrentWeapon == nullptr);
}
// 아마도 안쓸예정
//void ABaseCharacter::SetWeapon(ANormalWeaponActor* NewWeapon)
//{
//	//if (NewWeapon != nullptr) {
//	//	FName WeaponSocket = TEXT("middle_01_rSocket");
//	//	NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket); 
//	//	NewWeapon->SetOwner(this);
//	//	CurrentWeapon = NewWeapon;
//	//	CurrentWeapon->SetActorRelativeRotation(FRotator(0.f, 110.f, 0.f));
//	//	//SetHandIn(true);
//
//	//}
//}

void ABaseCharacter::GameUIUpdate()
{
	GameUIWidget->Update();
}

void ABaseCharacter::SpawnNormalWeapon()
{
	if (CurrentWeapon != nullptr) {
		DestroyNormalWeapon();
	}
	USkeletalMeshComponent* SkeletalMesh = GetMesh();

	if (CurrentWeapon == nullptr) {
		if (QuickSlot[4].Name == "Book") { //*
			CurrentWeapon = GetWorld()->SpawnActor<ANWBook>(ANWBook::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(4.963954f, -3.163849f, 1.415202f);
			CurrentWeapon->ItemHandRot = FRotator(-49.169772f, 77.43936f, 19.22938f);
		}

		else if (QuickSlot[4].Name == "Bottle") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWBottle>(ANWBottle::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(-1.46633f, 4.540913f, 3.060996f);
			CurrentWeapon->ItemHandRot = FRotator(-0.000289f, 39.999591f, 79.999289f);
		}

		else if (QuickSlot[4].Name == "ButchersKnife") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWButchersKnife>(ANWButchersKnife::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(-0.757521f, 1.883819f, 1.937180f);
			CurrentWeapon->ItemHandRot = FRotator(-12.172822f, -34.649834f, -158.006864f);
		}

		else if (QuickSlot[4].Name == "FireAxe") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWFireAxe>(ANWFireAxe::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(1.241818f, 0.621437f, -0.044616f);
			CurrentWeapon->ItemHandRot = FRotator(65.529377f, 122.729752f, 303.71118f);
		}

		
		else if (QuickSlot[4].Name == "FireExtinguisher") { //*
			CurrentWeapon = GetWorld()->SpawnActor<ANWFireExtinguisher>(ANWFireExtinguisher::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(1.346740f, -0.000092f, 16.967230f);
			CurrentWeapon->ItemHandRot = FRotator(-60.000240f, -179.999764f, -179.999233f);
		}


		else if (QuickSlot[4].Name == "FryingPan") { //*
			CurrentWeapon = GetWorld()->SpawnActor<ANWFryingPan>(ANWFryingPan::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(0.499642f, -0.89209f, -1.988601f);
			CurrentWeapon->ItemHandRot = FRotator(29.999483f, -109.999095f, -0.000121f);
		}


		else if (QuickSlot[4].Name == "GolfClub") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWGolfClub>(ANWGolfClub::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(-0.09167f, -1.358051f, 0.250882f);
			CurrentWeapon->ItemHandRot = FRotator(-0.00006f, -99.99988f, -0.000023f);
		}


		else if (QuickSlot[4].Name == "Iron") { //*
			CurrentWeapon = GetWorld()->SpawnActor<ANWIron>(ANWIron::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(-0.00006f, 2.036528f, 3.535635f);
			CurrentWeapon->ItemHandRot = FRotator(0.000322f, 0.001126f, 69.999473f);
		}


		else if (QuickSlot[4].Name == "MagicStick") { //*
			CurrentWeapon = GetWorld()->SpawnActor<ANWMagicStick>(ANWMagicStick::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(12.221718f, -0.955271f, 0.690505f);
			CurrentWeapon->ItemHandRot = FRotator(0.926526f, -4.243007f, -180.634745f);
		}


		else if (QuickSlot[4].Name == "MannequinArm") { //**
			CurrentWeapon = GetWorld()->SpawnActor<ANWMannequinArm>(ANWMannequinArm::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(-1.987761f, -9.628748f, 1.854362f);
			CurrentWeapon->ItemHandRot = FRotator(-30.000611f, -159.998858f, 0.000562f);
		}


		else if (QuickSlot[4].Name == "MannequinLeg") { //**
			CurrentWeapon = GetWorld()->SpawnActor<ANWMannequinLeg>(ANWMannequinLeg::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(-6.11014f, -0.906533f, 3.864085f);
			CurrentWeapon->ItemHandRot = FRotator(80.000082f, -0.000489f, -0.000798f);
		}


		else if (QuickSlot[4].Name == "Pipe") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWPipe>(ANWPipe::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(26.137358f, 13.137087f, 0.643183f);
			CurrentWeapon->ItemHandRot = FRotator(-5.44398f, 17.537948f, -33.423673f);
		}


		else if (QuickSlot[4].Name == "Plunger") { //*
			CurrentWeapon = GetWorld()->SpawnActor<ANWPlunger>(ANWPlunger::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(-8.434205f, -1.096508f, -1.012945f);
			CurrentWeapon->ItemHandRot = FRotator(82.25204f, -119.327506f, -131.361975);
		}


		else if (QuickSlot[4].Name == "SashimiKnife") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWSashimiKnife>(ANWSashimiKnife::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(-6.766818f, 5.129228f, -0.58278f);
			CurrentWeapon->ItemHandRot = FRotator(15.53382f, -60.800216f, 184.87332f);
		}


		else if (QuickSlot[4].Name == "Scissors") { //**
			CurrentWeapon = GetWorld()->SpawnActor<ANWScissors>(ANWScissors::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(-1.193205f, -1.628935f, 2.40875f);
			CurrentWeapon->ItemHandRot = FRotator(20.000335f, -90.000792f, -9.999781f);

		}


		else if (QuickSlot[4].Name == "Shovels") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWShovels>(ANWShovels::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(7.668457f, -0.797488f, -0.431196f);
			CurrentWeapon->ItemHandRot = FRotator(26.807803f, 68.108858f, 84.914172f);
		}


		else if (QuickSlot[4].Name == "SquareWood") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWSquareWood>(ANWSquareWood::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(-11.797672f, 0.419462f, 7.687267f);
			CurrentWeapon->ItemHandRot = FRotator(-6.285888f, -101.190543f, 111.478725f);
		}


		else if (QuickSlot[4].Name == "WoodenBat") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWWoodenBat>(ANWWoodenBat::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(29.212954f, -3.556313f, -0.245431f);
			CurrentWeapon->ItemHandRot = FRotator(-26.286617f, 81.560518f, 0.445659f);
		}
		else {
			return;
		}

		CurrentWeapon->OwnerCharacter = this;
		CurrentWeapon->m_fCharacterSTR = m_fSTR;
	}

	SetBringCurrentWeapon(true);
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("SpawnNormalWeapon"));
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

		if (QuickSlot[1].Name == "CannedTuna") {
			CurrentHealingItem = GetWorld()->SpawnActor<AHCannedTuna>(AHCannedTuna::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}


		else if (QuickSlot[1].Name == "Disinfectant") {
			CurrentHealingItem = GetWorld()->SpawnActor<AHDisinfectant>(AHDisinfectant::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}

		else if (QuickSlot[1].Name == "Drink") {
			CurrentHealingItem = GetWorld()->SpawnActor<AHDrink>(AHDrink::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}

		else if (QuickSlot[1].Name == "Ointment") {
			CurrentHealingItem = GetWorld()->SpawnActor<AHOintment>(AHOintment::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}

		else if (QuickSlot[1].Name == "Smoke") {
			CurrentHealingItem = GetWorld()->SpawnActor<AHSmoke>(AHSmoke::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}

		else if (QuickSlot[1].Name == "Snack") {
			CurrentHealingItem = GetWorld()->SpawnActor<AHSnack>(AHSnack::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}

		else if (QuickSlot[1].Name == "Water") {
			CurrentHealingItem = GetWorld()->SpawnActor<AHWater>(AHWater::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}
		else {
			return;
		}

		CurrentHealingItem->ItemHandPos = FVector(-1.616189f, -3.900112f, 1.626949f);
		CurrentHealingItem->ItemHandRot = FRotator(-0.005714f, -70.00028f, 90.03065f);



		CurrentHealingItem->OwnerCharacter = this;
	}

	SetBringCurrentHealingItem(true);
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("SpawnHealingItem"));
}

void ABaseCharacter::SpawnBleedingHealingItem()
{
	if (CurrentBleedingHealingItem != nullptr) {
		DestroyBleedingHealingItem();
	}
	if (CurrentBleedingHealingItem == nullptr) {


		if (QuickSlot[0].Name == "Bandage") {
			CurrentBleedingHealingItem = GetWorld()->SpawnActor<ABHBandage>(ABHBandage::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}

		else if (QuickSlot[0].Name == "Clothes") {
			CurrentBleedingHealingItem = GetWorld()->SpawnActor<ABHClothes>(ABHClothes::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}

		else if (QuickSlot[0].Name == "DirtyClothes") {
			CurrentBleedingHealingItem = GetWorld()->SpawnActor<ABHDirtyClothes>(ABHDirtyClothes::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}

		else if (QuickSlot[0].Name == "Gauze") {
			CurrentBleedingHealingItem = GetWorld()->SpawnActor<ABHGauze>(ABHGauze::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}
		else {
			return;
		}
		CurrentBleedingHealingItem->ItemHandPos = FVector(0.211861f, -1.554539f, -1.946655f);
		CurrentBleedingHealingItem->ItemHandRot = FRotator(0.f, 0.f, 0.f);

		CurrentBleedingHealingItem->OwnerCharacter = this;
	}

	SetBringCurrentBleedingHealingItem(true);
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("SpawnBleedingHealingItem"));
}

void ABaseCharacter::SpawnKeyItem()
{
	if (CurrentKeyItem != nullptr) {
		DestroyKeyItem();
	}
	if (CurrentKeyItem == nullptr) {

		if (QuickSlot[3].Name == "CarKey1") {
			CurrentKeyItem = GetWorld()->SpawnActor<AKCarkey1>(AKCarkey1::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}

		else if (QuickSlot[3].Name == "CarKey2") {
			CurrentKeyItem = GetWorld()->SpawnActor<AKCarKey2>(AKCarKey2::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}

		else if (QuickSlot[3].Name == "CarKey3") {
			CurrentKeyItem = GetWorld()->SpawnActor<AKCarKey3>(AKCarKey3::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}

		else if (QuickSlot[3].Name == "CarKey4") {
			CurrentKeyItem = GetWorld()->SpawnActor<AKCarKey4>(AKCarKey4::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}

		else if (QuickSlot[3].Name == "RoofKey1") {
			CurrentKeyItem = GetWorld()->SpawnActor<AKRoofKey1>(AKRoofKey1::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}

		else if (QuickSlot[3].Name == "RoofKey2") {
			CurrentKeyItem = GetWorld()->SpawnActor<AKRoofKey2>(AKRoofKey2::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}
		else {
			return;
		}

		CurrentKeyItem->ItemHandPos = FVector(2.595437f, 1.093417f, 2.838135f);
		CurrentKeyItem->ItemHandRot = FRotator(69.997202f, -20.036148f, 29.978793f);
	}

	SetBringCurrentKeyItem(true);
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("SpawnKeyItem"));
}

void ABaseCharacter::DestroyNormalWeapon()
{
	if (CurrentWeapon != nullptr) {
		CurrentWeapon->Destroy();
	}

	SetNWHandIn(false);
	CurrentWeapon = nullptr;
}

void ABaseCharacter::DestroyThrowWeapon()
{
	if (CurrentThrowWeapon != nullptr) {
		CurrentThrowWeapon->Destroy();
	}

	SetThrowWHandIn(false);
	CurrentThrowWeapon = nullptr;
}

void ABaseCharacter::DestroyHealingItem()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "DestroyHealingItem!!!!!!");
	if (CurrentHealingItem != nullptr) {
		CurrentHealingItem->Destroy();
	}

	SetHealHandIn(false);
	CurrentHealingItem = nullptr;
}

void ABaseCharacter::DestroyBleedingHealingItem()
{
	if (CurrentBleedingHealingItem != nullptr) {
		CurrentBleedingHealingItem->Destroy();
	}
	SetBHHandIn(false);
	CurrentBleedingHealingItem = nullptr;
}

void ABaseCharacter::DestroyKeyItem()
{
	if (CurrentKeyItem != nullptr) {
		CurrentKeyItem->Destroy();
	}
	SetKeyHandIn(false);
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
	DestroyThrowWeapon();

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
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "DestroyHealingItemSlot!!!!!!");
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

void ABaseCharacter::FootSound()
{

	UWorld* World = GetWorld();
	FVector Center = GetActorLocation();
	float DetectRadius = 500.f;

	if (nullptr == World) return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	if (bResult) {

		for (const FOverlapResult& OverlapResult : OverlapResults)
		{
			// ABaseZombie인지 확인
			ABaseZombie* OverlappedZombie = Cast<ABaseZombie>(OverlapResult.GetActor());
			if (OverlappedZombie)
			{
				//OverlappedZombie->UpdateLastKnownPositionByFootSound(GetActorLocation());
			}

		}
	}
}

void ABaseCharacter::StartHealingTimer(float healingspeed, float healingduration)
{
	//if (m_bIsHealingTime) {
	//	return;
	//}
	// m_bIsHealingTime = true;

	m_fItemHealingSpeed = healingspeed;
	m_fItemHealingDuration = healingduration;

	GetWorld()->GetTimerManager().SetTimer(HealingHandle, this, &ABaseCharacter::HealingTimerElapsed, 1.0f, true, 1.0f);

}

void ABaseCharacter::HealingTimerElapsed()
{
	m_fHealingCount++;

	//종료조건
	if (m_fHealingCount > m_fItemHealingDuration) {
		GetWorld()->GetTimerManager().ClearTimer(HealingHandle);
		m_fHealingCount = 0.f;
		m_bIsHealingTime = false;

		SetHealing(0.f);

		if (HealingFX != NULL) {
			HealingFX->EndPlay(EEndPlayReason::Destroyed);
		}

		return;
	}

	SetHP(GetHP() + m_fItemHealingSpeed);

	SetHealing(m_fHealingCount / m_fItemHealingDuration * 100.f);

	if (GetHP() + m_fItemHealingSpeed > m_fMaxHP) {
		SetHP(GetMaxHP());
	}
}

void ABaseCharacter::AddScore(int32 score)
{
	m_iClearScore = score;
	// GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Score added: %d"), m_iClearScore));




	// ProStartGameEnd();

}

void ABaseCharacter::ProStartGameEnd()
{
	GetWorld()->GetTimerManager().SetTimer(GameEndHandle, this, &ABaseCharacter::ProGameEnd, 5.0f, false);
}



void ABaseCharacter::ProGameEnd()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		// 'quit' 명령을 실행하여 게임 종료
		PlayerController->ConsoleCommand("quit");
	}
}

void ABaseCharacter::StartBleedingTimer()
{
	SetSTR(GetSTR() - 2);
	SetBasicSpeed(GetBasicSpeed() - 1);
	GetWorld()->GetTimerManager().SetTimer(HealingHandle, this, &ABaseCharacter::BleedingTimerElapsed, 2.0f, true, 1.0f);
}

void ABaseCharacter::BleedingTimerElapsed()
{
	if (!m_bBleeding) {
		GetWorld()->GetTimerManager().ClearTimer(HealingHandle);
		SetSTR(GetSTR() + 2);
		SetBasicSpeed(GetBasicSpeed() + 1);
		return;
	}
	SetHP(GetHP() - 1);


	if (GetHP() <= 0 && !IsDeadPlay()) {
		SetDeadPlay(true);
	}


	if (IsDeadPlay() && !IsDead()) {
		PlayDead();
	}
}

bool ABaseCharacter::RandomBleeding()
{
	float RandomValue = FMath::FRand();
	return RandomValue <= m_fBleedPercent;
}

bool ABaseCharacter::RandomBleedHealing(float bhpercent)
{
	float RandomValue = FMath::FRand();
	return RandomValue > bhpercent;
}

void ABaseCharacter::UseStamina()
{
	GetWorld()->GetTimerManager().SetTimer(UseStaminaHandle, this, &ABaseCharacter::UseStaminaTimerElapsed, 1.0f, true, 1.0f);
}

void ABaseCharacter::UseStaminaTimerElapsed()
{
	if (GetVelocity().Size() == 0.f && GetStamina() > 0) {
		SetStamina(GetStamina() + 5);
		if (GetStamina() > 100) {
			SetStamina(100);
		}
		return;
	}
	SetStamina(GetStamina() - 10);


	if (GetStamina() <= 0) {
		m_bZeroStamina = true;
		m_bRun = false;
		HealingStamina();
		GetWorld()->GetTimerManager().SetTimer(UseStaminaHandle, this, &ABaseCharacter::StaminaWaitingByZero, 5.0f, false);
	}
}

void ABaseCharacter::StaminaWaitingByZero()
{
	m_bZeroStamina = false;
}

void ABaseCharacter::HealingStamina()
{
	GetWorld()->GetTimerManager().SetTimer(HealingStaminaHandle, this, &ABaseCharacter::HealingStaminaTimerElapsed, 1.0f, true, 1.0f);
}

void ABaseCharacter::HealingStaminaTimerElapsed()
{
	SetStamina(GetStamina() + 5);
	if (GetStamina() > 100) {
		SetStamina(100);
	}
}



void ABaseCharacter::ProStartGameDeadEnd()
{
	GetWorld()->GetTimerManager().SetTimer(GameDeadEndHandle, this, &ABaseCharacter::ProGameDeadEnd, 10.0f, false);
}

void ABaseCharacter::ProGameDeadEnd()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		// 'quit' 명령을 실행하여 게임 종료
		PlayerController->ConsoleCommand("quit");
	}
}

void ABaseCharacter::ProStartGameTimerEnd()
{
	GetWorld()->GetTimerManager().SetTimer(GameTimerEndHandle, this, &ABaseCharacter::ProGameTimerEnd, 1.0f, false);
}

void ABaseCharacter::ProGameTimerEnd()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		// 'quit' 명령을 실행하여 게임 종료
		PlayerController->ConsoleCommand("quit");
	}
}

uint32 ABaseCharacter::GetPlayerId() const
{
	return PlayerId;
}

void ABaseCharacter::SetPlayerId(uint32 NewPlayerId)
{
	PlayerId = NewPlayerId;
}

std::string ABaseCharacter::GetPlayerName() const
{
	return PlayerName;
}

void ABaseCharacter::SetPlayerName(std::string NewPlayerName)
{
	PlayerName = NewPlayerName;
}

void ABaseCharacter::UpdatePlayerData(FVector Location)
{
	NewLocation = Location;
}

void ABaseCharacter::SetAttack(bool bAttack)
{
	b_attack = bAttack;
	//UE_LOG(LogTemp, Warning, TEXT("SetAttack from %d to %d"), b_attack, bAttack);
	if (b_attack)
	{
		Attack();
		b_attack = false;
	}
}

void ABaseCharacter::SetPickUp()
{
	PickUp();
}

void ABaseCharacter::SetPlayerJump()
{
	m_bJump = true;

	UE_LOG(LogTemp, Warning, TEXT("SetPlayerJump"));
}

bool ABaseCharacter::GetAttack()
{
	return b_attack;
}

void ABaseCharacter::OtherSpawnNormalWeapon(const FString& WeaponName)
{
	if (CurrentWeapon != nullptr) {
		DestroyNormalWeapon();
	}
	if (CurrentWeapon == nullptr) {
		if (WeaponName == "Book") { //*
			CurrentWeapon = GetWorld()->SpawnActor<ANWBook>(ANWBook::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(4.963954f, -3.163849f, 1.415202f);
			CurrentWeapon->ItemHandRot = FRotator(-49.169772f, 77.43936f, 19.22938f);
		}

		else if (WeaponName == "Bottle") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWBottle>(ANWBottle::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(-1.46633f, 4.540913f, 3.060996f);
			CurrentWeapon->ItemHandRot = FRotator(-0.000289f, 39.999591f, 79.999289f);
		}

		else if (WeaponName == "ButchersKnife") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWButchersKnife>(ANWButchersKnife::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(-0.757521f, 1.883819f, 1.937180f);
			CurrentWeapon->ItemHandRot = FRotator(-12.172822f, -34.649834f, -158.006864f);
		}

		else if (WeaponName == "FireAxe") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWFireAxe>(ANWFireAxe::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(1.241818f, 0.621437f, -0.044616f);
			CurrentWeapon->ItemHandRot = FRotator(65.529377f, 122.729752f, 303.71118f);
		}


		else if (WeaponName == "FireExtinguisher") { //*
			CurrentWeapon = GetWorld()->SpawnActor<ANWFireExtinguisher>(ANWFireExtinguisher::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(1.346740f, -0.000092f, 16.967230f);
			CurrentWeapon->ItemHandRot = FRotator(-60.000240f, -179.999764f, -179.999233f);
		}


		else if (WeaponName == "FryingPan") { //*
			CurrentWeapon = GetWorld()->SpawnActor<ANWFryingPan>(ANWFryingPan::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(0.499642f, -0.89209f, -1.988601f);
			CurrentWeapon->ItemHandRot = FRotator(29.999483f, -109.999095f, -0.000121f);
		}


		else if (WeaponName == "GolfClub") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWGolfClub>(ANWGolfClub::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(-0.09167f, -1.358051f, 0.250882f);
			CurrentWeapon->ItemHandRot = FRotator(-0.00006f, -99.99988f, -0.000023f);
		}


		else if (WeaponName == "Iron") { //*
			CurrentWeapon = GetWorld()->SpawnActor<ANWIron>(ANWIron::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(-0.00006f, 2.036528f, 3.535635f);
			CurrentWeapon->ItemHandRot = FRotator(0.000322f, 0.001126f, 69.999473f);
		}


		else if (WeaponName == "MagicStick") { //*
			CurrentWeapon = GetWorld()->SpawnActor<ANWMagicStick>(ANWMagicStick::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(12.221718f, -0.955271f, 0.690505f);
			CurrentWeapon->ItemHandRot = FRotator(0.926526f, -4.243007f, -180.634745f);
		}


		else if (WeaponName == "MannequinArm") { //**
			CurrentWeapon = GetWorld()->SpawnActor<ANWMannequinArm>(ANWMannequinArm::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(-1.987761f, -9.628748f, 1.854362f);
			CurrentWeapon->ItemHandRot = FRotator(-30.000611f, -159.998858f, 0.000562f);
		}


		else if (WeaponName == "MannequinLeg") { //**
			CurrentWeapon = GetWorld()->SpawnActor<ANWMannequinLeg>(ANWMannequinLeg::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(-6.11014f, -0.906533f, 3.864085f);
			CurrentWeapon->ItemHandRot = FRotator(80.000082f, -0.000489f, -0.000798f);
		}


		else if (WeaponName == "Pipe") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWPipe>(ANWPipe::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(26.137358f, 13.137087f, 0.643183f);
			CurrentWeapon->ItemHandRot = FRotator(-5.44398f, 17.537948f, -33.423673f);
		}


		else if (WeaponName == "Plunger") { //*
			CurrentWeapon = GetWorld()->SpawnActor<ANWPlunger>(ANWPlunger::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(-8.434205f, -1.096508f, -1.012945f);
			CurrentWeapon->ItemHandRot = FRotator(82.25204f, -119.327506f, -131.361975);
		}


		else if (WeaponName == "SashimiKnife") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWSashimiKnife>(ANWSashimiKnife::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(-6.766818f, 5.129228f, -0.58278f);
			CurrentWeapon->ItemHandRot = FRotator(15.53382f, -60.800216f, 184.87332f);
		}


		else if (WeaponName == "Scissors") { //**
			CurrentWeapon = GetWorld()->SpawnActor<ANWScissors>(ANWScissors::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(-1.193205f, -1.628935f, 2.40875f);
			CurrentWeapon->ItemHandRot = FRotator(20.000335f, -90.000792f, -9.999781f);

		}


		else if (WeaponName == "Shovels") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWShovels>(ANWShovels::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(7.668457f, -0.797488f, -0.431196f);
			CurrentWeapon->ItemHandRot = FRotator(26.807803f, 68.108858f, 84.914172f);
		}


		else if (WeaponName == "SquareWood") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWSquareWood>(ANWSquareWood::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(-11.797672f, 0.419462f, 7.687267f);
			CurrentWeapon->ItemHandRot = FRotator(-6.285888f, -101.190543f, 111.478725f);
		}


		else if (WeaponName == "WoodenBat") {
			CurrentWeapon = GetWorld()->SpawnActor<ANWWoodenBat>(ANWWoodenBat::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
			CurrentWeapon->ItemHandPos = FVector(29.212954f, -3.556313f, -0.245431f);
			CurrentWeapon->ItemHandRot = FRotator(-26.286617f, 81.560518f, 0.445659f);
		}

		else {
			return;
		}

		//UE_LOG(LogTemp, Log, TEXT("Weapon Position: %s"), *CurrentWeapon->ItemHandPos.ToString());
		//UE_LOG(LogTemp, Log, TEXT("Weapon Rotation: %s"), *CurrentWeapon->ItemHandRot.ToString());

		CurrentWeapon->OwnerCharacter = this;
		CurrentWeapon->m_fCharacterSTR = m_fSTR;
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("OtherSpawnNormalWeapon"));

		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		CurrentWeapon->AttachToComponent(GetMesh(), AttachmentRules, FName("RightHandSocket"));

		CurrentWeapon->SetActorRelativeRotation(CurrentWeapon->ItemHandRot);
		CurrentWeapon->SetActorRelativeLocation(CurrentWeapon->ItemHandPos);

	}

	SetBringCurrentWeapon(true);
}

void ABaseCharacter::OtherSpawnHealingItem(const FString& ItemName)
{
	if (CurrentHealingItem != nullptr) {
		DestroyHealingItem();
	}
	if (CurrentHealingItem == nullptr) {

		if (ItemName == "CannedTuna") {
			CurrentHealingItem = GetWorld()->SpawnActor<AHCannedTuna>(AHCannedTuna::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}


		else if (ItemName == "Disinfectant") {
			CurrentHealingItem = GetWorld()->SpawnActor<AHDisinfectant>(AHDisinfectant::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}

		else if (ItemName == "Drink") {
			CurrentHealingItem = GetWorld()->SpawnActor<AHDrink>(AHDrink::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}

		else if (ItemName == "Ointment") {
			CurrentHealingItem = GetWorld()->SpawnActor<AHOintment>(AHOintment::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}

		else if (ItemName == "Smoke") {
			CurrentHealingItem = GetWorld()->SpawnActor<AHSmoke>(AHSmoke::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}

		else if (ItemName == "Snack") {
			CurrentHealingItem = GetWorld()->SpawnActor<AHSnack>(AHSnack::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}

		else if (ItemName == "Water") {
			CurrentHealingItem = GetWorld()->SpawnActor<AHWater>(AHWater::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}

		else {
			return;
		}

		CurrentHealingItem->ItemHandPos = FVector(-1.616189f, -3.900112f, 1.626949f);
		CurrentHealingItem->ItemHandRot = FRotator(-0.005714f, -70.00028f, 90.03065f);

		CurrentHealingItem->OwnerCharacter = this;

		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		CurrentHealingItem->AttachToComponent(GetMesh(), AttachmentRules, FName("RightHandSocket"));

		CurrentHealingItem->SetActorRelativeRotation(CurrentHealingItem->ItemHandRot);
		CurrentHealingItem->SetActorRelativeLocation(CurrentHealingItem->ItemHandPos);
	}

	SetBringCurrentHealingItem(true);
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("SpawnHealingItem"));
}

void ABaseCharacter::OtherSpawnBleedingHealingItem(const FString& ItemName)
{
	if (CurrentBleedingHealingItem != nullptr) {
		DestroyBleedingHealingItem();
	}
	if (CurrentBleedingHealingItem == nullptr) {


		if (ItemName == "Bandage") {
			CurrentBleedingHealingItem = GetWorld()->SpawnActor<ABHBandage>(ABHBandage::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}

		else if (ItemName == "Clothes") {
			CurrentBleedingHealingItem = GetWorld()->SpawnActor<ABHClothes>(ABHClothes::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}

		else if (ItemName == "DirtyClothes") {
			CurrentBleedingHealingItem = GetWorld()->SpawnActor<ABHDirtyClothes>(ABHDirtyClothes::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}

		else if (ItemName == "Gauze") {
			CurrentBleedingHealingItem = GetWorld()->SpawnActor<ABHGauze>(ABHGauze::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}
		else {
			return;
		}

		CurrentBleedingHealingItem->ItemHandPos = FVector(0.211861f, -1.554539f, -1.946655f);
		CurrentBleedingHealingItem->ItemHandRot = FRotator(0.f, 0.f, 0.f);

		CurrentBleedingHealingItem->OwnerCharacter = this;

		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		CurrentBleedingHealingItem->AttachToComponent(GetMesh(), AttachmentRules, FName("RightHandSocket"));

		CurrentBleedingHealingItem->SetActorRelativeRotation(CurrentBleedingHealingItem->ItemHandRot);
		CurrentBleedingHealingItem->SetActorRelativeLocation(CurrentBleedingHealingItem->ItemHandPos);


	}

	SetBringCurrentBleedingHealingItem(true);
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("SpawnBleedingHealingItem"));
}



void ABaseCharacter::OtherSpawnThrowWeapon(const FString& ItemName)
{

}

void ABaseCharacter::OtherSpawnKeyItem(const FString& ItemName)
{
	if (CurrentKeyItem != nullptr) {
		DestroyKeyItem();
	}
	if (CurrentKeyItem == nullptr) {

		if (ItemName == "CarKey1") {
			CurrentKeyItem = GetWorld()->SpawnActor<AKCarkey1>(AKCarkey1::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}

		else if (ItemName == "CarKey2") {
			CurrentKeyItem = GetWorld()->SpawnActor<AKCarKey2>(AKCarKey2::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}

		else if (ItemName == "CarKey3") {
			CurrentKeyItem = GetWorld()->SpawnActor<AKCarKey3>(AKCarKey3::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}

		else if (ItemName == "CarKey4") {
			CurrentKeyItem = GetWorld()->SpawnActor<AKCarKey4>(AKCarKey4::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}

		else if (ItemName == "RoofKey1") {
			CurrentKeyItem = GetWorld()->SpawnActor<AKRoofKey1>(AKRoofKey1::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}

		else if (ItemName == "RoofKey2") {
			CurrentKeyItem = GetWorld()->SpawnActor<AKRoofKey2>(AKRoofKey2::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
		}

		else {
			return;
		}

		CurrentKeyItem->ItemHandPos = FVector(2.595437f, 1.093417f, 2.838135f);
		CurrentKeyItem->ItemHandRot = FRotator(69.997202f, -20.036148f, 29.978793f);

		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		CurrentKeyItem->AttachToComponent(GetMesh(), AttachmentRules, FName("RightHandSocket"));

		CurrentKeyItem->SetActorRelativeRotation(CurrentKeyItem->ItemHandRot);
		CurrentKeyItem->SetActorRelativeLocation(CurrentKeyItem->ItemHandPos);
	}

	SetBringCurrentKeyItem(true);
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("SpawnKeyItem"));
}

void ABaseCharacter::Send_Destroy(uint32 itemboxid)
{
	Protocol::destroy_item Packet;
	Packet.set_itemid(itemboxid);
	Packet.set_playerid(GameInstance->ClientSocketPtr->GetMyPlayerId());
	Packet.set_packet_type(17);

	std::string serializedData;
	Packet.SerializeToString(&serializedData);

	bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());
}

void ABaseCharacter::Send_GetKey(uint32 itemid)
{
	Protocol::get_key Packet;
	Packet.set_itemid(itemid);
	Packet.set_playerid(GameInstance->ClientSocketPtr->GetMyPlayerId());
	Packet.set_packet_type(18);

	std::string serializedData;
	Packet.SerializeToString(&serializedData);

	bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());
}

void ABaseCharacter::Send_OpenRoot(uint32 itemid)
{
	Protocol::escape Packet;
	// itemid 1 = car, 2 = rooftopdoor
	Packet.set_root(itemid);
	Packet.set_packet_type(19);

	std::string serializedData;
	Packet.SerializeToString(&serializedData);

	bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());
}