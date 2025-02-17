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
#include "ProCharacter/PlayerCharacterAnimInstance.h"

// Zombie
#include "ProZombie/BaseZombie.h"

//Network
#include "GStruct.pb.h"

#include "ProUI/GameChatUI.h"

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


	// 캐릭터 움직임 통신 작업
	TimeSinceLastSend += DeltaTime; // 시간 누적

	if (TimeSinceLastSend >= 0.011f) // 0.011초마다 전송 실행 
		// (약 90분에 1초 => 최소 90 프레임이 방어되면 캐릭터 움직임 딜레이되는 일 없음 -> 딜레이 발생하더라도 아래에서 큐에 2개 이상 쌓이면 모두 빼도록 처리해둠)
	{
		CheckAndSendMovement();
		TimeSinceLastSend = 0.0f; // 초기화
	}

	Check_run();
	Send_Attack();
	Send_Equipment();
	//Send_run();
	Send_jump();


	// ClientSocketPtr 큐에 쌓인 명령들 실행(pop)
	if (GameInstance && GameInstance->ClientSocketPtr)
	{
		if (GameInstance->ClientSocketPtr->Q_player.try_pop(recvPlayerData))
		{
			// 플레이어 큐가 너무 많이 쌓여서 전체적인 캐릭터 동기화에 딜레이가 생기는 걸 방지하기 위해
			int threshold = 2;	// 2개 이상 쌓이면 이전 데이터 try_pop해서 비우기 
			int loop = 0;	// 몇번 try_pop 할 횟수

			if (GameInstance->ClientSocketPtr->Q_player.unsafe_size() >= threshold) {
				loop = GameInstance->ClientSocketPtr->Q_player.unsafe_size();	// 쌓여있는거 다 비우기
			}
			else {
				loop = 1;	// threshold 이하면 아래 for문 한번만 실행되게(try_pop 한번만) 하려고
			}

			int pop_cnt = 0;

			for (pop_cnt = 0; pop_cnt < loop; pop_cnt++) {

				//UE_LOG(LogNet, Display, TEXT("Update Other Player: PlayerId=%d"), recvPlayerData.PlayerId);

				// 현재 GameMode 인스턴스를 얻기
				if (AOneGameModeBase* MyGameMode = Cast<AOneGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
				{
					// GameMode 내의 함수 호출하여 다른 플레이어의 위치 업데이트
					MyGameMode->UpdateOtherPlayer(recvPlayerData.PlayerId, recvPlayerData.Location, recvPlayerData.Rotation, recvPlayerData.charactertype,
						recvPlayerData.username, recvPlayerData.hp);

					// 현재 컨트롤러가 빙의한 Pawn 가져오기
					APawn* ControlledPawn = GetPawn();

					//UE_LOG(LogNet, Display, TEXT("Update Other Player12432543543535: PlayerId=%d"), recvPlayerData.PlayerId);
					//UE_LOG(LogNet, Display, TEXT("Update Other Player234234324324er: hp=%f"), recvPlayerData.hp);
					//UE_LOG(LogNet, Display, TEXT("Update Other Player        : hp=%f"), recvPlayerData.hp);

					// 빙의된 Pawn이 ACharacter라면 캐스팅
					if (ABaseCharacter* ControlledCharacter = Cast<ABaseCharacter>(ControlledPawn))
					{
						if (ControlledCharacter) {

							ControlledCharacter->UpdateOtherPlayerUI(recvPlayerData.PlayerId, recvPlayerData.hp, recvPlayerData.charactertype, recvPlayerData.username);
						}
					}
				}

				if (GameInstance->ClientSocketPtr->Q_player.try_pop(recvPlayerData) == false)
					break;
			}
			if (loop != 1) {
				//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Q_player cleared (try_pop count): %d"), pop_cnt));
				//UE_LOG(LogTemp, Log, TEXT("Q_player cleared (try_pop count): %d"), pop_cnt);
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
				//UE_LOG(LogNet, Display, TEXT("Update Other Player: PlayerId=%d"), recvEquipItem.PlayerId);
			}
		}
		// MyGameMode->UpdateUnEquipItem(recvUnEquipItem.PlayerId, recvUnEquipItem.Itemtype); Send부분 필요(아이템 장착해제시 동기화를 위해 필요)
		// recvUnEquipItem.PlayerId, recvUnEquipItem.ItemType 와 이걸 보내줄수있는 Send()부분 있으면 남은 부분 제작 예정

		if (GameInstance->ClientSocketPtr->Q_detachitem.try_pop(recvDetachItem))
		{
			if (AOneGameModeBase* MyGameMode = Cast<AOneGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				MyGameMode->UpdateUnEquipItem(recvDetachItem.PlayerId, recvDetachItem.Itemtype);
			}
		}

		if (GameInstance->ClientSocketPtr->Q_slicevector.try_pop(recvSliceVector))
		{
			if (AOneGameModeBase* MyGameMode = Cast<AOneGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				MyGameMode->UpdateCuttingZombie(recvSliceVector.zombieid, recvSliceVector.location, recvSliceVector.position, recvSliceVector.normal, recvSliceVector.impulse);
			}
		}

		if (GameInstance->ClientSocketPtr->Q_run.try_pop(recvRun))
		{
			// 러닝 큐가 너무 많이 쌓여서 걷기/뛰기 애니메이션 동기화에 딜레이가 생기는 걸 방지하기 위해
			int threshold = 2;	// 2개 이상 쌓이면 이전 데이터 try_pop해서 비우기 
			int loop = 0;	// 몇번 try_pop 할 횟수

			if (GameInstance->ClientSocketPtr->Q_run.unsafe_size() >= threshold) {
				loop = GameInstance->ClientSocketPtr->Q_run.unsafe_size();	// 쌓여있는거 다 비우기
			}
			else {
				loop = 1;	// threshold 이하면 아래 for문 한번만 실행되게(try_pop 한번만) 하려고
			}

			int pop_cnt = 0;

			for (pop_cnt = 0; pop_cnt < loop; pop_cnt++) {

				if (AOneGameModeBase* MyGameMode = Cast<AOneGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
				{
					if (recvRun.b_run == 1) {
						MyGameMode->UpdatePlayerRun(recvRun.PlayerId, false);
					}
					else if (recvRun.b_run == 2) {
						MyGameMode->UpdatePlayerRun(recvRun.PlayerId, true);
					}
					//UE_LOG(LogNet, Display, TEXT("Update Other Player: PlayerId=%d"), recvPlayerData.PlayerId);
				}

				if (GameInstance->ClientSocketPtr->Q_run.try_pop(recvRun) == false)
					break;
			}
			if (loop != 1) {
				//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Printf(TEXT("Q_run cleared (try_pop count): %d"), pop_cnt));
				//UE_LOG(LogTemp, Log, TEXT("Q_run cleared (try_pop count): %d"), pop_cnt);
			}
		}

		if (GameInstance->ClientSocketPtr->Q_jump.try_pop(recvJump)) {
			if (AOneGameModeBase* MyGameMode = Cast<AOneGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				MyGameMode->UpdatePlayerJump(recvJump.PlayerId);
			}
		}

		if (GameInstance->ClientSocketPtr->Q_setitem.try_pop(recvSetItem)) {
			if (AOneGameModeBase* MyGameMode = Cast<AOneGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				FName Fitemname = FName(recvSetItem.itemname.c_str());

				FString Ftexture_path = FString(recvSetItem.texture_path.c_str());

				// FString을 TCHAR*로 변환하여 LoadObject에 전달
				UTexture2D* LoadedTexture = LoadObject<UTexture2D>(NULL, *Ftexture_path, NULL, LOAD_None, NULL);

				MyGameMode->SpawnItemBoxes(recvSetItem.itemid, Fitemname, recvSetItem.itemclass, LoadedTexture, recvSetItem.count, recvSetItem.floor, recvSetItem.itempos);
			}
		}

		if (GameInstance->ClientSocketPtr->Q_dropitem.try_pop(recvDropItem)) {
			if (AOneGameModeBase* MyGameMode = Cast<AOneGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				FName Fitemname = FName(recvDropItem.itemname.c_str());

				FString Ftexture_path = FString(recvDropItem.texture_path.c_str());

				UTexture2D* LoadedTexture = LoadObject<UTexture2D>(NULL, *Ftexture_path, NULL, LOAD_None, NULL);

				UE_LOG(LogNet, Display, TEXT("queue try pop Q_dropitem"));

				MyGameMode->SpawnOtherCharGroundItemBoxes((recvDropItem.itemid - 1), Fitemname, recvDropItem.itemclass, LoadedTexture, recvDropItem.count, recvDropItem.itempos);
			}
		}

		if (GameInstance->ClientSocketPtr->Q_setcar.try_pop(recvSetCar)) {
			if (AOneGameModeBase* MyGameMode = Cast<AOneGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				FName Fcarname = FName(recvSetCar.carname.c_str());
				FName Fcarkeyname = FName(recvSetCar.carkeyname.c_str());

				MyGameMode->SpawnInterItem(recvSetCar.carid, Fcarname, recvSetCar.carpos, recvSetCar.carrotator, Fcarkeyname);
			}
		}

		if (GameInstance->ClientSocketPtr->Q_ditem.try_pop(recvDestroyItem)) {
			if (AOneGameModeBase* MyGameMode = Cast<AOneGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				UE_LOG(LogNet, Display, TEXT("queue try pop destroy item: itemid= %d"), recvDestroyItem.itemid);
				MyGameMode->DestroyItem(recvDestroyItem.itemid, recvDestroyItem.playerid);
			}
		}

		if (GameInstance->ClientSocketPtr->Q_getkey.try_pop(recvGetkey)) {
			if (AOneGameModeBase* MyGameMode = Cast<AOneGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				//처리 recvGetKey.itemid, recvGetKey.playerid

				UE_LOG(LogNet, Display, TEXT("queue try pop get_key: itemid= %d"), recvGetkey.itemid);
				APawn* ControlledPawn = GetPawn();
				if (ABaseCharacter* ControlledCharacter = Cast<ABaseCharacter>(ControlledPawn))
				{
					if (ControlledCharacter) {
						ControlledCharacter->UpdatePickUpKey(recvGetkey.itemid, recvGetkey.playerid);
					}
				}

			}
		}

		if (GameInstance->ClientSocketPtr->Q_escape.try_pop(recvEscapeRoot)) {
			if (AOneGameModeBase* MyGameMode = Cast<AOneGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				//처리 recvEscapeRoot.playerid, recvEscapeRoot.root
				APawn* ControlledPawn = GetPawn();
				if (ABaseCharacter* ControlledCharacter = Cast<ABaseCharacter>(ControlledPawn))
				{
					if (ControlledCharacter) {
						ControlledCharacter->UpdateOpenKey(recvEscapeRoot.root);
					}
				}

				MyGameMode->TurnOnCarHeadlights(recvEscapeRoot.carid);
				UE_LOG(LogNet, Display, TEXT("tur on headlight: carid = %d"), recvEscapeRoot.carid);
			}
		}

		if (GameInstance->ClientSocketPtr->Q_Gchat.try_pop(recvGameChat)) {
			APawn* ControlledPawn = GetPawn();
			if (ABaseCharacter* ControlledCharacter = Cast<ABaseCharacter>(ControlledPawn))
			{
				if (ControlledCharacter->GameChatUIWidget)
				{
					ControlledCharacter->GameChatUIWidget->AddChatMessage(recvGameChat.chat);
				}
			}
		}

		if (GameInstance->ClientSocketPtr->Q_gclear.try_pop(recvGameClear)) {
			if (AOneGameModeBase* MyGameMode = Cast<AOneGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				/*uint32 root;
				uint32 alive_players;
				uint32 dead_players;
				std::string open_player;
				uint32 my_kill_count;
				uint32 best_kill_count;
				std::string best_kill_player;*/


				UE_LOG(LogNet, Display, TEXT("recvGameClearrecvGameClearrecvGameClearrecvGameClearrecvGameClear"));

				//처리 recvEscapeRoot.playerid, recvEscapeRoot.root
				APawn* ControlledPawn = GetPawn();
				if (ABaseCharacter* ControlledCharacter = Cast<ABaseCharacter>(ControlledPawn))
				{
					if (ControlledCharacter) {
						std::string bestkillStr = recvGameClear.best_kill_player;
						FString bestkillplayerFStr = FString(bestkillStr.c_str());  // std::string을 FString으로 변환

						std::string OpenStr = recvGameClear.open_player;
						FString OpenPlayerFStr = FString(OpenStr.c_str());  // std::string을 FString으로 변환

						ControlledCharacter->ProGameClear(recvGameClear.root
							, recvGameClear.alive_players
							, recvGameClear.dead_players
							, OpenPlayerFStr
							, recvGameClear.my_kill_count
							, recvGameClear.best_kill_count
							, bestkillplayerFStr);
					}


				}
			}
		}

		//UE_LOG(LogNet, Display, TEXT("Update call Zombie: Playerid=%d"), GameInstance->ClientSocketPtr->MyPlayerId);
		if (GameInstance->ClientSocketPtr->Q_zombie.try_pop(recvZombieData))
		{
			//UE_LOG(LogNet, Display, TEXT("try_pop Zombie: ZombieId=%d"), recvZombieData.ZombieId);
			if (AOneGameModeBase* MyGameMode = Cast<AOneGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				MyGameMode->UpdateZombie(recvZombieData.ZombieId, recvZombieData.ZombieType, recvZombieData.Location, recvZombieData.Rotation);
				UE_LOG(LogNet, Display, TEXT("Update call Zombie: Playerid=%d"), GameInstance->ClientSocketPtr->MyPlayerId);	// 좀비가 움직이거나 스폰되면 보내는데 이제는 움직임은 따로 통신하지 않아서 스폰될때 불림
			}
		}

		if (GameInstance->ClientSocketPtr->Q_zattack.try_pop(recvZombieAttack))
		{
			//UE_LOG(LogNet, Display, TEXT("try_pop Zombie: ZombieId=%d"), recvZombieData.ZombieId);
			if (AOneGameModeBase* MyGameMode = Cast<AOneGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				MyGameMode->UpdateZombieAttack(recvZombieAttack.ZombieId, recvZombieAttack.PlayerId);
				//UE_LOG(LogNet, Display, TEXT("Update Zombie Attack: Zombieid=%d, Playerid=%d"), recvZombieAttack.ZombieId, recvZombieAttack.PlayerId);
			}
		}

		if (GameInstance->ClientSocketPtr->Q_zhp.try_pop(recvZombieHP)) {
			if (AOneGameModeBase* MyGameMode = Cast<AOneGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				MyGameMode->UpdateZombieHP(recvZombieHP.ZombieId, recvZombieHP.Damage);
			}
		}

		if (GameInstance->ClientSocketPtr->Q_shouting.try_pop(recvZombieShouting)) {
			if (AOneGameModeBase* MyGameMode = Cast<AOneGameModeBase>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				// zombieshouting 함수 호출 추가
				MyGameMode->UpdateShoutingZombie(recvZombieShouting.zombieid, recvZombieShouting.playerid);
			}
		}

		//UpdateCuttingZombie(uint32 ZombieId, FVector zombieLocation, FVector planePosition, FVector planeNormal, FVector impulseDirection)

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

		if (ZombieMap.IsEmpty() == true) {
			break;
		}

		ABaseZombie** zombie = ZombieMap.Find(tmp_path.ZombieId);

		if (zombie == nullptr || *zombie == nullptr) {
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("(Q_path) Could not find Zombie ID: %d"), tmp_path.ZombieId));
			UE_LOG(LogTemp, Warning, TEXT("(Q_path) Could not find Zombie ID: %d"), tmp_path.ZombieId);
			continue;  // 좀비를 찾을 수 없으면 다음으로 넘어감
		}

		switch (tmp_path.targetType) {
		case 1:
			(*zombie)->targetType = (*zombie)->TARGET::NULL_TARGET;
			break;
		case 2:

			//(*zombie)->targetType => prevTargetType
			if ((*zombie)->targetType != (*zombie)->TARGET::PLAYER) {	// 좀비 플레이어를 처음 발견 또는 놓쳤다가 다시 발견했을 때 호드 사운드 재생
				UE_LOG(LogTemp, Log, TEXT("zombieType: %s"), *(*zombie)->GetZombieName().ToString());

				if ((*zombie)->GetZombieName() == TEXT("ShoutingZombie") && (*zombie)->IsShouted() == true) {	// 샤우팅 좀비의 경우, 샤우팅이랑 소리 안 겹치게 ㅇㅇ
					UGameplayStatics::PlaySoundAtLocation(this, (*zombie)->GrowlSound, (*zombie)->GetActorLocation(), 0.7333f);
				}
				else if ((*zombie)->GetZombieName() == TEXT("NormalZombie") || (*zombie)->GetZombieName() == TEXT("RunningZombie")) {
					UGameplayStatics::PlaySoundAtLocation(this, (*zombie)->GrowlSound, (*zombie)->GetActorLocation(), 0.7333f);
				}
			}

			(*zombie)->targetType = (*zombie)->TARGET::PLAYER;
			break;
		case 3:
			(*zombie)->targetType = (*zombie)->TARGET::SHOUTING;
			break;
		case 4:
			(*zombie)->targetType = (*zombie)->TARGET::FOOTSOUND;
			break;
		case 5:
			(*zombie)->targetType = (*zombie)->TARGET::INVESTIGATED;
			break;
		case 6:
			(*zombie)->targetType = (*zombie)->TARGET::PATROL;
			break;
		case 7:
			(*zombie)->targetType = (*zombie)->TARGET::HORDESOUND;
			break;
		}

		bool isAnimPlaying_besideWalking = false;	// 걷기 애니메이션 말고 다른 애니메이션 (공격, 피격, 샤우팅) 재생 중인지

		if ((*zombie)->CachedAnimInstance->Montage_IsPlaying((*zombie)->CachedAnimInstance->AttackMontage) == true
			|| (*zombie)->CachedAnimInstance->Montage_IsPlaying((*zombie)->CachedAnimInstance->BeAttackedMontage) == true
			|| (*zombie)->CachedAnimInstance->Montage_IsPlaying((*zombie)->CachedAnimInstance->ShoutingMontage) == true) {

			isAnimPlaying_besideWalking = true;
		}

		if (isAnimPlaying_besideWalking == false) {	// 공격, 피격, 샤우팅 애니메이션이 재생 중이지 않을때만

			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, FString::Printf(TEXT("Update Zombie path: ZombieID=%d, PlayerID=%d"/*, NextPath=(%.2f , %.2f , %.2f)"*/), tmp_path.ZombieId, GameInstance->ClientSocketPtr->MyPlayerId));
			//UE_LOG(LogNet, Display, TEXT("Update Zombie path: ZombieID=%d, PlayerID = %d"/*, NextPath=(%.2f , %.2f , %.2f)"*/), tmp_path.ZombieId, GameInstance->ClientSocketPtr->MyPlayerId);

			// 좀비 위치 서버에서 받은 위치로 갱신
			if (tmp_path.Location.IsZero() == false)
				(*zombie)->SetActorLocation(tmp_path.Location);

			// 좀비 목적지 설정
			if (tmp_path.Path1.empty() == false) {

				// 서버에서 해당 좀비 애니메이션 재생중이면 path1 에 { 9999.f, 9999.f, 9999.f } 담아서 보냄
				if (*(tmp_path.Path1.begin()) == std::tuple{ 9999.f, 9999.f, 9999.f }) {
					//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, FString::Printf(TEXT("Update Zombie path: ZombieID=%d, PlayerID=%d (HaveToWait)"/*, NextPath=(%.2f , %.2f , %.2f)"*/), tmp_path.ZombieId, GameInstance->ClientSocketPtr->MyPlayerId));

					// + MontageEnded 보다 먼저 실행되서 ZombieMoveTo 실행됨 => ZombieMoveTo 내에 path1 { 9999.f, 9999.f, 9999.f } 면 return하는 예외처리!
					(*zombie)->NextPath[0] = *(tmp_path.Path1.begin());
				}
				else {
					//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Update Zombie path: ZombieID=%d, PlayerID=%d, Path=(%.2f , %.2f , %.2f)"), 
					//	tmp_path.ZombieId, GameInstance->ClientSocketPtr->MyPlayerId, get<0>(*(tmp_path.Path1.begin())), get<1>(*(tmp_path.Path1.begin())), get<2>(*(tmp_path.Path1.begin()))));

					(*zombie)->NextPath[0] = *(tmp_path.Path1.begin());

					(*zombie)->afterAnim_idleDuration = 0.f;	// 서버로부터 새로운 ZombiePath를 받으면 실행 => ZombieMoveTo idle 상태 초기화 => 다시 움직이게 함!
				}

			}
			else
				(*zombie)->NextPath[0] = std::tuple<float, float, float>();

			if (tmp_path.Path2.empty() == false)
				(*zombie)->NextPath[1] = *(tmp_path.Path2.begin());
			else
				(*zombie)->NextPath[1] = std::tuple<float, float, float>();
		}
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
	if (PreviousLocation != CurrentLocation || PreviousRotation != CurrentRotation || b_GetItem || PreviouHP != hp || sendRun || sendjump) {
		uint32 MyPlayerId = GameInstance->ClientSocketPtr->GetMyPlayerId();
		uint32 MyRoomId = GameInstance->ClientSocketPtr->MyRoomId;
		MyCharacterNumber = GameInstance->GetChoicedCharacterNumber();
		std::string MyPlayerName = GameInstance->ClientSocketPtr->MyUserName;

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
		packet.set_b_run(IsRlyRun + 1);
		packet.set_username(MyPlayerName);
		packet.set_roomnum(MyRoomId);
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
		sendRun = false;
	}
}

