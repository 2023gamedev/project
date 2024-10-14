//#include "ItemController.h"
//
//uniform_int_distribution uid;
//
//ItemController::ItemController(IOCP_CORE& mainServer)
//{
//	iocpServer = &mainServer;
//
//    
//}
//
//ItemController::~ItemController()
//{
//}
//
//void ItemController::SpawnRandomItem(FLOOR itemfloor)
//{
//    int RandomNumber = 0;
//
//    int MinIndex = 0;
//    int MaxIndex = 0;
//
//
//    if (itemfloor == FLOOR::FLOOR_B1) {
//        MinIndex = 0;
//        MaxIndex = 19;
//    }
//    else if (itemfloor == FLOOR::FLOOR_F1) {
//        MinIndex = 20;
//        MaxIndex = 39;
//    }
//    else if (itemfloor == FLOOR::FLOOR_F2) {
//        MinIndex = 40;
//        MaxIndex = 59;
//    }
//
//    random_device rd;
//    uniform_int_distribution<> uid(MinIndex, MaxIndex);
//
//    while (true) {
//        RandomNumber = uid();
//
//        if (!ItemRandomLocationStruct[RandomNumber].bIsSeatLocation) {
//            ItemRandomLocationStruct[RandomNumber].bIsSeatLocation = true;
//            return ItemRandomLocationStruct[RandomNumber].sLocation;
//        }
//    }
//
//    return FVector::ZeroVector;
//}