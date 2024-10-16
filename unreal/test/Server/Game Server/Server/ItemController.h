#pragma once
#include "Common.h"
#include "iocpServerClass.h"
#include "ZombiePathfinder.h"

class IOCP_CORE;

enum class EItemClass
{
	NORMALWEAPON,
	THROWINGWEAPON,
	HEALINGITEM,
	BLEEDINGHEALINGITEM,
	KEYITEM,
	BAGITEM,
	NONE
};

struct Item_Data {
	int itemID;
	std::string itemName;
	int itemClass;
	std::string texturePath;
	int count;
	int itemFloor;
	float x, y, z;

	Item_Data()
		: itemID(0), itemName(""), itemClass(0), texturePath(""),
		count(0), itemFloor(0), x(0.f), y(0.f), z(0.f) {}

	Item_Data(int id, const std::string& name, int cls, const std::string& texture, int cnt, int floor, float posX, float posY, float posZ)
		: itemID(id), itemName(name), itemClass(cls), texturePath(texture),
		count(cnt), itemFloor(floor), x(posX), y(posY), z(posZ) {}
};


struct FVector {
	float x, y, z;
};

struct ItemRandomLocation
{
	FVector sLocation;                  // 위치가 어딘가
	bool    bIsSeatLocation = false;    // 위치에 누가 이미 차지하고 있는가 true 
};


class ItemController
{
public:
	ItemController(IOCP_CORE& mainServer);
	~ItemController();

	void ItemRandomLocationSetting();

    FVector RandomItemLocation(FLOOR itemfloor);

	void SpawnItemBoxes(int itemboxindex, std::string itemname, EItemClass itemclass, string texture, int count, FLOOR itemfloor);

	void SendItemData(int id);

public:
	IOCP_CORE* iocpServer;

	ItemRandomLocation  ItemRandomLocationStruct[60];

	std::vector<Item_Data> items;
};

