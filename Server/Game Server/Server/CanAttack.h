#pragma once

#include "Sequence.h"


class Seq_CanAttack : public Sequence {
public:

    bool CanSeePlayer(Zombie& zom) override {
#ifdef ENABLE_BT_NODE_LOG
        cout << "<CanSeePlayer>의 [CanAttack Task] 호출" << endl;
#endif

        result = false;

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
#ifdef ENABLE_BT_NODE_LOG
                cout << "플레이어\'#" << player.first << "\' 좀비 \'#" << zom.ZombieData.zombieID << "\' 의 시야에 있음!" << endl;
                cout << "플레이어\'#" << player.first << "\' 와 좀비 \'#" << zom.ZombieData.zombieID << "\' 의 거리: " << zom.DistanceTo_PlayerInsight.at(player.first) << endl;
                cout << "좀비 \'#" << zom.ZombieData.zombieID << "\' 의 공격 사거리: " << zom.CanAttackDistance << endl;
#endif

                if (zom.DistanceTo_PlayerInsight.at(player.first) <= zom.CanAttackDistance && zom.DistanceTo_PlayerInsight.at(player.first) > 0) {
#ifdef ENABLE_BT_NODE_LOG
                    cout << "플레이어\'#" << player.first << "\' 좀비 \'#" << zom.ZombieData.zombieID << "\' 의 공격 사거리 내에 있음!" << endl;
#endif

                    result = true;  // 한명이라도 공격 범위 들어오면 -> Attack 수행
                    //break;          // 각 플레이어 마다 쭉 모두 로그 찍고 싶으면 break 하면 안 됨 (대신 그럴게 아니라면 break 넣어서 최적화하기)
                }
                else {
#ifdef ENABLE_BT_NODE_LOG
                    cout << "플레이어\'#" << player.first << "\' 좀비 \'#" << zom.ZombieData.zombieID << "\' 의 공격 사거리 내에 없음!" << endl;
#endif
                }
            }
            else {
#ifdef ENABLE_BT_NODE_LOG
                cout << "플레이어\'#" << player.first << "\' 좀비 \'#" << zom.ZombieData.zombieID << "\' 의 시야에 없음!" << endl;
#endif
            }
        }

        if (zom.IsAttacking == true) {      // 만약 좀비 공격 애니메이션이 아직 재생 중이라면 -> 좀비 정지 시켜 놓기
                                            // BT 시작시에 이미 zom.HaveToWait을 이용해서 Wait으로 들어가게 구현했지만 혹시 몰라서;;
#ifdef ENABLE_BT_NODE_LOG
            cout << "따라서, 좀비 \'#" << zom.ZombieData.zombieID << "\' 에 <CanSeePlayer>의 [CanAttack Task] 결과: \"false\" - \'Attack Animation is still on playing\'" << endl;
#endif
        }

#ifdef ENABLE_BT_NODE_LOG
        if (result) {
            cout << "따라서, 좀비 \'#" << zom.ZombieData.zombieID << "\' 에 <CanSeePlayer>의 [CanAttack Task] 결과: \"true\"" << endl;
        }
        else {
            cout << "따라서, 좀비 \'#" << zom.ZombieData.zombieID << "\' 에 <CanSeePlayer>의 [CanAttack Task] 결과: \"false\"" << endl;
        }
        cout << endl;
#endif

        return result;
    }

};