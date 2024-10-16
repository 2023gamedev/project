#pragma once
#include "Common.h"
#include "iocpServerClass.h"
#include "Zombie.h"

class IOCP_CORE;

enum class FLOOR {
    FLOOR_B2,
    FLOOR_B1,
    FLOOR_F1,
    FLOOR_F2,
    FLOOR_F3
};


class ItemController
{
	ItemController(IOCP_CORE& mainServer);
	~ItemController();

    void SpawnRandomItem(FLOOR itemfloor);

public:
	IOCP_CORE* iocpServer;
};

