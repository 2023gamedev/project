#pragma once

#include "Task.h"
#include "iocpServerClass.h"


class TCanAttack : public Task {
public:

    string CanSeePlayer(Zombie& zom) const override {
        //cout << "<CanSeePlayer>의 [CanAttack Task] 호출" << endl;

        bool result = false;

        for (auto player : playerDB_BT) {
            // 죽은 플레이어 무시
            if (player.second.health <= 0) {
                continue;
            }
            // 연결 끊긴 플레이어 무시
            if (g_players.find(player.first) == g_players.end()) {
                continue;
            }

            if (zom.DistanceTo_PlayerInsight.find(player.first) != zom.DistanceTo_PlayerInsight.end()) {
                //cout << "플레이어\'#" << player.first << "\' 좀비 \'#" << zom.ZombieData.zombieID << "\' 의 시야에 있음!" << endl;

                //cout << "플레이어\'#" << player.first << "\' 와 좀비 \'#" << zom.ZombieData.zombieID << "\' 의 거리: " << zom.DistanceTo_PlayerInsight.at(player.first) << endl;
                //cout << "좀비 \'#" << zom.ZombieData.zombieID << "\' 의 공격 사거리: " << zom.CanAttackDistance << endl;

                if (zom.DistanceTo_PlayerInsight.at(player.first) <= zom.CanAttackDistance && zom.DistanceTo_PlayerInsight.at(player.first) > 0) {
                    //cout << "플레이어\'#" << player.first << "\' 좀비 \'#" << zom.ZombieData.zombieID << "\' 의 공격 사거리 내에 있음!" << endl;
                    
                    result = true;  // 한명이라도 공격 범위 들어오면 -> Attack 수행
                    //break;        // 각 플레이어 마다 쭉 모두 로그 찍고 싶으면 break 하면 안 됨 (대신 그럴게 아니라면 break 넣어서 최적화하기)
                }
                else {
                    //cout << "플레이어\'#" << player.first << "\' 좀비 \'#" << zom.ZombieData.zombieID << "\' 의 공격 사거리 내에 없음!" << endl;
                }
            }
            else {
                //cout << "플레이어\'#" << player.first << "\' 좀비 \'#" << zom.ZombieData.zombieID << "\' 의 시야에 없음!" << endl;
            }
        }

        if (zom.IsAttacking == true) {      // 만약 좀비 공격 애니메이션이 아직 재생 중이라면 -> 좀비 정지 시켜 놓기
                                            // BT 시작시에 이미 zom.HaveToWait을 이용해서 Wait으로 들어가게 구현했지만 혹시 몰라서;;

            //cout << "따라서, 좀비 \'#" << zom.ZombieData.zombieID << "\' 에 <CanSeePlayer>의 [CanAttack Task] 결과: \"false\" - \'Attack Animation is still on playing\'" << endl;

            return "CanAttack-Succeed[Attack Animation is still on playing]";
        }

        if (result) {
            //cout << "따라서, 좀비 \'#" << zom.ZombieData.zombieID << "\' 에 <CanSeePlayer>의 [CanAttack Task] 결과: \"true\"" << endl;

            return "CanAttack-Succeed";
        }
        else {
            //cout << "따라서, 좀비 \'#" << zom.ZombieData.zombieID << "\' 에 <CanSeePlayer>의 [CanAttack Task] 결과: \"false\"" << endl;

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