// 플레이어가 공격 서버로 전송-> 다른 클라도 해당 플레이어 공격 애니메이션 재생되게
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

		//UE_LOG(LogNet, Display, TEXT("Send Attack: PlayerId=%d"), GameInstance->ClientSocketPtr->GetMyPlayerId());
	}
}

void APlayerCharacterController::Send_Equipment()
{
	if (e_NWeapon || e_TWeapon || e_BHItem || e_HItem || e_KeyItem)
	{
		APawn* MyPawn = GetPawn();
		ABaseCharacter* MyBaseCharacter = Cast<ABaseCharacter>(MyPawn);
		uint32 MyPlayerId = GameInstance->ClientSocketPtr->GetMyPlayerId();
		uint32 ItemType; // 1 = BHItem, 2 = HItem, 3 = TWeapon, 4 = KeyItem, 5 = NWeapon

		if (e_BHItem)
		{
			ItemName = TCHAR_TO_UTF8(*(MyBaseCharacter->QuickSlot[0].Name.ToString()));
			ItemType = 1;
		}
		if (e_HItem)
		{
			ItemName = TCHAR_TO_UTF8(*(MyBaseCharacter->QuickSlot[1].Name.ToString()));
			ItemType = 2;
		}
		if (e_TWeapon)
		{
			ItemName = TCHAR_TO_UTF8(*(MyBaseCharacter->QuickSlot[2].Name.ToString()));
			ItemType = 3;
		}
		if (e_KeyItem)
		{
			ItemName = TCHAR_TO_UTF8(*(MyBaseCharacter->QuickSlot[3].Name.ToString()));
			ItemType = 4;
		}
		if (e_NWeapon)
		{
			ItemName = TCHAR_TO_UTF8(*(MyBaseCharacter->QuickSlot[4].Name.ToString()));
			ItemType = 5;
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
		//UE_LOG(LogNet, Display, TEXT("Send Equip: PlayerId=%d"), GameInstance->ClientSocketPtr->GetMyPlayerId());
		e_NWeapon = false;
		e_TWeapon = false;
		e_BHItem = false; 
		e_HItem = false;
		e_KeyItem = false;
	}
}

void APlayerCharacterController::Check_run() 
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());

	if (b_run) {	// 뛰기 모드 ON
		basecharacter->RunWalkIcon(b_run);
		if (b_move_forward || b_move_left) {	// 플레이어 실제 이동 중

			if (basecharacter->m_bZeroStamina) {	
				// 캐릭터는 스태미너가 다 닳은 상태라 Run 상태 OFF 지만 IsRlyRun 은 아직 true 라면	-> 해당 작업을 해줘야 스태미너가 다 닳았을 때 false값 여러번 보내지 않게 막을 수 있음
				if (IsRlyRun) {
					IsRlyRun = basecharacter->IsRun();	// IsRlyRun = false;
					sendRun = true;						// 서버로 Run 상태 OFF 전송
				}
			}

			else if (basecharacter->IsRun() == false) {	// 아직 캐릭터 Run 상태 OFF 라면
				basecharacter->Run();				// 캐릭터 Run 상태 ON
				IsRlyRun = basecharacter->IsRun();	// IsRlyRun = true;
				sendRun = true;						// 서버로 Run 상태 ON 전송
			}

		}
		else {									// 플레이어 정지

			if (basecharacter->IsRun() == true) {	// 아직 캐릭터 Run 상태 ON 라면
				basecharacter->Run();				// 캐릭터 Run 상태 OFF
				IsRlyRun = basecharacter->IsRun();	// IsRlyRun = false;
				sendRun = true;						// 서버로 Run 상태 OFF 전송
			}

		}
	}
	else {			// 뛰기 모드 OFF
		basecharacter->RunWalkIcon(b_run);
		if (basecharacter->IsRun() == true) {	// 아직 캐릭터 Run 상태 ON 라면
			basecharacter->Run();				// 캐릭터 Run 상태 OFF
			IsRlyRun = basecharacter->IsRun();	// IsRlyRun = false;
			sendRun = true;						// 서버로 Run 상태 OFF 전송
		}

	}
}

