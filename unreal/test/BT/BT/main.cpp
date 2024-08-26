#include <iostream>

#include "Task.h"
#include "Selector.h"
#include "Sequence.h"
#include "CanSeePlayer.h"
#include "HasInvestigated.h"
#include "NotHasLastKnownPlayerLocation.h"
#include "CanNotAttack.h"
#include "CanAttack.h"
#include "MoveTo.h"
#include "Attack.h"


int main()
{
	//플레이어 초기 위치
	vector<vector<vector<int>>> pl = vector<vector<vector<int>>>{ {{-8/*2*//*80*/, 9, 12}} };
	//좀비 초기 위치
	vector<vector<vector<int>>> zl = vector<vector<vector<int>>>{ {{2, 8, 12}} };

	//플레이어 인스턴스
	Player* p = new Player(pl);
	//좀비 인스턴스
	Zombie* z = new Zombie(p, "zombieee", zl);

	vector<vector<vector<int>>> p_l = p->GetPlayerPos();
	int p_x = p_l[0][0][0]; int p_y = p_l[0][0][1]; int p_z = p_l[0][0][2];
	int z_x = z->ZombieLocation[0][0][0]; int z_y = z->ZombieLocation[0][0][1]; int z_z = z->ZombieLocation[0][0][2];
	cout << "플레이어의 시작 위치: ( " << p_x << ", " << p_y << ", " << p_z << " )" << endl;
	cout << "좀비의 시작 위치: ( " << z_x << ", " << z_y << ", " << z_z << " )" << endl;
	cout << endl;


	//======[좀비 BT 생성]======

	//==========선언============

	//<Selector> 선언 

	//<Selector-Detect> (사실상 최상위 노드)
	Selector sel_detect;
	//<Selector-CanSeePlayer>  
	Selector sel_canseeplayer;

	//{Sequence} 선언

	//{Sequence-CanNotAttack}
	Sequence seq_cannotattack;
	//{Sequence-CanAttack}
	Sequence seq_canattack;
	//
	//{Sequence-HasInvestigated}
	Sequence seq_hasinvestigated;
	//{Sequence-NotHasLastKnownPlayerLocation}
	Sequence seq_nothaslastknownplayerlocation;


	//[Task] 선언 & 메모리 할당

	//<Selector Detact> 가 가지는 Task들

	//[CanSeePlayer-Task]
	TCanSeePlayer* t_canseeplayer = new TCanSeePlayer;
	//[HasInvestigated-Task]
	THasInvestigated* t_hasinvestigated = new THasInvestigated;
	//[NotHasLastKnownPlayerLocation-Task]
	TNotHasLastKnownPlayerLocation* t_nothaslastknownplayerlocation = new TNotHasLastKnownPlayerLocation;

	//<Selector CanSeePlayer> 가 가지는 Task들

	//[CanNotAttack-Task]
	TCanNotAttack* t_cannotattack = new TCanNotAttack;
	//[CanAttack-Task]
	TCanAttack* t_canattack = new TCanAttack;

	//{Sequence} 가 가지는 Task들

	//[MoveTo-Task]
	TMoveTo* t_moveto = new TMoveTo;
	//[Attack-Task]
	TAttack* t_attack = new TAttack;


	//========작업 할당==========

	//<Selector-Detect> 할당
	//<Selector-Detect>에 해당 Task들 '순서대로' 삽입
	sel_detect.AddChild(t_canseeplayer);
	sel_detect.AddChild(t_hasinvestigated);
	sel_detect.AddChild(t_nothaslastknownplayerlocation);

	//<Selector-CanSeePlayer> 할당
	//<Selector-CanSeePlayer>에 해당 Task들 '순서대로' 삽입
	sel_canseeplayer.AddChild(t_cannotattack);
	sel_canseeplayer.AddChild(t_canattack);

	//{Sequence-CanNotAttack} 할당
	//{Sequence-CanNotAttack}에 해당 Task들 '순서대로' 삽입
	seq_cannotattack.AddChild(t_moveto);

	//{Sequence-CanAttack} 할당
	//{Sequence-CanAttack}에 해당 Task들 '순서대로' 삽입
	seq_canattack.AddChild(t_attack);

	//{Sequence-HasInvestigated} 할당
	//{Sequence-HasInvestigated}에 해당 Task들 '순서대로' 삽입
	seq_hasinvestigated.AddChild(t_moveto);

	//{Sequence-NotHasLastKnownPlayerLocation} 할당
	//{Sequence-NotHasLastKnownPlayerLocation}에 해당 Task들 '순서대로' 삽입
	seq_nothaslastknownplayerlocation.AddChild(t_moveto);


	//========작업 실행==========

	string result = "Initial";

	while (true) {

		cout << "========BT 실행==========" << endl;
		cout << endl;

		vector<vector<vector<int>>> p_l = p->GetPlayerPos();
		int p_x = p_l[0][0][0]; int p_y = p_l[0][0][1]; int p_z = p_l[0][0][2];
		int z_x = z->ZombieLocation[0][0][0]; int z_y = z->ZombieLocation[0][0][1]; int z_z = z->ZombieLocation[0][0][2];
		cout << "플레이어의 현제 위치: ( " << p_x << ", " << p_y << ", " << p_z << " )" << endl;
		cout << "좀비의 현제 위치: ( " << z_x << ", " << z_y << ", " << z_z << " )" << endl;
		cout << endl;

		//좀비와 플레이어의 거리 갱신
		z->SetDistance();

		//<Selector-Detect> 실행
		result = sel_detect.Sel_Detect(*z);

		//<Selector-Detect> 결과 값에 따라 다음 Task들 실행
		if (result == "CanSeePlayer-Succeed") {

			//<Selector-CanSeePlayer> 실행
			result = sel_canseeplayer.Sel_CanSeePlayer(*z);

			//<Selector-CanSeePlayer> 결과 값에 따라 다음 Task들 실행
			if (result == "CanNotAttack-Succeed") {

				//{Sequence-CanNotAttack} 실행
				result = seq_cannotattack.Seq_CanNotAttack(*z);

			}
			else if (result == "CanAttack-Succeed") {

				//{Sequence-CanAttack} 실행
				result = seq_canattack.Seq_CanAttack(*z);

			}
			else {	//result == "Fail"
				cout << "EEEERRRROOOOOORRRR" << endl;
			}
		}
		else if (result == "HasInvestigated-Succeed") {

			//{Sequence-HasInvestigated} 실행
			result = seq_hasinvestigated.Seq_HasInvestigated(*z);

		}
		else if (result == "NotHasLastKnownPlayerLocation-Succeed") {

			//{Sequence-NotHasLastKnownPlayerLocation} 실행
			result = seq_nothaslastknownplayerlocation.Seq_NotHasLastKnownPlayerLocation(*z);

		}
		else {	//result == "Fail"
			cout << "EEEERRRROOOOOORRRR" << endl;
		}

		//플레이어 입력 받기
		bool proper_input = false;
		int input_x = 0;	// 6:x++, 4:x--
		int input_y = 0;	// 8:y++, 2:y--
		while (proper_input == false) {
			//cout << "플레이어 이동=> 6: x++ , 4: x-- , 8: y++ , 2: y--" << endl;
			cout << "플레이어 위치 설정" << endl;
			//cout << "x축 이동 입력: ";
			cout << "x좌표 입력: ";
			cin >> input_x;
			if (cin.fail()) {
				cout << "잘못된 입력입니다 다시 입력해 주세요." << endl;
				cout << endl;
				cin.clear();
				cin.ignore(1000, '\n');
				continue;
			}
			cin.clear();
			cin.ignore(1000, '\n');
			//cout << "y축 이동 입력: ";
			cout << "y좌표 입력: ";
			cin >> input_y;
			if (cin.fail()) {
				cout << "잘못된 입력입니다 다시 입력해 주세요." << endl;
				cout << endl;
				cin.clear();
				cin.ignore(1000, '\n');
				continue;
			}
			cin.clear();
			cin.ignore(1000, '\n');
			cout << endl;
			

			/*if ((input_x == 6 || input_x == 4) && (input_y == 8 || input_y == 2)) {
				proper_input = true;

				if (input_x == 6) {
					cout << "x축으로 ++ , ";
					p->Move(1, 0, 0);
				}
				else if (input_x == 4) {
					cout << "x축으로 -- , ";
					p->Move(-1, 0, 0);
				}
				else
					cout << "[Error]!!!";

				if (input_y == 8) {
					cout << "y축으로 ++ ";
					p->Move(0, 1, 0);
				}
				else if (input_y == 2) {
					cout << "y축으로 -- ";
					p->Move(0, -1, 0);
				}
				else
					cout << "[Error]!!!";

				cout << "이동!" << endl;
				cout << endl;
			}
			else {
				proper_input = false;

				cout << "잘못된 입력입니다 다시 입력해 주세요." << endl;
				cout << endl;
			}*/

			p->Teleport(input_x, input_y, 00000);
			break;

		}
	}
	
	//==========================

	
	//할당한 메모리 해제
	delete(p);
	delete(z);

	delete(t_canseeplayer);
	delete(t_cannotattack);
	delete(t_moveto);
	delete(t_canattack);
	delete(t_attack);
	delete(t_hasinvestigated);
	delete(t_nothaslastknownplayerlocation);

	return 0;
}