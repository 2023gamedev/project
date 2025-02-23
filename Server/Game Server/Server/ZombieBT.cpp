#include "ZombieBT.h"

// BT
#include "Task.h"
#include "Selector.h"
#include "Sequence.h"

#include "Detect.h"
#include "CanSeePlayer.h"
#include "HasShouting.h"
#include "HasFootSound.h"
#include "HordeAction.h"
#include "HasInvestigated.h"
#include "NotHasLastKnownPlayerLocation.h"
#include "CanAttack.h"
#include "CanNotAttack.h"
#include "Attack.h"
#include "MoveTo.h"


//======[[���� BT ����]]======//
ZombieBT::ZombieBT()
{
	//======[Task] �޸� �Ҵ�======//

	//<Detect-Selector>
	sel_detect = new Sel_Detect;
	//<CanSeePlayer-Selector>
	sel_canseeplayer = new Sel_CanSeePlayer;

	//{HasShouting-Sequence}
	seq_hasshouting = new Seq_HasShouting;
	//{HasFootSound-Sequence}
	seq_hasfootsound = new Seq_HasFootSound;
	//{HordeAction-Sequence}
	seq_hordeaction = new Seq_HordeAction;
	//{HasInvestigated-Sequence}
	seq_hasinvestigated = new Seq_HasInvestigated;
	//{NotHasLastKnownPlayerLocation-Sequence}
	seq_nothaslastknownplayerlocation = new Seq_NotHasLastKnownPlayerLocation;

	//{CanNotAttack-Sequence}
	seq_cannotattack = new Seq_CanNotAttack;
	//{CanAttack-Sequence}
	seq_canattack = new Seq_CanAttack;

	//[MoveTo-Task]
	t_moveto = new T_MoveTo;
	//[Attack-Task]
	t_attack = new T_Attack;


	MakeZombieBT();

	//==========================//
}

//======== Ʈ�� �ۼ� (�۾� �Ҵ�) ========//
void ZombieBT::MakeZombieBT()
{
	//<Selector-Detect> �Ҵ� -> �ʿ� �ڽĳ��� '�������' ����
	sel_detect->AddChild(sel_canseeplayer);
	sel_detect->AddChild(seq_hasshouting);
	sel_detect->AddChild(seq_hasfootsound);
	sel_detect->AddChild(seq_hordeaction);
	sel_detect->AddChild(seq_hasinvestigated);
	sel_detect->AddChild(seq_nothaslastknownplayerlocation);

	//<Selector-CanSeePlayer> �Ҵ� -> �ʿ� �ڽĳ��� '�������' ����
	sel_canseeplayer->AddChild(seq_canattack);
	sel_canseeplayer->AddChild(seq_cannotattack);

	//{Sequence-CanAttack} �Ҵ� -> �ʿ� �ڽĳ��� '�������' ����
	seq_canattack->AddChild(t_attack);

	//{Sequence-CanNotAttack} �Ҵ� -> �ʿ� �ڽĳ��� '�������' ����
	seq_cannotattack->AddChild(t_moveto);

	//{Sequence-HasShouting} �Ҵ� -> �ʿ� �ڽĳ��� '�������' ����
	seq_hasshouting->AddChild(t_moveto);

	//{Sequence-HasFootSound} �Ҵ� -> �ʿ� �ڽĳ��� '�������' ����
	seq_hasfootsound->AddChild(t_moveto);

	//{Sequence-HordeAction} �Ҵ� -> �ʿ� �ڽĳ��� '�������' ����
	seq_hordeaction->AddChild(t_moveto);

	//{Sequence-HasInvestigated} �Ҵ� -> �ʿ� �ڽĳ��� '�������' ����
	seq_hasinvestigated->AddChild(t_moveto);

	//{Sequence-NotHasLastKnownPlayerLocation} �Ҵ� -> �ʿ� �ڽĳ��� '�������' ����
	seq_nothaslastknownplayerlocation->AddChild(t_moveto);
}

ZombieBT::~ZombieBT()
{
	delete(sel_detect);
	delete(sel_canseeplayer);

	delete(seq_hasshouting);
	delete(seq_hasfootsound);
	delete(seq_hordeaction);
	delete(seq_hasinvestigated);
	delete(seq_nothaslastknownplayerlocation);

	delete(seq_cannotattack);
	delete(seq_canattack);

	delete(t_attack);
	delete(t_moveto);
}