//void APlayerCharacterController::Send_run() {
//	if (sendRun) {
//		uint32 MyPlayerId = GameInstance->ClientSocketPtr->GetMyPlayerId();
//
//		Protocol::run packet;
//		packet.set_playerid(MyPlayerId);
//		packet.set_b_run(IsRlyRun);
//		packet.set_packet_type(6);
//
//		// 직렬화
//		std::string serializedData;
//		packet.SerializeToString(&serializedData);
//		bool bIsSent = GameInstance->ClientSocketPtr->Send(serializedData.size(), (void*)serializedData.data());
//
//		sendRun = false;
//	}
//}

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
				PEI->BindAction(InputActions->InputMoveForward, ETriggerEvent::Completed, this, &APlayerCharacterController::MoveForwardStop);
				PEI->BindAction(InputActions->InputMoveLeft, ETriggerEvent::Triggered, this, &APlayerCharacterController::MoveLeft);
				PEI->BindAction(InputActions->InputMoveLeft, ETriggerEvent::Completed, this, &APlayerCharacterController::MoveLeftStop);
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
				PEI->BindAction(InputActions->InputChatToggle, ETriggerEvent::Completed, this, &APlayerCharacterController::ChatToggle);

				PEI->BindAction(InputActions->InputUpSTR, ETriggerEvent::Completed, this, &APlayerCharacterController::UpSTR);
				PEI->BindAction(InputActions->InputDownSTR, ETriggerEvent::Completed, this, &APlayerCharacterController::DownSTR);
				PEI->BindAction(InputActions->InputUpSpeed, ETriggerEvent::Completed, this, &APlayerCharacterController::UpSpeed);
				PEI->BindAction(InputActions->InputDownSpeed, ETriggerEvent::Completed, this, &APlayerCharacterController::DownSpeed);
				 
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

	// 키를 여는 (애니메이션) 도중이면 플레이어의 해당 입력 막음
	if (basecharacter->IsUsingKey() == true) {
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Using Key - Allow no Inputs(Move_WS)")));
		return;
	}

	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// find out which way is forward
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// add movement 
	basecharacter->MoveForward(ForwardDirection, MovementVector.Y);

	b_move_forward = true;
}

