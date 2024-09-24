// Fill out your copyright notice in the Description page of Project Settings.

#include "ProCharacter/PlayerCharacterController.h"

// Engine
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

// GameMode
#include "ProGamemode/OneGameModeBase.h"
#include "ProGamemode/ProGameInstance.h"

// Character
#include "ProCharacter/BaseCharacter.h"
#include "ProCharacter/EmployeeCharacter.h"
#include "ProCharacter/GirlCharacter.h"
#include "ProCharacter/IdolCharacter.h"
#include "ProCharacter/FireFighterCharacter.h"

#include "ProZombie/BaseZombie.h"

//Network
#include "GStruct.pb.h"



// EnhancedInput
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"


APlayerCharacterController::APlayerCharacterController(const FObjectInitializer& ObjectInitalizer)
	:Super(ObjectInitalizer)
{
	InputMapping = CreateDefaultSubobject<UInputMappingContext>(TEXT("INPUTMAPPING"));

	InputActions = CreateDefaultSubobject<UInputDataAsset>(TEXT("INPUTACTIONS"));

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> SK_INPUTMAPPINGCONTEXT(TEXT("/Game/input/IMC_InputMappingCon.IMC_InputMappingCon"));

	if (SK_INPUTMAPPINGCONTEXT.Succeeded()) {
		InputMapping = SK_INPUTMAPPINGCONTEXT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputDataAsset> SK_INPUTDATAASSET(TEXT("/Game/input/BP_InputDataAsset.BP_InputDataAsset"));

	if (SK_INPUTDATAASSET.Succeeded()) {
		InputActions = SK_INPUTDATAASSET.Object;
	}

}

void APlayerCharacterController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void APlayerCharacterController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<UProGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

void APlayerCharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckAndSendMovement();
	Send_Attack();
	Send_Equipment();
	Send_run();
	Send_jump();

	if (GameInstance && GameInstance->ClientSocketPtr)
	{
		if (GameInstance->ClientSocketPtr->Q_player.try_pop(recvPlayerData))
		{
			//UE_LOG(LogNet, Display, TEXT("Update Other Player: PlayerId=%d"), recvPlayerData.PlayerId);
			// 현재 GameMode 인스턴스를 얻기
			if (AOneGameModeBase* MyGameMode = Cast<AOneGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				// GameMode 내의 함수 호출하여 다른 플레이어의 위치 업데이트
				MyGameMode->UpdateOtherPlayer(recvPlayerData.PlayerId, recvPlayerData.Location, recvPlayerData.Rotation, recvPlayerData.charactertype,
					recvPlayerData.hp);
			}
		}

		if (GameInstance->ClientSocketPtr->Q_pattack.try_pop(recvPlayerAttack))
		{
			if (AOneGameModeBase* MyGameMode = Cast<AOneGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				MyGameMode->UpdatePlayerAttack(recvPlayerAttack.PlayerId, recvPlayerAttack.b_attack);
				//UE_LOG(LogNet, Display, TEXT("Update Other Player: PlayerId=%d"), recvPlayerData.PlayerId);
			}
		}

		if (GameInstance->ClientSocketPtr->Q_eitem.try_pop(recvEquipItem))
		{
			if (AOneGameModeBase* MyGameMode = Cast<AOneGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				FString EquipItem = *FString(recvEquipItem.Itemname.c_str());
				MyGameMode->UpdateEquipItem(recvEquipItem.PlayerId, EquipItem, recvEquipItem.Itemtype);
				UE_LOG(LogNet, Display, TEXT("Update Other Player: PlayerId=%d"), recvEquipItem.PlayerId);
			}
		}

		if (GameInstance->ClientSocketPtr->Q_run.try_pop(recvRun))
		{
			if (AOneGameModeBase* MyGameMode = Cast<AOneGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				MyGameMode->UpdatePlayerRun(recvRun.PlayerId, recvRun.b_run);
				//UE_LOG(LogNet, Display, TEXT("Update Other Player: PlayerId=%d"), recvPlayerData.PlayerId);
			}
		}

		if (GameInstance->ClientSocketPtr->Q_jump.try_pop(recvJump)) {
			if (AOneGameModeBase* MyGameMode = Cast<AOneGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				MyGameMode->UpdatePlayerJump(recvJump.PlayerId);
			}
		}

		//UE_LOG(LogNet, Display, TEXT("Update call Zombie: Playerid=%d"), GameInstance->ClientSocketPtr->MyPlayerId);
		if (GameInstance->ClientSocketPtr->Q_zombie.try_pop(recvZombieData))
		{
			//UE_LOG(LogNet, Display, TEXT("try_pop Zombie: ZombieId=%d"), recvZombieData.ZombieId);
			if (AOneGameModeBase* MyGameMode = Cast<AOneGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				MyGameMode->UpdateZombie(recvZombieData.ZombieId, recvZombieData.ZombieType, recvZombieData.Location, recvZombieData.Rotation);
				UE_LOG(LogNet, Display, TEXT("Update call Zombie: Playerid=%d"), GameInstance->ClientSocketPtr->PlayerId);
			}
		}

		if (GameInstance->ClientSocketPtr->Q_zattack.try_pop(ZombieId))
		{
			//UE_LOG(LogNet, Display, TEXT("try_pop Zombie: ZombieId=%d"), recvZombieData.ZombieId);
			if (AOneGameModeBase* MyGameMode = Cast<AOneGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				MyGameMode->UpdateZombieAttack(ZombieId);
				UE_LOG(LogNet, Display, TEXT("Update Zombie Attack: Zombieid=%d"), ZombieId);
			}
		}

		bool recvping = false;
		if (GameInstance->ClientSocketPtr->Q_ping.try_pop(recvping))
		{
			if (recvping)
			{
				Protocol::ping packet;

				uint32 MyPlayerId = GameInstance->ClientSocketPtr->GetMyPlayerId();

				packet.set_packet_type(11);
				packet.set_playerid(MyPlayerId);

				std::string serializedData;
				packet.SerializeToString(&serializedData);

				bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());
			}
		}
	}


	// 좀비 관련 이지만 일단 좀비 관련 싱글턴 객체가 딱히 없어 - 여기에서 작업
	// 좀비 관련 네트워크 데이터 각 좀비 객체에 할당

	if (GameInstance->ClientSocketPtr->Q_path.empty()) {
		return;
	}
	while (GameInstance->ClientSocketPtr->Q_path.empty() != true) {
		ZombiePath tmp_path;
		GameInstance->ClientSocketPtr->Q_path.try_pop(tmp_path);

		ABaseZombie** zombie = ZombieMap.Find(tmp_path.ZombieId);

		if(tmp_path.Path.empty() == false)
			(*zombie)->NextPath = *(tmp_path.Path.begin());
	}

}

