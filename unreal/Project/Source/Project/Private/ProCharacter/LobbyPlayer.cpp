// Fill out your copyright notice in the Description page of Project Settings.


#include "ProCharacter/LobbyPlayer.h"
#include "ProUI/StartGameUI.h"
#include "ProUI/Select_RoomUI.h"
#include "ProUI/ChoiceCharacterUI.h"
#include "ProUI/LoginUI.h"
#include "ProUI/WaitingRoomUI.h"
#include "Kismet/GameplayStatics.h"
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

	static ConstructorHelpers::FClassFinder <ULoginUI> PLAYER_LOGINUI(TEXT("/Game/UI/BP_LoginUI.BP_LoginUI_C"));

	if (PLAYER_LOGINUI.Succeeded()) {
		LoginUI = PLAYER_LOGINUI.Class;
	}

	/*static ConstructorHelpers::FClassFinder <UStartGameUI> PLAYER_STARTUI(TEXT("/Game/UI/BP_StartGameUI.BP_StartGameUI_C"));

	if (PLAYER_STARTUI.Succeeded()) {
		StartGameUI = PLAYER_STARTUI.Class;
	}*/

	static ConstructorHelpers::FClassFinder <USelect_RoomUI> PLAYER_SELECT_ROOMUI(TEXT("/Game/UI/BP_SelectRoom.BP_SelectRoom_C"));

	if (PLAYER_SELECT_ROOMUI.Succeeded()) {
		StartGameUI = PLAYER_SELECT_ROOMUI.Class;
	}

	static ConstructorHelpers::FClassFinder <UWaitingRoomUI> PLAYER_WAITINGROOMUI(TEXT("/Game/UI/BP_WaitingRoom.BP_WaitingRoom_C"));

	if (PLAYER_WAITINGROOMUI.Succeeded()) {
		WaitingRoomUI = PLAYER_WAITINGROOMUI.Class;
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

	GameInstance = Cast<UProGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	// choicecharacterui를 먼저 생성할 때
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
	
	// StartGameUI를 먼저 생성할 때
	//if (StartGameUI != nullptr) {
	//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "StartGameUI != nullptr");
	//	ALobbyPlayerController* controller = Cast<ALobbyPlayerController>(this->GetController());
	//	if (controller == nullptr) {
	//		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "controller == nullptr");
	//		return;
	//	}
	//	StartGameUIWidget = CreateWidget<UStartGameUI>(controller, StartGameUI);

	//	if (!StartGameUIWidget) {
	//		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "!StartGameUIWidget");
	//		return;
	//	}

	//	StartGameUIWidget->Init();
	//	StartGameUIWidget->AddToViewport();


	//	StartGameUIWidget->MoveChoiceCharacterUI.BindUObject(this, &ALobbyPlayer::MoveChoiceCharacterUI);
	//}
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "LoginUI 123");

	// LoginUI를 먼저 생성할 때
	if (LoginUI != nullptr) {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "LoginUI != nullptr");
		ALobbyPlayerController* controller = Cast<ALobbyPlayerController>(this->GetController());
		if (controller == nullptr) {
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "controller == nullptr");
			return;
		}
		LoginUIWidget = CreateWidget<ULoginUI>(controller, LoginUI);

		if (!LoginUIWidget) {
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "!LoginUIWidget");
			return;
		}

		LoginUIWidget->Init();
		LoginUIWidget->AddToViewport();


		LoginUIWidget->MoveStartGameUI.BindUObject(this, &ALobbyPlayer::MoveStartGameUI);
	}



}

// Called every frame
void ALobbyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GameInstance->ClientSocketPtr->Q_select.try_pop(recvSelect)){
		choicecharacterui->UpdateSelectImage(recvSelect);
	}

    if (GameInstance->ClientSocketPtr->b_allready){
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "All players are ready!");

		choicecharacterui->RemoveFromParent();

        if (ALobbyGameMode* MyGameMode = Cast<ALobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
        {
            MyGameMode->LobbyStageClear();
        }
    }

	if (GameInstance->ClientSocketPtr->Q_chat.try_pop(recvChat)) {
		if (WaitingRoomUIWidget)
		{
			WaitingRoomUIWidget->AddChatMessage(recvChat.chat);
		}
	}

	if (GameInstance->ClientSocketPtr->Q_jplayer.try_pop(recvJplayer)) {
		if (WaitingRoomUIWidget)
		{
			FString FStringname = FString(UTF8_TO_TCHAR(recvJplayer.username.c_str()));
			WaitingRoomUIWidget->AddPlayerToList(FStringname);
		}
	}

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

void ALobbyPlayer::MoveStartGameUI()
{
	if (StartGameUI != nullptr) {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "StartGameUI != nullptr");
		ALobbyPlayerController* controller = Cast<ALobbyPlayerController>(this->GetController());
		if (controller == nullptr) {
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "controller == nullptr");
			return;
		}
		StartGameUIWidget = CreateWidget<USelect_RoomUI>(controller, StartGameUI);

		if (!StartGameUIWidget) {
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "!StartGameUIWidget");
			return;
		}

		//StartGameUIWidget->RemoveFromViewport();



		StartGameUIWidget->Init();
		StartGameUIWidget->AddToViewport();

		StartGameUIWidget->MoveWaitingRoomUI.BindUObject(this, &ALobbyPlayer::MoveWaitingRoomUI);

	}

}

void ALobbyPlayer::MoveWaitingRoomUI()
{
	if (WaitingRoomUI != nullptr) {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "WaitingRoomUI != nullptr");
		ALobbyPlayerController* controller = Cast<ALobbyPlayerController>(this->GetController());
		if (controller == nullptr) {
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "controller == nullptr");
			return;
		}
		WaitingRoomUIWidget = CreateWidget<UWaitingRoomUI>(controller, WaitingRoomUI);

		if (!WaitingRoomUIWidget) {
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "!WaitingRoomUIWidget");
			return;
		}



		WaitingRoomUIWidget->Init();
		WaitingRoomUIWidget->AddToViewport();

		WaitingRoomUIWidget->MoveChoiceCharacterUI.BindUObject(this, &ALobbyPlayer::MoveChoiceCharacterUI);

	}

}

void ALobbyPlayer::OnMouseLeftClick()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Left Mouse Button Clicked");
	if (StartGameUIWidget) {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, "Left Mouse Button Clicked");

		//StartGameUIWidget->StartButton->OnClicked.Broadcast();
		
	}
}



