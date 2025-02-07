#pragma once

#include "Selector.h"
#include "Sequence.h"

class ZombieBT
{
public:
	void Zombie_BT_Initialize(int roomid);


	Selector* sel_detect;
	Selector* sel_canseeplayer;

	Sequence* seq_cannotattack;
	Sequence* seq_canattack;
	Sequence* seq_hasshouting;
	Sequence* seq_hasfootsound;
	Sequence* seq_hasinvestigated;
	Sequence* seq_nothaslastknownplayerlocation;

	Task* t_moveto;
	Task* t_attack;
};