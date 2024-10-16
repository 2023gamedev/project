#include "ItemController.h"

uniform_int_distribution uid;

ItemController::ItemController(IOCP_CORE& mainServer)
{
	iocpServer = &mainServer;

    SpawnItemBoxes(0, "SquareWood", EItemClass::NORMALWEAPON, ("/Game/InvenPng/InvenSquareWood.InvenSquareWood"), 1, FLOOR::FLOOR_B1);
    SpawnItemBoxes(1, "FireExtinguisher", EItemClass::NORMALWEAPON, ("/Game/InvenPng/InvenFireEx.InvenFireEx"), 1, FLOOR::FLOOR_B1);
    SpawnItemBoxes(2, "Pipe", EItemClass::NORMALWEAPON, ("/Game/InvenPng/InvenPipe.InvenPipe"), 1, FLOOR::FLOOR_B1);
    SpawnItemBoxes(3, "FireAxe", EItemClass::NORMALWEAPON, ("/Game/InvenPng/InvenFireAxe.InvenFireAxe"), 1, FLOOR::FLOOR_B1);
    SpawnItemBoxes(4, "SashimiKnife", EItemClass::NORMALWEAPON, ("/Game/InvenPng/InvenSasimiKnife.InvenSasimiKnife"), 1, FLOOR::FLOOR_B1);
    SpawnItemBoxes(5, "ButchersKnife", EItemClass::NORMALWEAPON, ("/Game/InvenPng/InvenButherKnife.InvenButherKnife"), 1, FLOOR::FLOOR_B1);
    SpawnItemBoxes(6, "Bottle", EItemClass::NORMALWEAPON, ("/Game/InvenPng/InvenBottle.InvenBottle"), 1, FLOOR::FLOOR_B1);
    SpawnItemBoxes(7, "DirtyClothes", EItemClass::BLEEDINGHEALINGITEM, ("/Game/InvenPng/InvenDirtyCloth.InvenDirtyCloth"), 3, FLOOR::FLOOR_B1);
    SpawnItemBoxes(8, "Water", EItemClass::HEALINGITEM, ("/Game/InvenPng/InvenWater.InvenWater"), 2, FLOOR::FLOOR_B1);
    SpawnItemBoxes(9, "Smoke", EItemClass::HEALINGITEM, ("/Game/InvenPng/InvenSmoke.InvenSmoke"), 2, FLOOR::FLOOR_B1);

    SpawnItemBoxes(10, "Drink", EItemClass::HEALINGITEM, ("/Game/InvenPng/InvenDrink.InvenDrink"), 2, FLOOR::FLOOR_B1);
    SpawnItemBoxes(11, "Snack", EItemClass::HEALINGITEM, ("/Game/InvenPng/InvenSnack.InvenSnack"), 2, FLOOR::FLOOR_B1);
    SpawnItemBoxes(12, "CannedTuna", EItemClass::HEALINGITEM, ("/Game/InvenPng/InvenCannedTuna.InvenCannedTuna"), 2, FLOOR::FLOOR_B1);
    SpawnItemBoxes(13, "Water", EItemClass::HEALINGITEM, ("/Game/InvenPng/InvenWater.InvenWater"), 2, FLOOR::FLOOR_B1);
    SpawnItemBoxes(14, "Smoke", EItemClass::HEALINGITEM, ("/Game/InvenPng/InvenSmoke.InvenSmoke"), 2, FLOOR::FLOOR_B1);
    SpawnItemBoxes(15, "Drink", EItemClass::HEALINGITEM, ("/Game/InvenPng/InvenDrink.InvenDrink"), 2, FLOOR::FLOOR_B1);
    SpawnItemBoxes(16, "BagActor", EItemClass::BAGITEM, ("/Game/InvenPng/InvenBag.InvenBag"), 1, FLOOR::FLOOR_B1); // 수정 필요
    SpawnItemBoxes(17, "BigBagActor", EItemClass::BAGITEM, ("/Game/InvenPng/InvenBigBag.InvenBigBag"), 1, FLOOR::FLOOR_B1);

    SpawnItemBoxes(18, "CarKey1", EItemClass::KEYITEM, ("/Game/InvenPng/InvenCarKey1.InvenCarKey1"), 1, FLOOR::FLOOR_B1);
    SpawnItemBoxes(19, "RoofKey1", EItemClass::KEYITEM, ("/Game/InvenPng/InvenRoofKey1.InvenRoofKey1"), 1, FLOOR::FLOOR_B1);

    // F1
    SpawnItemBoxes(20, "SquareWood", EItemClass::NORMALWEAPON, ("/Game/InvenPng/InvenSquareWood.InvenSquareWood"), 1, FLOOR::FLOOR_F1);
    SpawnItemBoxes(21, "FireExtinguisher", EItemClass::NORMALWEAPON, ("/Game/InvenPng/InvenFireEx.InvenFireEx"), 1, FLOOR::FLOOR_F1);
    SpawnItemBoxes(22, "Pipe", EItemClass::NORMALWEAPON, ("/Game/InvenPng/InvenPipe.InvenPipe"), 1, FLOOR::FLOOR_F1);
    SpawnItemBoxes(23, "FireAxe", EItemClass::NORMALWEAPON, ("/Game/InvenPng/InvenFireAxe.InvenFireAxe"), 1, FLOOR::FLOOR_F1);
    SpawnItemBoxes(24, "MannequinArm", EItemClass::NORMALWEAPON, ("/Game/InvenPng/InvnenMannequinArm.InvnenMannequinArm"), 1, FLOOR::FLOOR_F1);
    SpawnItemBoxes(25, "MannequinLeg", EItemClass::NORMALWEAPON, ("/Game/InvenPng/InvenMannequinLeg.InvenMannequinLeg"), 1, FLOOR::FLOOR_F1);
    SpawnItemBoxes(26, "Book", EItemClass::NORMALWEAPON, ("/Game/InvenPng/Invenbook.Invenbook"), 1, FLOOR::FLOOR_F1);
    SpawnItemBoxes(27, "Scissors", EItemClass::NORMALWEAPON, ("/Game/InvenPng/InvenSissor.InvenSissor"), 1, FLOOR::FLOOR_F1);
    SpawnItemBoxes(28, "MagicStick", EItemClass::NORMALWEAPON, ("/Game/InvenPng/InvenMagicStick.InvenMagicStick"), 1, FLOOR::FLOOR_F1);
    SpawnItemBoxes(29, "DirtyClothes", EItemClass::BLEEDINGHEALINGITEM, ("/Game/InvenPng/InvenDirtyCloth.InvenDirtyCloth"), 3, FLOOR::FLOOR_F1);

    SpawnItemBoxes(30, "Bandage", EItemClass::BLEEDINGHEALINGITEM, ("/Game/InvenPng/InvenBandage.InvenBandage"), 3, FLOOR::FLOOR_F1);
    SpawnItemBoxes(31, "Clothes", EItemClass::BLEEDINGHEALINGITEM, ("/Game/InvenPng/InvenCloth.InvenCloth"), 3, FLOOR::FLOOR_F1);
    SpawnItemBoxes(32, "Gauze", EItemClass::BLEEDINGHEALINGITEM, ("/Game/InvenPng/InvenGauze.InvenGauze"), 3, FLOOR::FLOOR_F1);
    SpawnItemBoxes(33, "Water", EItemClass::HEALINGITEM, ("/Game/InvenPng/InvenWater.InvenWater"), 2, FLOOR::FLOOR_F1);
    SpawnItemBoxes(34, "Smoke", EItemClass::HEALINGITEM, ("/Game/InvenPng/InvenSmoke.InvenSmoke"), 2, FLOOR::FLOOR_F1);
    SpawnItemBoxes(35, "Disinfectant", EItemClass::HEALINGITEM, ("/Game/InvenPng/InvenDisinfectant.InvenDisinfectant"), 2, FLOOR::FLOOR_F1);
    SpawnItemBoxes(36, "Ointment", EItemClass::HEALINGITEM, ("/Game/InvenPng/InvenOintment.InvenOintment"), 2, FLOOR::FLOOR_F1);
    SpawnItemBoxes(37, "BigBagActor", EItemClass::BAGITEM, ("/Game/InvenPng/InvenBigBag.InvenBigBag"), 1, FLOOR::FLOOR_F1);

    SpawnItemBoxes(38, "CarKey2", EItemClass::KEYITEM, ("/Game/InvenPng/InvenCarKey1.InvenCarKey1"), 1, FLOOR::FLOOR_F1);
    SpawnItemBoxes(39, "RoofKey2", EItemClass::KEYITEM, ("/Game/InvenPng/InvenRoofKey2.InvenRoofKey2"), 1, FLOOR::FLOOR_F1);

    // F2
    SpawnItemBoxes(40, "SquareWood", EItemClass::NORMALWEAPON, ("/Game/InvenPng/InvenSquareWood.InvenSquareWood"), 1, FLOOR::FLOOR_F2);
    SpawnItemBoxes(41, "FireExtinguisher", EItemClass::NORMALWEAPON, ("/Game/InvenPng/InvenFireEx.InvenFireEx"), 1, FLOOR::FLOOR_F2);
    SpawnItemBoxes(42, "Pipe", EItemClass::NORMALWEAPON, ("/Game/InvenPng/InvenPipe.InvenPipe"), 2, FLOOR::FLOOR_F2);
    SpawnItemBoxes(43, "FireAxe", EItemClass::NORMALWEAPON, ("/Game/InvenPng/InvenFireAxe.InvenFireAxe"), 1, FLOOR::FLOOR_F2);
    SpawnItemBoxes(44, "Plunger", EItemClass::NORMALWEAPON, ("/Game/InvenPng/InvenPlunger.InvenPlunger"), 1, FLOOR::FLOOR_F2);
    SpawnItemBoxes(45, "FryingPan", EItemClass::NORMALWEAPON, ("/Game/InvenPng/InvenFryingPan.InvenFryingPan"), 1, FLOOR::FLOOR_F2);
    SpawnItemBoxes(46, "Iron", EItemClass::NORMALWEAPON, ("/Game/InvenPng/InvenIron.InvenIron"), 1, FLOOR::FLOOR_F2);
    SpawnItemBoxes(47, "Shovels", EItemClass::NORMALWEAPON, ("/Game/InvenPng/InvenShovel.InvenShovel"), 1, FLOOR::FLOOR_F2);
    SpawnItemBoxes(48, "GolfClub", EItemClass::NORMALWEAPON, ("/Game/InvenPng/InvenGolfClub.InvenGolfClub"), 1, FLOOR::FLOOR_F2);
    SpawnItemBoxes(49, "WoodenBat", EItemClass::NORMALWEAPON, ("/Game/InvenPng/InvenWoodenBat.InvenWoodenBat"), 1, FLOOR::FLOOR_F2);

    SpawnItemBoxes(50, "DirtyClothes", EItemClass::BLEEDINGHEALINGITEM, ("/Game/InvenPng/InvenDirtyCloth.InvenDirtyCloth"), 3, FLOOR::FLOOR_F2);
    SpawnItemBoxes(51, "Clothes", EItemClass::BLEEDINGHEALINGITEM, ("/Game/InvenPng/InvenCloth.InvenCloth"), 3, FLOOR::FLOOR_F2);
    SpawnItemBoxes(52, "Water", EItemClass::HEALINGITEM, ("/Game/InvenPng/InvenWater.InvenWater"), 2, FLOOR::FLOOR_F2);
    SpawnItemBoxes(53, "Smoke", EItemClass::HEALINGITEM, ("/Game/InvenPng/InvenSmoke.InvenSmoke"), 2, FLOOR::FLOOR_F2);
    SpawnItemBoxes(54, "Water", EItemClass::HEALINGITEM, ("/Game/InvenPng/InvenWater.InvenWater"), 2, FLOOR::FLOOR_F2);
    SpawnItemBoxes(55, "Smoke", EItemClass::HEALINGITEM, ("/Game/InvenPng/InvenSmoke.InvenSmoke"), 2, FLOOR::FLOOR_F2);
    SpawnItemBoxes(56, "BagActor", EItemClass::BAGITEM, ("/Game/InvenPng/InvenBag.InvenBag"), 1, FLOOR::FLOOR_F2);
    SpawnItemBoxes(57, "BigBagActor", EItemClass::BAGITEM, ("/Game/InvenPng/InvenBigBag.InvenBigBag"), 1, FLOOR::FLOOR_F2);

    SpawnItemBoxes(58, "CarKey3", EItemClass::NORMALWEAPON, ("/Game/InvenPng/InvenCarKey2.InvenCarKey2"), 10, FLOOR::FLOOR_F2);
    SpawnItemBoxes(59, "CarKey4", EItemClass::NORMALWEAPON, ("/Game/InvenPng/InvenCarKey2.InvenCarKey2"), 20, FLOOR::FLOOR_F2);

    //SpawnInterItem(0, "CarActor");
    //SpawnInterItem(1, "CarActor");
    //SpawnInterItem(2, "CarActor");
    //SpawnInterItem(3, "CarActor");
    //SpawnInterItem(4, "CarActor");
    //SpawnInterItem(5, "CarActor");
    //SpawnInterItem(6, "CarActor");
    //SpawnInterItem(7, "RoofTopDoorActor");

    
}

