#include <iostream>

#include "Task.h"
#include "Selector.h"
#include "CanSeePlayer.h"
#include "HasInvestigated.h"
#include "NotHasLastKnownPlayerLocation.h"


int main()
{
	//플레이어 초기 위치
	vector<vector<vector<int>>> pl = vector<vector<vector<int>>>{ {{-8, 9, 12}} };
	//좀비 초기 위치
	vector<vector<vector<int>>> zl = vector<vector<vector<int>>>{ {{2, 8, 7}} };

	cout << "플레이어의 시작 위치: ( " << pl[0][0][0] << ", " << pl[0][0][1] << ", " << pl[0][0][2] << " )" << endl;
	cout << "좀비의 시작 위치: ( " << zl[0][0][0] << ", " << zl[0][0][1] << ", " << zl[0][0][2] << " )" << endl;
	cout << endl;

	//플레이어 인스턴스
	Player* p = new Player(pl);
	//좀비 인스턴스
	Zombie* z = new Zombie(p, "zombieee", zl);


	//=======좀비 BT 생성=======
	//Selector-Detect 선언 (사실상 최상위 노드)
	Selector sel_detect;
	
	//Selector-Detect가 가지는 Task들 메모리 할당

	//CanSeePlayer-Task
	TCanSeePlayer* t_canseeplayer = new TCanSeePlayer;
	//HasInvestigated-Task
	THasInvestigated* t_hasinvestigated = new THasInvestigated;
	//NotHasLastKnownPlayerLocation-Task
	TNotHasLastKnownPlayerLocation* t_nothaslastknownplayerlocation = new TNotHasLastKnownPlayerLocation;
	
	//Selector-Detect에 해당 Task들 '순서대로' 삽입
	sel_detect.AddChild(t_canseeplayer);
	sel_detect.AddChild(t_hasinvestigated);
	sel_detect.AddChild(t_nothaslastknownplayerlocation);
	
	//Selector-Detect 실행
	sel_detect.Sel_Detect(*z);
	//==========================

	
	//할당한 메모리 해제
	delete(p);
	delete(z);
	delete(t_canseeplayer);


	return 0;
}