// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacterController.h"

// Engine
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

// GameMode
#include "OneGameModeBase.h"

// Character
#include "BaseCharacter.h"
#include "EmployeeCharacter.h"
#include "GirlCharacter.h"
#include "IdolCharacter.h"
#include "FireFighterCharacter.h"



// EnhancedInput
#include "InputMappingContext.h"
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
}

void APlayerCharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	//UEnhancedInputLocalPlayerSubsystem* PlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	//PlayerSubsystem->ClearAllMappings();
	//PlayerSubsystem->AddMappingContext(InputMapping, 0);


	//UEnhancedInputComponent* PEI = Cast<UEnhancedInputComponent>(InputComponent);

	//PEI->BindAction(InputActions->InputMoveForward, ETriggerEvent::Triggered, this, &APlayerCharacterController::MoveForward);
	//PEI->BindAction(InputActions->InputMoveLeft, ETriggerEvent::Triggered, this, &APlayerCharacterController::MoveLeft);
	//PEI->BindAction(InputActions->InputTurn, ETriggerEvent::Triggered, this, &APlayerCharacterController::Turn);
	//PEI->BindAction(InputActions->InputLookUp, ETriggerEvent::Triggered, this, &APlayerCharacterController::LookUp);
	//PEI->BindAction(InputActions->InputRun, ETriggerEvent::Completed, this, &APlayerCharacterController::Run);
	//PEI->BindAction(InputActions->InputGetItem, ETriggerEvent::Completed, this, &APlayerCharacterController::GetItem);
	//PEI->BindAction(InputActions->InputLightOnOff, ETriggerEvent::Completed, this, &APlayerCharacterController::LightOnOff);
	//PEI->BindAction(InputActions->InputInventoryOnOff, ETriggerEvent::Completed, this, &APlayerCharacterController::InventoryOnOff);
	//PEI->BindAction(InputActions->InputJump, ETriggerEvent::Completed, this, &APlayerCharacterController::Jump);

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
	basecharacter->MoveForward(MovementVector.Y);
}

void APlayerCharacterController::MoveLeft(const FInputActionValue& Value)
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());

	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// find out which way is forward
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	//const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// add movement 
	//basecharacter->AddMovementInput(ForwardDirection, MovementVector.Y);
	basecharacter->MoveLeft(MovementVector.Y);

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

