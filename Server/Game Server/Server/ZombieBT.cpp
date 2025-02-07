#include "ZombieBT.h"

void ZombieBT::Zombie_BT_Initialize(int roomid)
{
	/*//======[���� BT ����]======
	//======�޸� �Ҵ� -> �۾� �Ҵ�======

	//======[Task] �޸� �Ҵ�======

	//<Selector>��
	zombie_bt_map[roomid].sel_detect = new Selector;
	zombie_bt_map[roomid].sel_canseeplayer = new Selector;

	//<Selector Detact> �� ������ Task��

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

	//<Selector CanSeePlayer> �� ������ Task��

	//[CanNotAttack-Task]
	zombie_bt_map[roomid].t_cannotattack = new TCanNotAttack;
	//[CanAttack-Task]
	zombie_bt_map[roomid].t_canattack = new TCanAttack;

	//{Sequence} �� ������ Task��

	//[MoveTo-Task]
	zombie_bt_map[roomid].t_moveto = new TMoveTo;
	//[Attack-Task]
	zombie_bt_map[roomid].t_attack = new TAttack;

	//======== Ʈ�� �ۼ� (�۾� �Ҵ�) ========

	//<Selector-Detect> �Ҵ�
	//<Selector-Detect>�� �ش� Task�� '�������' ����
	zombie_bt_map[roomid].sel_detect->AddChild(zombie_bt_map[roomid].t_canseeplayer);
	zombie_bt_map[roomid].sel_detect->AddChild(zombie_bt_map[roomid].t_hasshouting);
	zombie_bt_map[roomid].sel_detect->AddChild(zombie_bt_map[roomid].t_hasfootsound);
	zombie_bt_map[roomid].sel_detect->AddChild(zombie_bt_map[roomid].t_hasinvestigated);
	zombie_bt_map[roomid].sel_detect->AddChild(zombie_bt_map[roomid].t_nothaslastknownplayerlocation);

	//<Selector-CanSeePlayer> �Ҵ�
	//<Selector-CanSeePlayer>�� �ش� Task�� '�������' ����
	zombie_bt_map[roomid].sel_canseeplayer->AddChild(zombie_bt_map[roomid].t_canattack);
	zombie_bt_map[roomid].sel_canseeplayer->AddChild(zombie_bt_map[roomid].t_cannotattack);

	//{Sequence-CanAttack} �Ҵ�
	//{Sequence-CanAttack}�� �ش� Task�� '�������' ����
	zombie_bt_map[roomid].seq_canattack.AddChild(zombie_bt_map[roomid].t_attack);

	//{Sequence-CanNotAttack} �Ҵ�
	//{Sequence-CanNotAttack}�� �ش� Task�� '�������' ����
	zombie_bt_map[roomid].seq_cannotattack.AddChild(zombie_bt_map[roomid].t_moveto);

	//{Sequence-HasShouting} �Ҵ�
	//{Sequence-HasShouting}�� �ش� Task�� '�������' ����
	zombie_bt_map[roomid].seq_hasshouting.AddChild(zombie_bt_map[roomid].t_moveto);

	//{Sequence-HasFootSound} �Ҵ�
	//{Sequence-HasFootSound}�� �ش� Task�� '�������' ����
	zombie_bt_map[roomid].seq_hasfootsound.AddChild(zombie_bt_map[roomid].t_moveto);

	//{Sequence-HasInvestigated} �Ҵ�
	//{Sequence-HasInvestigated}�� �ش� Task�� '�������' ����
	zombie_bt_map[roomid].seq_hasinvestigated.AddChild(zombie_bt_map[roomid].t_moveto);

	//{Sequence-NotHasLastKnownPlayerLocation} �Ҵ�
	//{Sequence-NotHasLastKnownPlayerLocation}�� �ش� Task�� '�������' ����
	zombie_bt_map[roomid].seq_nothaslastknownplayerlocation.AddChild(zombie_bt_map[roomid].t_moveto);

	//==========================*/
}