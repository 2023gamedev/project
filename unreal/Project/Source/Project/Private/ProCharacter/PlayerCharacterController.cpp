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

	ClientSocketPtr = nullptr;

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

	auto GameInstance = Cast<UProGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		this->ClientSocketPtr = GameInstance->ClientSocketPtr;
	}
}

void APlayerCharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckAndSendMovement();

	if (ClientSocketPtr->Q_player.try_pop(recvPlayerData))
	{
		UE_LOG(LogNet, Display, TEXT("Update Other Player: PlayerId=%d"), recvPlayerData.PlayerId);
		// 현재 GameMode 인스턴스를 얻기
		if (AOneGameModeBase* MyGameMode = Cast<AOneGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
		{
			// GameMode 내의 함수 호출하여 다른 플레이어의 위치 업데이트
			MyGameMode->UpdateOtherPlayer(recvPlayerData.PlayerId, recvPlayerData.Location, recvPlayerData.Rotation);
		}
	}
}

void APlayerCharacterController::CheckAndSendMovement()
{
	APawn* MyPawn = GetPawn();
	FVector CurrentLocation = MyPawn->GetActorLocation();
	FRotator CurrentRotation = MyPawn->GetActorRotation();

	
	// 이전 위치와 현재 위치 비교 (움직임 감지)
	if (PreviousLocation != CurrentLocation || PreviousRotation != CurrentRotation)
	{
		uint32 MyPlayerId = ClientSocketPtr->GetMyPlayerId();

		// Protobuf를 사용하여 TestPacket 생성
		Protocol:: Character packet;
		packet.set_playerid(MyPlayerId);
		packet.set_type(1); // 원하는 유형 설정
		packet.set_x(CurrentLocation.X);
		packet.set_y(CurrentLocation.Y);
		packet.set_z(CurrentLocation.Z);
		packet.set_pitch(CurrentRotation.Pitch);
		packet.set_yaw(CurrentRotation.Yaw);
		packet.set_roll(CurrentRotation.Roll);

		// 직렬화
		std::string serializedData;
		packet.SerializeToString(&serializedData);

		// 직렬화된 데이터를 서버로 전송
		bool bIsSent = ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());

		// 현재 위치를 이전 위치로 업데이트
		PreviousLocation = CurrentLocation;
		PreviousRotation = CurrentRotation;
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
				PEI->BindAction(InputActions->InputAttack, ETriggerEvent::Completed, this, &APlayerCharacterController::Attck);

				PEI->BindAction(InputActions->InputQuickNWeapon, ETriggerEvent::Completed, this, &APlayerCharacterController::QuickNWeapon);
				PEI->BindAction(InputActions->InputQuickTWeapon, ETriggerEvent::Completed, this, &APlayerCharacterController::QuickTWeapon);
				PEI->BindAction(InputActions->InputQuickBHItem, ETriggerEvent::Completed, this, &APlayerCharacterController::QuickBHItem);
				PEI->BindAction(InputActions->InputQuickHItem, ETriggerEvent::Completed, this, &APlayerCharacterController::QuickHItem);
				 
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

	basecharacter->LookUp(LookAxisVector.Y);
}

void APlayerCharacterController::Run(const FInputActionValue& Value)
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());
	basecharacter->Run();
}

void APlayerCharacterController::Jump(const FInputActionValue& Value)
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());
	basecharacter->Jump();
}

void APlayerCharacterController::GetItem(const FInputActionValue& Value)
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());
	basecharacter->GetItem();
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

void APlayerCharacterController::Attck(const FInputActionValue& Value)
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());
	basecharacter->Attack();
}

void APlayerCharacterController::QuickNWeapon(const FInputActionValue& Value)
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());

	UE_LOG(LogTemp, Error, TEXT("QuickNWeapon"));

	basecharacter->QuickNWeapon();
}

void APlayerCharacterController::QuickTWeapon(const FInputActionValue& Value)
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());


	UE_LOG(LogTemp, Error, TEXT("QuickTWeapon"));


	basecharacter->QuickTWeapon();
}

void APlayerCharacterController::QuickBHItem(const FInputActionValue& Value)
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());

	UE_LOG(LogTemp, Error, TEXT("QuickBHItem"));


	basecharacter->QuickBHItem();

}

void APlayerCharacterController::QuickHItem(const FInputActionValue& Value)
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());

	UE_LOG(LogTemp, Error, TEXT("QuickHItem"));

	basecharacter->QuickHItem();
}