ItemController::~ItemController()
{
}

void ItemController::ItemRandomLocationSetting()
{
    // 0~19은 B1 20~39는 F1 40~59는 F2
// B1
    ItemRandomLocationStruct[0].sLocation = FVector(2330.f, 3200.f, 975.0626f);
    ItemRandomLocationStruct[1].sLocation = FVector(1880.f, 3870.f, 975.0626f);
    ItemRandomLocationStruct[2].sLocation = FVector(400.f, 3650.f, 975.0626f);

    ItemRandomLocationStruct[3].sLocation = FVector(1000.f, 2800.f, 975.0626f);
    ItemRandomLocationStruct[4].sLocation = FVector(2200.f, 3300.f, 975.0626f);
    ItemRandomLocationStruct[5].sLocation = FVector(1770.f, 2170.f, 975.0626f);

    ItemRandomLocationStruct[6].sLocation = FVector(440.f, 2310.f, 975.0626f);
    ItemRandomLocationStruct[7].sLocation = FVector(2300.f, 1800.f, 975.0626f);
    ItemRandomLocationStruct[8].sLocation = FVector(820.f, 1650.f, 975.0626f);

    ItemRandomLocationStruct[9].sLocation = FVector(120.f, 1200.f, 975.0626f);
    ItemRandomLocationStruct[10].sLocation = FVector(270.f, 2000.f, 975.0626f);
    ItemRandomLocationStruct[11].sLocation = FVector(1870.f, 800.f, 975.0626f);

    ItemRandomLocationStruct[12].sLocation = FVector(1200.f, 100.f, 975.0626f);
    ItemRandomLocationStruct[13].sLocation = FVector(30.f, 540.f, 975.0626f);
    ItemRandomLocationStruct[14].sLocation = FVector(500.f, -238.f, 975.0626f);

    ItemRandomLocationStruct[15].sLocation = FVector(1200.f, -976.f, 975.0626f);
    ItemRandomLocationStruct[16].sLocation = FVector(480.f, 800.f, 975.0626f);
    ItemRandomLocationStruct[17].sLocation = FVector(2330.f, 400.f, 975.0626f);

    ItemRandomLocationStruct[18].sLocation = FVector(730.f, 2130.f, 975.0626f);
    ItemRandomLocationStruct[19].sLocation = FVector(30.f, 3540.f, 975.0626f);


    // F1
    ItemRandomLocationStruct[20].sLocation = FVector(1200.f, 70.f, 1929.0626f);
    ItemRandomLocationStruct[21].sLocation = FVector(180.f, 520.f, 1929.0626f);
    ItemRandomLocationStruct[22].sLocation = FVector(1200.f, -817.f, 1929.0626f);

    ItemRandomLocationStruct[23].sLocation = FVector(800.f, -817.f, 1929.0626f);
    ItemRandomLocationStruct[24].sLocation = FVector(165.f, 960.f, 1929.0626f);
    ItemRandomLocationStruct[25].sLocation = FVector(1980.f, 1100.f, 1929.0626f);

    ItemRandomLocationStruct[26].sLocation = FVector(1390.f, 800.f, 1929.0626f);
    ItemRandomLocationStruct[27].sLocation = FVector(400.f, 1680.f, 1929.0626f);
    ItemRandomLocationStruct[28].sLocation = FVector(1100.f, 1260.f, 1929.0626f);

    ItemRandomLocationStruct[29].sLocation = FVector(70.f, 2390.f, 1929.0626f);
    ItemRandomLocationStruct[30].sLocation = FVector(550.f, 2520.f, 1929.0626f);
    ItemRandomLocationStruct[31].sLocation = FVector(200.f, 2800.f, 1929.0626f);

    ItemRandomLocationStruct[32].sLocation = FVector(1600.f, 3050.f, 1929.0626f);
    ItemRandomLocationStruct[33].sLocation = FVector(1530.f, 3900.f, 1929.0626f);
    ItemRandomLocationStruct[34].sLocation = FVector(2340.f, 3850.f, 1929.0626f);

    ItemRandomLocationStruct[35].sLocation = FVector(290.f, 3020.f, 1929.0626f);
    ItemRandomLocationStruct[36].sLocation = FVector(90.f, 3770.f, 1929.0626f);
    ItemRandomLocationStruct[37].sLocation = FVector(1050.f, 3350.f, 1929.0626f);

    ItemRandomLocationStruct[38].sLocation = FVector(50.f, 1740.f, 1929.0626f);
    ItemRandomLocationStruct[39].sLocation = FVector(2280.f, 2620.f, 1929.0626f);


    // F2                                                              
    ItemRandomLocationStruct[40].sLocation = FVector(870.f, -926.f, 2888.0626f);
    ItemRandomLocationStruct[41].sLocation = FVector(1260.f, -467.f, 2888.0626f);
    ItemRandomLocationStruct[42].sLocation = FVector(2170.f, 60.f, 2888.0626f);

    ItemRandomLocationStruct[43].sLocation = FVector(460.f, 800.f, 2888.0626f);
    ItemRandomLocationStruct[44].sLocation = FVector(1100.f, 600.f, 2888.0626f);
    ItemRandomLocationStruct[45].sLocation = FVector(1600.f, 1140.f, 2888.0626f);

    ItemRandomLocationStruct[46].sLocation = FVector(1200.f, 1300.f, 2888.0626f);
    ItemRandomLocationStruct[47].sLocation = FVector(1980.f, 1600.f, 2888.0626f);
    ItemRandomLocationStruct[48].sLocation = FVector(550.f, 1770.f, 2888.0626f);

    ItemRandomLocationStruct[49].sLocation = FVector(170.f, 1120.f, 2888.0626f);
    ItemRandomLocationStruct[50].sLocation = FVector(1240.f, 2270.f, 2888.0626f);
    ItemRandomLocationStruct[51].sLocation = FVector(1410.f, 2650.f, 2888.0626f);

    ItemRandomLocationStruct[52].sLocation = FVector(410.f, 3030.f, 2888.0626f);
    ItemRandomLocationStruct[53].sLocation = FVector(801.f, 3780.f, 2888.0626f);
    ItemRandomLocationStruct[54].sLocation = FVector(2001.f, 3470.f, 2888.0626f);

    ItemRandomLocationStruct[55].sLocation = FVector(1730.f, 1200.f, 2888.0626f);
    ItemRandomLocationStruct[56].sLocation = FVector(260.f, 3780.f, 2888.0626f);
    ItemRandomLocationStruct[57].sLocation = FVector(2310.f, 2040.f, 2888.0626f);

    ItemRandomLocationStruct[58].sLocation = FVector(869.f, 3120.f, 2888.0626f);
    ItemRandomLocationStruct[59].sLocation = FVector(1140.f, 1460.f, 2888.0626f);

    for (int i = 0; i < 60; ++i) {
        ItemRandomLocationStruct[i].bIsSeatLocation = false;
    }

}

