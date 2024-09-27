#pragma once

#include "Task.h"
#include "Zombie.h"


class Zombie;

class TCanAttack : public Task {
public:

    string CanSeePlayer(Zombie& zom) const override {
        bool result = false;
        if(zom.DistanceToPlayers.find(zom.bt_playerID) != zom.DistanceToPlayers.end())
            result = (zom.DistanceToPlayers.at(zom.bt_playerID) <= zom.CanAttackDistance);

        //cout << "<CanSeePlayer>의 [CanAttack Task] 호출" << endl;
        //cout << "플레이어\'#" << zom.bt_playerID << "\' 와 좀비 \'#" << zom.ZombieData.zombieID << "\' 의 거리: " << zom.DistanceToPlayers.at(zom.bt_playerID) << endl;
        //cout << "좀비 \'#" << zom.ZombieData.zombieID << "\' 의 공격 사거리: " << zom.CanAttackDistance << endl;
        //cout << "따라서, 플레이어 \'#" << zom.bt_playerID << "\' 에 대한 좀비 \'#" << zom.ZombieData.zombieID << "\' 에 <CanSeePlayer>의 [CanAttack Task] 결과: \"" << boolalpha << result << "\"" << endl;
        //cout << endl;

        if(zom.IsAttacking == true)         // 만약 좀비 공격 애니메이션이 아직 재생 중이라면 -> 좀비 정지 시켜 놓기
            return "CanAttack-Succeed";

        if (result) {
            cout << "플레이어\'#" << zom.bt_playerID << "\' 와 좀비 \'#" << zom.ZombieData.zombieID << "\' 의 거리: " << zom.DistanceToPlayers.at(zom.bt_playerID) << endl;
            cout << "좀비 \'#" << zom.ZombieData.zombieID << "\' 의 공격 사거리: " << zom.CanAttackDistance << endl;

            return "CanAttack-Succeed";
        }
        else
            return "Fail";
    }

    //사실상 더미 함수들
    string Detect(Zombie& zom) const override { return "Fail"; };
    //string CanSeePlayer(Zombie& zom) const override { return "Fail"; };
    string CanAttack(Zombie& zom) const override { return "Fail"; };
    string CanNotAttack(Zombie& zom) const override { return "Fail"; };
    string HasShouting(Zombie& zom) const override { return "Fail"; };
    string HasFootSound(Zombie& zom) const override { return "Fail"; };
    string HasInvestigated(Zombie& zom) const override { return "Fail"; };
    string NotHasLastKnownPlayerLocation(Zombie& zom) const override { return "Fail"; };

};