#pragma once
#include "Common.h"
#include "iocpServerClass.h"
#include "ZombiePathfinder.h"

class IOCP_CORE;


class ItemController
{
	ItemController(IOCP_CORE& mainServer);
	~ItemController();

    void SpawnRandomItem(FLOOR itemfloor);

public:
	IOCP_CORE* iocpServer;

    FLOOR floor;
};

