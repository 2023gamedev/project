#pragma once

#include "Task.h"


class TNotHasLastKnownPlayerLocation : public Task {
public:

    bool Detect(Zombie& zom) override {
        //cout << "<Detect>의 [NotHasLastKnownPlayerLocation Task] 호출" << endl;

        result = !zom.KnewPlayerLocation;

        //cout << "좀비 플레이어의 최신 위치 정보를 가지고 있는가?: " << boolalpha << !result << endl;
        //if (!result) {
        //    cout << "좀비가 가진 플레이어 최신 위치: ( " << zom.PrevTargetLocation[0][0][0] << ", " << zom.PrevTargetLocation[0][0][1] << ", " << zom.PrevTargetLocation[0][0][2] << " )" << endl;
        //}
        cout << "따라서, 좀비 \'#" << zom.ZombieData.zombieID << "\' 에 <Detect>의 [NotHasLastKnownPlayerLocation Task] 결과: \"" << boolalpha << result << "\"" << endl;
        //cout << endl;

        return result;
    }

};