void APlayerCharacterController::MoveForwardStop()
{
	b_move_forward = false;
}

void APlayerCharacterController::MoveLeft(const FInputActionValue& Value)
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());

	// 키를 여는 (애니메이션) 도중이면 플레이어의 해당 입력 막음
	if (basecharacter->IsUsingKey() == true) {
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Using Key - Allow no Inputs(Move_AD)")));
		return;
	}

	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// find out which way is forward
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// add movement 
	basecharacter->MoveLeft(RightDirection, MovementVector.Y);

	b_move_left = true;
}

void APlayerCharacterController::MoveLeftStop()
{
	b_move_left = false;
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
	if (!b_run)
		b_run = true;
	else
		b_run = false;
}

void APlayerCharacterController::Jump(const FInputActionValue& Value)
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());

	// 키를 여는 (애니메이션) 도중이면 플레이어의 해당 입력 막음
	if (basecharacter->IsUsingKey() == true) {
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Using Key - Allow no Inputs(Jump)")));
		return;
	}

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

	// 지혈 아이템/회복 아이템 사용 애니메이션 플레이 도중에는 아이템 먹기 입력 막기 -> 아이템 먹기로 해당 애니메이션 캔슬이 가능해서 (이렇게되면 아이템 효과가 바로 시전됨)
	if (basecharacter->m_bIsBleedHealing == true || basecharacter->m_bIsHealing == true) {
		return;
	}

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
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("PlayerController Attack")));
			Attack();
		}
		else if (basecharacter->IsBHHandIn() && !(basecharacter->IsBHealing())) {
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("PlayerController BH")));
			BleedHealing();

		}
		else if (basecharacter->IsHealHandIn() && !(basecharacter->IsHealing())) {
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("PlayerController HHHHHHHHH")));

			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("PlayerController HHHHHHHHH22222")));
			Healing();

		}
		else if (basecharacter->IsKeyHandIn()) {
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("PlayerController Key")));
			PlayKey();

		}
		else if (basecharacter->IsThrowWHandIn()) {
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("PlayerController TH")));
			Throw();
		}

	}

}