FVector ItemController::RandomItemLocation(FLOOR itemfloor)
{
    int RandomNumber = 0;

    int MinIndex = 0;
    int MaxIndex = 0;


    if (itemfloor == FLOOR::FLOOR_B1) {
        MinIndex = 0;
        MaxIndex = 19;
    }
    else if (itemfloor == FLOOR::FLOOR_F1) {
        MinIndex = 20;
        MaxIndex = 39;
    }
    else if (itemfloor == FLOOR::FLOOR_F2) {
        MinIndex = 40;
        MaxIndex = 59;
    }

    random_device rd;
    uniform_int_distribution<> uid(MinIndex, MaxIndex);

    while (true) {
        RandomNumber = uid(rd);

        if (!ItemRandomLocationStruct[RandomNumber].bIsSeatLocation) {
            ItemRandomLocationStruct[RandomNumber].bIsSeatLocation = true;
            return ItemRandomLocationStruct[RandomNumber].sLocation;
        }
    }

    return FVector(0, 0, 0);
}

void ItemController::SpawnItemBoxes(int itemID, std::string itemName, EItemClass itemClass, std::string texturePath, int count, FLOOR itemFloor)
{
    Item_Data newItemData;

    newItemData.itemID = itemID;
    newItemData.itemName = itemName;
    newItemData.itemClass = static_cast<int>(itemClass);
    newItemData.texturePath = texturePath;
    newItemData.count = count;
    newItemData.itemFloor = static_cast<int>(itemFloor);

    // 아이템 초기 위치 설정 (예: 임의의 위치 설정)
    FVector position = RandomItemLocation(itemFloor); // 기본 위치
    newItemData.x = position.x;
    newItemData.y = position.y;
    newItemData.z = position.z;

    items.push_back(newItemData);

    printf("Spawned Item ID: %d, itemName: %s\n", itemID, itemName.c_str());
}

void ItemController::SendItemData(int id)
{
    Protocol::ItemDataList itemDataList;

    for (const auto& item : items) {
        Protocol::set_item* itemProto = itemDataList.add_items();
        itemProto->set_itemid(item.itemID);
        itemProto->set_itemname(item.itemName);
        itemProto->set_itemclass(item.itemClass);
        itemProto->set_texture_path(item.texturePath);
        itemProto->set_count(item.count);
        itemProto->set_floor(item.itemFloor);
        itemProto->set_posx(item.x);
        itemProto->set_posy(item.y);
        itemProto->set_posz(item.z);
    }

    itemDataList.set_packet_type(15);

    std::string serializedData;
    itemDataList.SerializeToString(&serializedData);

    iocpServer->IOCP_SendPacket(id, serializedData.data(), serializedData.size());
}
