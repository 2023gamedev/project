#pragma once

#include "Sequence.h"


class Seq_CanNotAttack : public Sequence {
public:

    bool CanSeePlayer(Zombie& zom) override {
#ifdef ENABLE_BT_NODE_LOG
        cout << "<CanSeePlayer>의 (CanNotAttack Decorator) 호출" << endl;
#endif

        result = true;

        if (zom.DistanceTo_PlayerInsight.size() == 0) {
#ifdef ENABLE_BT_NODE_LOG
            cout << "따라서, 좀비 \'#" << zom.ZombieData.zombieID << "\' 에 <CanSeePlayer>의 (CanNotAttack Decorator) 결과: \"false\"" << endl;
#endif
            cout << "Zombie #" << zom.ZombieData.zombieID;
            cout << " DistanceTo_PlayerInsight Map ERROR!!! -> Player is in sight (PlayerInSight == true) but DistanceTo_PlayerInsight Map is empty" << endl;
#ifdef ENABLE_BT_NODE_LOG
            cout << endl;
#endif

            result = false;
            return result;
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
                else {  
                    result = false;
                }
            }

        }

        if (result) {
#ifdef ENABLE_BT_NODE_LOG
            cout << "따라서, 좀비 \'#" << zom.ZombieData.zombieID << "\' 에 <CanSeePlayer>의 (CanNotAttack Decorator) 결과: " << boolalpha << result << endl;
            cout << endl;
#endif
        }
        else {  // 사실상 여기에 걸리면 안됨! (CanNotAttack은 항상 CanAttack 검사가 실패할 경우에만 실행되므로 (CanSeePlayer 시퀀스로 직렬적, 순차적으로 작동))
            if (zom.PlayerInSight == false) {
                cout << "Zombie #" << zom.ZombieData.zombieID;
                cout << " PlayerInSight Data Race Occured ERROR!!! -> CanSeePlayer Task is excecuted (PlayerInSight has to be true) but now PlayerInSight is false" << endl;
            }
            else {
                //cout << "Zombie #" << zom.ZombieData.zombieID;
                //cout << " got ERROR!!! And I dont know whhhhhyyyyy!!!" << endl;
                // 만약, Detect Selector 를 병렬적으로 작동시키면 해당 에러 로그 필요 X
            }

#ifdef ENABLE_BT_NODE_LOG
            cout << "따라서, 좀비 \'#" << zom.ZombieData.zombieID << "\' 에 <CanSeePlayer>의 (CanNotAttack Decorator) 결과: " << boolalpha << result << endl;
            cout << endl;
#endif
        }

        if (result == true)
            CanNotAttack(zom);

        return result;
    }

    bool CanNotAttack(Zombie& zom) override {
#ifdef ENABLE_BT_NODE_LOG
        cout << "Sequence {CanNotAttack} 호출" << endl;
        cout << endl;
#endif

        for (const auto& child : seq_children) {
            result = child->CanNotAttack(zom);
        }

        if (result == false) {
            cout << "\"Sequence CanNotAttack [ERROR]!!!\" - ZombieID #" << zom.ZombieData.zombieID << endl;
            cout << endl;
        }

        return result;
    }

};