void APlayerCharacterController::Attack()
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());
	basecharacter->SetAttack(true);
	b_attack = true;
	UE_LOG(LogTemp, Log, TEXT("AttackStart: %d"), GameInstance->ClientSocketPtr->GetMyPlayerId());
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

	UE_LOG(LogTemp, Log, TEXT("QuickNWeapon"));

	basecharacter->QuickNWeapon();

	e_NWeapon = true;
}

void APlayerCharacterController::QuickTWeapon(const FInputActionValue& Value)
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());

	UE_LOG(LogTemp, Log, TEXT("QuickTWeapon"));

	basecharacter->QuickTWeapon();

	e_TWeapon = true;
}

void APlayerCharacterController::QuickBHItem(const FInputActionValue& Value)
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());

	UE_LOG(LogTemp, Log, TEXT("QuickBHItem"));

	basecharacter->QuickBHItem();

	e_BHItem = true;

}

void APlayerCharacterController::QuickHItem(const FInputActionValue& Value)
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());

	UE_LOG(LogTemp, Log, TEXT("QuickHItem"));

	basecharacter->QuickHItem();

	e_HItem = true;
}

void APlayerCharacterController::QuickKeyItem(const FInputActionValue& Value)
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());

	UE_LOG(LogTemp, Log, TEXT("QuickKeyItem"));

	basecharacter->QuickKeyItem();

	e_KeyItem = true;
}

