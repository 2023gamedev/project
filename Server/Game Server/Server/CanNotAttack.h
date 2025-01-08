#pragma once

#include "Task.h"
#include "iocpServerClass.h"


class TCanNotAttack : public Task {
public:

    string CanSeePlayer(Zombie& zom) const override {
        //cout << "<CanSeePlayer>의 [CanNotAttack Task] 호출" << endl;

        bool result = true;

        if (zom.DistanceTo_PlayerInsight.size() == 0) {
            //cout << "따라서, 좀비 \'#" << zom.ZombieData.zombieID << "\' 에 <CanSeePlayer>의 [CanNotAttack Task] 결과: \"false\"" << endl;
            cout << "Zombie #" << zom.ZombieData.zombieID;
            cout << " DistanceTo_PlayerInsight Map ERROR!!! -> Detected is done [Player is in sight -> (PlayerInSight == true)] but DistanceTo_PlayerInsight Map is empty" << endl;

            return "Fail";
        }

        for (auto player : playerDB_BT[zom.roomid]) {
            // 죽은 플레이어 무시
            if (player.second.health <= 0) {
                continue;
            }
            // 연결 끊긴 플레이어 무시
            if (g_players.find(player.first) == g_players.end()) {
                continue;
            }

            if (zom.DistanceTo_PlayerInsight.find(player.first) != zom.DistanceTo_PlayerInsight.end()) {
                if (zom.DistanceTo_PlayerInsight.at(player.first) > zom.CanAttackDistance && zom.DistanceTo_PlayerInsight.at(player.first) > 0 || zom.DistanceTo_PlayerInsight.at(player.first) <= 0) 
                {   }
                else {  // 사실상 여기에 걸리면 안됨!
                    result = false;
                }
            }

        }

        if (result) {
            //cout << "따라서, 좀비 \'#" << zom.ZombieData.zombieID << "\' 에 <CanSeePlayer>의 [CanNotAttack Task] 결과: " << boolalpha << result << endl;

            return "CanNotAttack-Succeed";
        }
        else {  // 사실상 여기에 걸리면 안됨!
            if (zom.PlayerInSight == false) {
                cout << "Zombie #" << zom.ZombieData.zombieID;
                cout << " PlayerInSight Data Race Occured ERROR!!!" << endl;
            }
            else {
                cout << "Zombie #" << zom.ZombieData.zombieID;
                cout << " got ERROR!!! And I dont know whhhhhyyyyy!!!" << endl;
            }

            //cout << "따라서, 좀비 \'#" << zom.ZombieData.zombieID << "\' 에 <CanSeePlayer>의 [CanNotAttack Task] 결과: " << boolalpha << result << endl;

            return "Fail";
        }
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