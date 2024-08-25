#include <iostream>

#include "Task.h"
#include "Selector.h"
#include "CanSeePlayer.h"


int main()
{
	vector<vector<vector<int>>> pl = vector<vector<vector<int>>>{ {{-8, 9, 12}} };
	vector<vector<vector<int>>> zl = vector<vector<vector<int>>>{ {{2, 8, 7}} };

	Player* p = new Player();

	Zombie* z = new Zombie(p, "zombieee", zl);


	class Test {
	public:
		int A, B;

		Test(){}
		Test(int a, int b) : A(a), B(b) {}
	};

	unique_ptr<Test> t(new Test(21, 9));
	//cout << t.get()->A << endl;


	vector<int> v;
	v.emplace_back(6);
	//cout << v.front() << endl;


	vector<Test> t1;
	t1.emplace_back(Test(8, 4));
	//cout << t1.front().A << endl;

	vector<unique_ptr<Test>> t2;
	unique_ptr<Test> tt2(new Test(56, 48));
	//cout << tt2.get()->A << endl;
	t2.emplace_back(move(tt2));
	//cout << t2.front().get()->A << endl;
	//cout << tt2.get()->A << endl; //-> error: move함수로 소유권을 넘겨줬으므로 메모리가 해제됨

	vector<unique_ptr<Test>> t_2;
	t_2.emplace_back(make_unique<Test>(45, 37));
	//cout << t_2.front().get()->A << endl;
	

	//int* tmp = new int;
	//*tmp = 6;
	//cout << *tmp << endl;

	vector<unique_ptr<Test>>* t3 = new vector<unique_ptr<Test>>;
	t3->emplace_back(make_unique<Test>(454, 373));
	//cout << t3->front().get()->A << endl;


	//z->Z_BT->emplace_back(make_unique<Selector>(z->Z_BT));
	
	TCanSeePlayer t_canseeplayer;
	Selector sel_detect;
	//cout << sel_detect.sel_children->size() << endl;
	//cout << typeid(sel_detect.sel_children->front()).name() << endl;
	//sel_detect.AddChild(make_unique<Selector>(t_canseeplayer));
	//cout << sel_detect.sel_children->size() << endl;
	//cout << typeid(sel_detect.sel_children->front()).name() << endl;

	//cout << typeid(t_canseeplayer).name() << endl;
	//cout << typeid(make_unique<Selector>(t_canseeplayer)).name() << endl;
	//sel_detect.Sel_Detect(*z);


	TCanSeePlayer* pt_canseeplayer = new TCanSeePlayer;
	Selector psel_detect;
	psel_detect.AddChild(pt_canseeplayer);
	psel_detect.Sel_Detect(*z);

	return 0;
}