// Fill out your copyright notice in the Description page of Project Settings.


#include "ProCharacter/LobbyPlayer.h"
#include "ProUI/StartGameUI.h"
#include "ProUI/ChoiceCharacterUI.h"
#include "ProCharacter/LobbyPlayerController.h"

// Sets default values
ALobbyPlayer::ALobbyPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE"));


	RootComponent = Capsule;
	Camera->SetupAttachment(Capsule);


	static ConstructorHelpers::FClassFinder <UStartGameUI> PLAYER_STARTUI(TEXT("/Game/UI/BP_StartGameUI.BP_StartGameUI_C"));

	if (PLAYER_STARTUI.Succeeded()) {
		StartGameUI = PLAYER_STARTUI.Class;
	}

	static ConstructorHelpers::FClassFinder <UChoiceCharacterUI> PLAYER_CHOICEUI(TEXT("/Game/UI/BP_ChoiceCharacterUI.BP_ChoiceCharacterUI_C"));

	if (PLAYER_CHOICEUI.Succeeded()) {
		ChoiceCharacterUI = PLAYER_CHOICEUI.Class;
	}

}

// Called when the game starts or when spawned
void ALobbyPlayer::BeginPlay()
{
	Super::BeginPlay();

	//if (ChoiceCharacterUI != nullptr) {
	//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "ChoiceCharacterUI != nullptr");
	//	ALobbyPlayerController* controller = Cast<ALobbyPlayerController>(this->GetController());
	//	if (controller == nullptr) {
	//		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "controller == nullptr");
	//		return;
	//	}
	//	ChoiceCharacterUIWidget = CreateWidget<UChoiceCharacterUI>(controller, ChoiceCharacterUI);

	//	if (!ChoiceCharacterUIWidget) {
	//		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "!ChoiceCharacterUIWidget");
	//		return;
	//	}

	//	ChoiceCharacterUIWidget->Init();
	//	ChoiceCharacterUIWidget->AddToViewport();

	//	ChoiceCharacterUIWidget->ChoicedGirl.BindUObject(this, &ALobbyPlayer::ChoicedGirlCharacter);
	//	ChoiceCharacterUIWidget->ChoicedEmployee.BindUObject(this, &ALobbyPlayer::ChoicedEmployeeCharacter);
	//	ChoiceCharacterUIWidget->ChoicedIdol.BindUObject(this, &ALobbyPlayer::ChoicedIdolCharacter);
	//	ChoiceCharacterUIWidget->ChoicedFireFighter.BindUObject(this, &ALobbyPlayer::ChoicedFireFighterCharacter);

	//}
	 
	if (StartGameUI != nullptr) {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "StartGameUI != nullptr");
		ALobbyPlayerController* controller = Cast<ALobbyPlayerController>(this->GetController());
		if (controller == nullptr) {
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "controller == nullptr");
			return;
		}
		StartGameUIWidget = CreateWidget<UStartGameUI>(controller, StartGameUI);

		if (!StartGameUIWidget) {
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "!StartGameUIWidget");
			return;
		}

		StartGameUIWidget->Init();
		StartGameUIWidget->AddToViewport();


		StartGameUIWidget->MoveChoiceCharacterUI.BindUObject(this, &ALobbyPlayer::MoveChoiceCharacterUI);
	}



}

// Called every frame
void ALobbyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALobbyPlayer::ChoicedGirlCharacter()
{
	ChoicedGirlCharcaterD.Execute();
}

void ALobbyPlayer::ChoicedEmployeeCharacter()
{
	ChoicedEmployeeCharcaterD.Execute();
}

void ALobbyPlayer::ChoicedIdolCharacter()
{
	ChoicedIdolCharcaterD.Execute();
}

void ALobbyPlayer::ChoicedFireFighterCharacter()
{
	ChoicedFireFighterCharcaterD.Execute();
}

void ALobbyPlayer::MoveChoiceCharacterUI()
{
	if (ChoiceCharacterUI != nullptr) {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "ChoiceCharacterUI != nullptr");
		ALobbyPlayerController* controller = Cast<ALobbyPlayerController>(this->GetController());
		if (controller == nullptr) {
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "controller == nullptr");
			return;
		}
		ChoiceCharacterUIWidget = CreateWidget<UChoiceCharacterUI>(controller, ChoiceCharacterUI);

		if (!ChoiceCharacterUIWidget) {
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "!ChoiceCharacterUIWidget");
			return;
		}

		//StartGameUIWidget->RemoveFromViewport();

		ChoiceCharacterUIWidget->Init();
		ChoiceCharacterUIWidget->AddToViewport();

		ChoiceCharacterUIWidget->ChoicedGirl.BindUObject(this, &ALobbyPlayer::ChoicedGirlCharacter);
		ChoiceCharacterUIWidget->ChoicedEmployee.BindUObject(this, &ALobbyPlayer::ChoicedEmployeeCharacter);
		ChoiceCharacterUIWidget->ChoicedIdol.BindUObject(this, &ALobbyPlayer::ChoicedIdolCharacter);
		ChoiceCharacterUIWidget->ChoicedFireFighter.BindUObject(this, &ALobbyPlayer::ChoicedFireFighterCharacter);

	}

}

void ALobbyPlayer::OnMouseLeftClick()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Left Mouse Button Clicked");
	if (StartGameUIWidget) {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Left Mouse Button Clicked");

		StartGameUIWidget->StartButton->OnClicked.Broadcast();
		
	}
}


//void ALobbyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}


