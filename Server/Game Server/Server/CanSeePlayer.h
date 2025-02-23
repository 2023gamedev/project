#pragma once

#include "Selector.h"


class Sel_CanSeePlayer : public Selector {
public:

    bool Detect(Zombie& zom) override {
#ifdef ENABLE_BT_NODE_LOG
        cout << "<Detect>의 (CanSeePlayer Decorator) 호출" << endl;
#endif

        d_result = zom.PlayerInSight_Update_Check();
#ifdef ENABLE_BT_NODE_LOG
        cout << "좀비 \'#" << zom.ZombieData.zombieID << "\' 의 시야에 플레이어가 있는가?: " << boolalpha << d_result << endl;
#endif

        if (d_result == true) {
#ifdef ENABLE_BT_DETECT_RANDOMCHANCE_LOG
            vector<vector<vector<float>>> closest_player_pos = {};
            float dist = zom.SearchClosestPlayer(closest_player_pos, 1);
            cout << "좀비 \'#" << zom.ZombieData.zombieID << "\' 와 가장 가까운 플레이어 사이의 거리: " << dist << endl;
#endif

            if (zom.detectCanSeePlayer_randomChance == false) {

                auto delayAfterTime = std::chrono::high_resolution_clock::now();
                std::chrono::duration<float> deltaTime = delayAfterTime - zom.detectCanSeePlayerFail_StartTime;

                if (deltaTime.count() >= zom.detectCanSeePlayerFail_delayTime) {

                    d_result = zom.CanSeePlayerRandomChance();  // 포착 랜덤확률 부여
                    if (d_result == false) {
#ifdef ENABLE_BT_DETECT_RANDOMCHANCE_LOG
                        cout << "좀비 \'#" << zom.ZombieData.zombieID << "\' Detect-CanSeePlayer(플레이어 포착) RandomChance 실패!!!!!!!" << endl;
#endif
                        zom.detectCanSeePlayerFail_StartTime = std::chrono::high_resolution_clock::now();
                    }
                    else {
#ifdef ENABLE_BT_DETECT_RANDOMCHANCE_LOG
                        cout << "좀비 \'#" << zom.ZombieData.zombieID << "\' Detect-CanSeePlayer(플레이어 포착) RandomChance 성공!" << endl;
#endif
                    }
                }
                else {
#ifdef ENABLE_BT_DETECT_RANDOMCHANCE_LOG
                    cout << "좀비 \'#" << zom.ZombieData.zombieID << "\' Detect-CanSeePlayer(플레이어 포착) RandomChance 실패 상태" << endl;
                    cout << "남은 시간: " << zom.detectCanSeePlayerFail_delayTime - deltaTime.count() << "s" << endl;
#endif
                    d_result = false; // 남은 딜레이 시간 동안 실패 상태 유지
                }

            }

        }
        else {
            zom.detectCanSeePlayer_randomChance = false;    // 다시 초기화
        }

#ifdef ENABLE_BT_NODE_LOG
        cout << "따라서, 좀비 \'#" << zom.ZombieData.zombieID << "\' 에 <Detect>의 (CanSeePlayer Decorator) 결과: \"" << boolalpha << d_result << "\"" << endl;
        cout << endl;
#endif

        bool Prev_CanSeePlayer_result = zom.CanSeePlayer_result;
        if (Prev_CanSeePlayer_result == false && d_result) {    // 플레이어를 처음 발견했거나 플레이어를 놓쳤다가 다시 발견했다면 -> 호드 사운드 재생
            zom.MakeNoise();    // 싱글톤 패턴 활용
        }

        zom.CanSeePlayer_result = d_result;

        if (d_result == true)
            CanSeePlayer(zom);

        return d_result;
    }

    bool CanSeePlayer(Zombie& zom) override {
#ifdef ENABLE_BT_NODE_LOG
        cout << "Selector <CanSeePlayer> 호출" << endl;
        cout << endl;
#endif

        d_result = false;
        for (const auto& child : sel_children) {
            bool task_result = child->CanSeePlayer(zom);    // 다형성 이용 (함수 오버라이딩)

            if (d_result == false)
                d_result = task_result;
        }

        if (d_result == false) {  // selector의 모든 task가 실패 할 경우(그럴 일은 없어야 하지만..)
            cout << "\"Selector CanSeePlayer [ERROR]!!!\" - ZombieID #" << zom.ZombieData.zombieID << endl;
            cout << endl;
        }

        return d_result;
    }

};