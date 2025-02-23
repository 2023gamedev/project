#pragma once

// 전방선언 - 순환 참조 발생해서
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


	Selector* sel_detect;
	Selector* sel_canseeplayer;

	Sequence* seq_cannotattack;
	Sequence* seq_canattack;
	Sequence* seq_hasshouting;
	Sequence* seq_hasfootsound;
	Sequence* seq_hordeaction;
	Sequence* seq_hasinvestigated;
	Sequence* seq_nothaslastknownplayerlocation;

	Task* t_moveto;
	Task* t_attack;
};