void APlayerCharacterController::CheckAndSendMovement()
{
	APawn* MyPawn = GetPawn();
	FVector CurrentLocation = MyPawn->GetActorLocation();
	FRotator CurrentRotation = MyPawn->GetActorRotation();

	ABaseCharacter* MyBaseCharacter = Cast<ABaseCharacter>(MyPawn);
	uint32 ItemBoxId = MyBaseCharacter->ItemBoxId;
	float hp = MyBaseCharacter->GetHP();
	
	// 이전 위치와 현재 위치 비교 (움직임 감지)
	if (PreviousLocation != CurrentLocation || PreviousRotation != CurrentRotation || b_GetItem || PreviouHP != hp) {
		uint32 MyPlayerId = GameInstance->ClientSocketPtr->GetMyPlayerId();
		MyCharacterNumber = GameInstance->GetChoicedCharacterNumber();

		// Protobuf를 사용하여 TestPacket 생성
		Protocol::Character packet;
		packet.set_playerid(MyPlayerId);
		packet.set_packet_type(1); // 원하는 유형 설정
		packet.set_charactertype(MyCharacterNumber);
		packet.set_x(CurrentLocation.X);
		packet.set_y(CurrentLocation.Y);
		packet.set_z(CurrentLocation.Z);
		packet.set_pitch(CurrentRotation.Pitch);
		packet.set_yaw(CurrentRotation.Yaw);
		packet.set_roll(CurrentRotation.Roll);
		packet.set_hp(hp);
		packet.set_isingame(true);

		// 직렬화
		std::string serializedData;
		packet.SerializeToString(&serializedData);

		// 직렬화된 데이터를 서버로 전송
		bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());

		// 현재 위치를 이전 위치로 업데이트
		PreviousLocation = CurrentLocation;
		PreviousRotation = CurrentRotation;
		PreviouHP = hp;
		b_GetItem = false;
	}
}

