#pragma once

// ���漱�� - ��ȯ ���� �߻��ؼ�
// BT
class Task;
class Selector;
class Sequence;



class ZombieBT
{
public:
	ZombieBT();
	~ZombieBT();

	void MakeZombieBT();


	Selector* sel_detect = nullptr;
	Selector* sel_canseeplayer = nullptr;

	Sequence* seq_cannotattack = nullptr;
	Sequence* seq_canattack = nullptr;
	Sequence* seq_hasshouting = nullptr;
	Sequence* seq_hasfootsound = nullptr;
	Sequence* seq_hordeaction = nullptr;
	Sequence* seq_hasinvestigated = nullptr;
	Sequence* seq_nothaslastknownplayerlocation = nullptr;

	Task* t_moveto = nullptr;
	Task* t_attack = nullptr;

};