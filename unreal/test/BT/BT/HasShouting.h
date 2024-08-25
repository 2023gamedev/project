#pragma once

#include "Task.h"


class THasShouting : public Task {
public:

    string Detect(Zombie zom) const override {
        //샤우팅 좀비로 부터 위치를 받아와야 하므로 -> 따로 작업 필요

        //string result = (zom.DistanceToSZombie <= zom.CanHearShoutDistance); //DistanceToSZombie: 샤우팅 좀비로 부터의 거리, CanHearShoutDistance: 샤우팅을 들을 수 있는 거리 (const 변수)

        //zom.HeardShouting = reuslt;

        //return result;
        return "Fail";
    }

};