void APlayerCharacterController::ChatToggle(const FInputActionValue& Value)
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());

	UE_LOG(LogTemp, Log, TEXT("ChatToggle"));

	bool bIsChatVisible = basecharacter->GameChatUIWidget->IsChatVisible();

	if (bIsChatVisible)
	{
		// 채팅창이 열려 있으면 닫기
		basecharacter->GameChatUIWidget->ToggleChatVisibility(false);
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
	else
	{
		// 채팅창이 닫혀 있으면 열기
		basecharacter->GameChatUIWidget->ToggleChatVisibility(true);
		basecharacter->GameChatUIWidget->SetKeyboardFocus();
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
}

void APlayerCharacterController::UpSTR(const FInputActionValue& Value)
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());

	UE_LOG(LogTemp, Log, TEXT("UpSTR"));

	basecharacter->UpSTR();
}

void APlayerCharacterController::DownSTR(const FInputActionValue& Value)
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());

	UE_LOG(LogTemp, Log, TEXT("DownSTR"));

	basecharacter->DownSTR();
}

void APlayerCharacterController::UpSpeed(const FInputActionValue& Value)
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());

	UE_LOG(LogTemp, Log, TEXT("UpSpeed"));

	basecharacter->UpSpeed();
}

void APlayerCharacterController::DownSpeed(const FInputActionValue& Value)
{
	ABaseCharacter* basecharacter = Cast<ABaseCharacter>(GetCharacter());

	UE_LOG(LogTemp, Log, TEXT("DownSpeed"));

	basecharacter->DownSpeed();
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
