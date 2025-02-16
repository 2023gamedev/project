#include "ZombieBT.h"

void ZombieBT::Zombie_BT_Initialize(int roomid)
{
	/*//======[좀비 BT 생성]======
	//======메모리 할당 -> 작업 할당======

	//======[Task] 메모리 할당======

	//<Selector>들
	zombie_bt_map[roomid].sel_detect = new Selector;
	zombie_bt_map[roomid].sel_canseeplayer = new Selector;

	//<Selector Detact> 가 가지는 Task들

	//[CanSeePlayer-Task]
	zombie_bt_map[roomid].t_canseeplayer = new TCanSeePlayer;
	//[HasShouting-Task]
	zombie_bt_map[roomid].t_hasshouting = new THasShouting;
	//[HasFootSound-Task]
	zombie_bt_map[roomid].t_hasfootsound = new THasFootSound;
	//[HasInvestigated-Task]
	zombie_bt_map[roomid].t_hasinvestigated = new THasInvestigated;
	//[NotHasLastKnownPlayerLocation-Task]
	zombie_bt_map[roomid].t_nothaslastknownplayerlocation = new TNotHasLastKnownPlayerLocation;

	//<Selector CanSeePlayer> 가 가지는 Task들

	//[CanNotAttack-Task]
	zombie_bt_map[roomid].t_cannotattack = new TCanNotAttack;
	//[CanAttack-Task]
	zombie_bt_map[roomid].t_canattack = new TCanAttack;

	//{Sequence} 가 가지는 Task들

	//[MoveTo-Task]
	zombie_bt_map[roomid].t_moveto = new TMoveTo;
	//[Attack-Task]
	zombie_bt_map[roomid].t_attack = new TAttack;

	//======== 트리 작성 (작업 할당) ========

	//<Selector-Detect> 할당
	//<Selector-Detect>에 해당 Task들 '순서대로' 삽입
	zombie_bt_map[roomid].sel_detect->AddChild(zombie_bt_map[roomid].t_canseeplayer);
	zombie_bt_map[roomid].sel_detect->AddChild(zombie_bt_map[roomid].t_hasshouting);
	zombie_bt_map[roomid].sel_detect->AddChild(zombie_bt_map[roomid].t_hasfootsound);
	zombie_bt_map[roomid].sel_detect->AddChild(zombie_bt_map[roomid].t_hasinvestigated);
	zombie_bt_map[roomid].sel_detect->AddChild(zombie_bt_map[roomid].t_nothaslastknownplayerlocation);

	//<Selector-CanSeePlayer> 할당
	//<Selector-CanSeePlayer>에 해당 Task들 '순서대로' 삽입
	zombie_bt_map[roomid].sel_canseeplayer->AddChild(zombie_bt_map[roomid].t_canattack);
	zombie_bt_map[roomid].sel_canseeplayer->AddChild(zombie_bt_map[roomid].t_cannotattack);

	//{Sequence-CanAttack} 할당
	//{Sequence-CanAttack}에 해당 Task들 '순서대로' 삽입
	zombie_bt_map[roomid].seq_canattack.AddChild(zombie_bt_map[roomid].t_attack);

	//{Sequence-CanNotAttack} 할당
	//{Sequence-CanNotAttack}에 해당 Task들 '순서대로' 삽입
	zombie_bt_map[roomid].seq_cannotattack.AddChild(zombie_bt_map[roomid].t_moveto);

	//{Sequence-HasShouting} 할당
	//{Sequence-HasShouting}에 해당 Task들 '순서대로' 삽입
	zombie_bt_map[roomid].seq_hasshouting.AddChild(zombie_bt_map[roomid].t_moveto);

	//{Sequence-HasFootSound} 할당
	//{Sequence-HasFootSound}에 해당 Task들 '순서대로' 삽입
	zombie_bt_map[roomid].seq_hasfootsound.AddChild(zombie_bt_map[roomid].t_moveto);

	//{Sequence-HasInvestigated} 할당
	//{Sequence-HasInvestigated}에 해당 Task들 '순서대로' 삽입
	zombie_bt_map[roomid].seq_hasinvestigated.AddChild(zombie_bt_map[roomid].t_moveto);

	//{Sequence-NotHasLastKnownPlayerLocation} 할당
	//{Sequence-NotHasLastKnownPlayerLocation}에 해당 Task들 '순서대로' 삽입
	zombie_bt_map[roomid].seq_nothaslastknownplayerlocation.AddChild(zombie_bt_map[roomid].t_moveto);

	//==========================*/
}