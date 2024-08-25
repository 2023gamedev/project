#include <iostream>

#include "Task.h"
#include "Selector.h"
#include "CanSeePlayer.h"
#include "HasInvestigated.h"
#include "NotHasLastKnownPlayerLocation.h"
#include "CanNotAttack.h"
#include "CanAttack.h"


int main()
{
	//플레이어 초기 위치
	vector<vector<vector<int>>> pl = vector<vector<vector<int>>>{ {{-8, 9, 12}} };
	//좀비 초기 위치
	vector<vector<vector<int>>> zl = vector<vector<vector<int>>>{ {{-8, 8, 7}} };

	cout << "플레이어의 시작 위치: ( " << pl[0][0][0] << ", " << pl[0][0][1] << ", " << pl[0][0][2] << " )" << endl;
	cout << "좀비의 시작 위치: ( " << zl[0][0][0] << ", " << zl[0][0][1] << ", " << zl[0][0][2] << " )" << endl;
	cout << endl;

	//플레이어 인스턴스
	Player* p = new Player(pl);
	//좀비 인스턴스
	Zombie* z = new Zombie(p, "zombieee", zl);


	//=======좀비 BT 생성=======
	//<Selector> 선언 
	
	//<Selector-Detect> (사실상 최상위 노드)
	Selector sel_detect;
	//<Selector-CanSeePlayer>  
	Selector sel_canseeplayer;


	//Selector들 선언 & 메모리 할당
 
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


	//<Selector-Detect> 할당
	//<Selector-Detect>에 해당 Task들 '순서대로' 삽입
	sel_detect.AddChild(t_canseeplayer);
	sel_detect.AddChild(t_hasinvestigated);
	sel_detect.AddChild(t_nothaslastknownplayerlocation);
	
	//<Selector-Detect> 실행
	string result = sel_detect.Sel_Detect(*z);


	//<Selector-Detect> 결과 값에 따라 다음 Task들 할당
	if (result == "CanSeePlayer-Succeed") {

		//<Selector-CanSeePlayer> 할당
		//<Selector-CanSeePlayer>에 해당 Task들 '순서대로' 삽입
		sel_canseeplayer.AddChild(t_cannotattack);
		sel_canseeplayer.AddChild(t_canattack);

		//<Selector-CanSeePlayer> 실행
		string result = sel_canseeplayer.Sel_CanSeePlayer(*z);

	}
	else if (result == "HasInvestigated-Succeed") {

	}
	else if (result == "NotHasLastKnownPlayerLocation-Succeed") {

	}
	else {	//result == "Fail"
		cout << "EEEERRRROOOOOORRRR" << endl;
	}
	
	//+ 하나의 Selector 변수로 여러번 재사용하며 작업하려 한다면 멤버 변수의 sel_children 초기화 필요
	//==========================

	
	//할당한 메모리 해제
	delete(p);
	delete(z);
	delete(t_canseeplayer);


	return 0;
}