void APlayerCharacterController::Send_Attack()
{
	if (b_attack) {
		uint32 MyPlayerId = GameInstance->ClientSocketPtr->GetMyPlayerId();

		Protocol::Character_Attack packet;
		packet.set_playerid(MyPlayerId);
		packet.set_packet_type(4);
		packet.set_attack(b_attack);

		// 직렬화
		std::string serializedData;
		packet.SerializeToString(&serializedData);

		bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());

		b_attack = false;

		UE_LOG(LogNet, Display, TEXT("Send Attack: PlayerId=%d"), recvPlayerData.PlayerId);
	}
}

void APlayerCharacterController::Send_Equipment()
{
	if (e_NWeapon || e_TWeapon || e_BHItem || e_HItem || e_KeyItem)
	{
		APawn* MyPawn = GetPawn();
		ABaseCharacter* MyBaseCharacter = Cast<ABaseCharacter>(MyPawn);
		uint32 MyPlayerId = GameInstance->ClientSocketPtr->GetMyPlayerId();
		uint32 ItemType; // 0 = BHItem, 1 = HItem, 2 = TWeapon, 3 = KeyItem, 4 = NWeapon

		if (e_BHItem)
		{
			ItemName = TCHAR_TO_UTF8(*(MyBaseCharacter->QuickSlot[0].Name.ToString()));
			ItemType = 0;
		}
		if (e_HItem)
		{
			ItemName = TCHAR_TO_UTF8(*(MyBaseCharacter->QuickSlot[1].Name.ToString()));
			ItemType = 1;
		}
		if (e_TWeapon)
		{
			ItemName = TCHAR_TO_UTF8(*(MyBaseCharacter->QuickSlot[2].Name.ToString()));
			ItemType = 2;
		}
		if (e_KeyItem)
		{
			ItemName = TCHAR_TO_UTF8(*(MyBaseCharacter->QuickSlot[3].Name.ToString()));
			ItemType = 3;
		}
		if (e_NWeapon)
		{
			ItemName = TCHAR_TO_UTF8(*(MyBaseCharacter->QuickSlot[4].Name.ToString()));
			ItemType = 4;
		}

		Protocol::Equip_Item packet;
		packet.set_playerid(MyPlayerId);
		packet.set_packet_type(5);
		packet.set_itemtype(ItemType);
		packet.set_itemname(ItemName);

		// 직렬화
		std::string serializedData;
		packet.SerializeToString(&serializedData);

		bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());
		UE_LOG(LogNet, Display, TEXT("Send Equip: PlayerId=%d"), recvPlayerData.PlayerId);
		e_NWeapon = false;
		e_TWeapon = false;
		e_BHItem = false; 
		e_HItem = false;
		e_KeyItem = false;
	}
}

void APlayerCharacterController::Send_run() {
	if (sendRun) {
		uint32 MyPlayerId = GameInstance->ClientSocketPtr->GetMyPlayerId();

		Protocol::run packet;
		packet.set_playerid(MyPlayerId);
		packet.set_b_run(b_run);
		packet.set_packet_type(6);

		// 직렬화
		std::string serializedData;
		packet.SerializeToString(&serializedData);
		bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());

		sendRun = false;
	}
}

void APlayerCharacterController::Send_jump() {
	if (sendjump) {
		uint32 MyPlayerId = GameInstance->ClientSocketPtr->GetMyPlayerId();

		Protocol::jump packet;
		packet.set_playerid(MyPlayerId);
		packet.set_packet_type(7);

		// 직렬화
		std::string serializedData;
		packet.SerializeToString(&serializedData);
		bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());

		sendjump = false;
	}
}
 
void APlayerCharacterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Get the player local subsystem
	ULocalPlayer* LocalPlayer = GetLocalPlayer();

	if (LocalPlayer)
	{
		UEnhancedInputLocalPlayerSubsystem* PlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
		if (PlayerSubsystem)
		{            // Player subsystem is valid, continue with the setup
			PlayerSubsystem->ClearAllMappings();
			PlayerSubsystem->AddMappingContext(InputMapping, 0);


			// Bind input actions
			UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(InputComponent);
			if (PEI)
			{

				PEI->BindAction(InputActions->InputMoveForward, ETriggerEvent::Triggered, this, &APlayerCharacterController::MoveForward);
				PEI->BindAction(InputActions->InputMoveLeft, ETriggerEvent::Triggered, this, &APlayerCharacterController::MoveLeft);
				PEI->BindAction(InputActions->InputTurn, ETriggerEvent::Triggered, this, &APlayerCharacterController::Turn);
				PEI->BindAction(InputActions->InputLookUp, ETriggerEvent::Triggered, this, &APlayerCharacterController::LookUp);
				PEI->BindAction(InputActions->InputRun, ETriggerEvent::Completed, this, &APlayerCharacterController::Run);
				PEI->BindAction(InputActions->InputGetItem, ETriggerEvent::Completed, this, &APlayerCharacterController::GetItem);
				PEI->BindAction(InputActions->InputLightOnOff, ETriggerEvent::Completed, this, &APlayerCharacterController::LightOnOff);
				PEI->BindAction(InputActions->InputInventoryOnOff, ETriggerEvent::Completed, this, &APlayerCharacterController::InventoryOnOff);
				PEI->BindAction(InputActions->InputJump, ETriggerEvent::Completed, this, &APlayerCharacterController::Jump);
				PEI->BindAction(InputActions->InputAttack, ETriggerEvent::Completed, this, &APlayerCharacterController::BehaviorToItem);

				PEI->BindAction(InputActions->InputQuickNWeapon, ETriggerEvent::Completed, this, &APlayerCharacterController::QuickNWeapon);
				PEI->BindAction(InputActions->InputQuickTWeapon, ETriggerEvent::Completed, this, &APlayerCharacterController::QuickTWeapon);
				PEI->BindAction(InputActions->InputQuickBHItem, ETriggerEvent::Completed, this, &APlayerCharacterController::QuickBHItem);
				PEI->BindAction(InputActions->InputQuickHItem, ETriggerEvent::Completed, this, &APlayerCharacterController::QuickHItem);
				PEI->BindAction(InputActions->InputQuickKeyItem, ETriggerEvent::Completed, this, &APlayerCharacterController::QuickKeyItem);
				 
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to cast InputComponent to UEnhancedInputComponent"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to get EnhancedInputLocalPlayerSubsystem"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to get LocalPlayer"));
	}
}

void APlayerCharacterController::MoveForward(const FInputActionValue& Value)
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());

	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();


	// find out which way is forward
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// add movement 
	basecharacter->MoveForward(ForwardDirection, MovementVector.Y);
}

void APlayerCharacterController::MoveLeft(const FInputActionValue& Value)
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());

	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// find out which way is forward
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// add movement 
	basecharacter->MoveLeft(RightDirection, MovementVector.Y);

}

void APlayerCharacterController::Turn(const FInputActionValue& Value)
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());

	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	basecharacter->Turn(LookAxisVector.Y);
}

void APlayerCharacterController::LookUp(const FInputActionValue& Value)
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());

	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	basecharacter->LookUp(-LookAxisVector.Y);
}

void APlayerCharacterController::Run(const FInputActionValue& Value)
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());
	basecharacter->Run();
	sendRun = true;

	if (!b_run) {
		b_run = true;
	}
	else b_run = false;
}

void APlayerCharacterController::Jump(const FInputActionValue& Value)
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());
	
	if (basecharacter->GetStamina() >= 20) {
		if (!basecharacter->GetMovementComponent()->IsFalling()) {
			basecharacter->Jump();
			basecharacter->SetStamina(basecharacter->GetStamina() - 20);
			sendjump = true;

			// 플레이어가 처음에 뛰는 키보다 jump키를 먼저 눌렀을 경우 예외상황 IF문
			if (!basecharacter->IsRun()) {

				basecharacter->HealingStamina();
			}
		}
	}

}

void APlayerCharacterController::GetItem(const FInputActionValue& Value)
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());
	basecharacter->GetItem();
	b_GetItem = true;
}

void APlayerCharacterController::LightOnOff(const FInputActionValue& Value)
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());
	basecharacter->LightOnOff();
}

void APlayerCharacterController::InventoryOnOff(const FInputActionValue& Value)
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());
	basecharacter->InventoryOnOff();
}

void APlayerCharacterController::BehaviorToItem(const FInputActionValue& Value)
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());

	if (m_bIsInputEnabled) {
		m_bIsInputEnabled = false;


		// 타이머 설정 (따닥을 방지하기 위해 설정)
		GetWorld()->GetTimerManager().SetTimer(InputCoolTimeHandle, this, &APlayerCharacterController::InputCoolTime, 0.2f, false);


		if (basecharacter->IsNWHandIn() && !(basecharacter->IsAttack())) { // 아예 함수에 접근을 못하게 조건을 추가
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("PlayerController Attack")));
			Attack();
		}
		else if (basecharacter->IsBHHandIn() && !(basecharacter->IsBHealing())) {
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("PlayerController BH")));
			BleedHealing();

		}
		else if (basecharacter->IsHealHandIn() && !(basecharacter->IsHealing())) {
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("PlayerController HHHHHHHHH")));

			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("PlayerController HHHHHHHHH22222")));
			Healing();

		}
		else if (basecharacter->IsKeyHandIn()) {
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("PlayerController Key")));
			PlayKey();

		}
		else if (basecharacter->IsThrowWHandIn()) {
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("PlayerController TH")));
			Throw();
		}

	}

}

void APlayerCharacterController::Attack()
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());
	basecharacter->SetAttack(true);
	b_attack = true;
	UE_LOG(LogTemp, Warning, TEXT("AttackStart: %d"), GameInstance->ClientSocketPtr->GetMyPlayerId());
}

void APlayerCharacterController::Healing()
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());

	basecharacter->Healing();
}

void APlayerCharacterController::BleedHealing()
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());

	basecharacter->BleedHealing();
}

void APlayerCharacterController::PlayKey()
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());

	basecharacter->PlayKey();
}

void APlayerCharacterController::Throw()
{
}



void APlayerCharacterController::QuickNWeapon(const FInputActionValue& Value)
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());

	UE_LOG(LogTemp, Error, TEXT("QuickNWeapon"));

	basecharacter->QuickNWeapon();

	e_NWeapon = true;
}

void APlayerCharacterController::QuickTWeapon(const FInputActionValue& Value)
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());


	UE_LOG(LogTemp, Error, TEXT("QuickTWeapon"));


	basecharacter->QuickTWeapon();

	e_TWeapon = true;
}

void APlayerCharacterController::QuickBHItem(const FInputActionValue& Value)
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());

	UE_LOG(LogTemp, Error, TEXT("QuickBHItem"));


	basecharacter->QuickBHItem();

	e_BHItem = true;

}

void APlayerCharacterController::QuickHItem(const FInputActionValue& Value)
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());

	UE_LOG(LogTemp, Error, TEXT("QuickHItem"));

	basecharacter->QuickHItem();

	e_HItem = true;
}

void APlayerCharacterController::QuickKeyItem(const FInputActionValue& Value)
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());

	UE_LOG(LogTemp, Error, TEXT("QuickKeyItem"));

	basecharacter->QuickKeyItem();

	e_KeyItem = true;
}



void APlayerCharacterController::InputCoolTime()
{
	m_bIsInputEnabled = true;
}

void APlayerCharacterController::DisabledControllerInput()
{
	DisableInput(this);
}

void APlayerCharacterController::ServerHandleAttack()
{
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetCharacter());
	if (BaseCharacter)
	{
		BaseCharacter->SetAttack(true);
	}
}
