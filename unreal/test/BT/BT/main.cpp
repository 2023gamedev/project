#include <iostream>
#include "Task.h"

int main()
{
	vector<vector<vector<int>>> pl = vector<vector<vector<int>>>{ {{-8, 9, 12}} };

	Player* p = new Player();
	//Player* p = new Player(pl);

	vector<vector<vector<int>>> zl = vector<vector<vector<int>>>{ {{2, 8, 7}} };

	//cout << zl[0][0][0] << ", " << zl[0][0][1] << ", " << zl[0][0][2] << endl;

	Zombie* z = new Zombie(p, "zombieee", zl);

	cout << z->DistanceToPlayer << endl;

	float a = sqrt(powf(zl[0][0][0] - pl[0][0][0], 2) + powf(zl[0][0][1] - pl[0][0][1], 2) + powf(zl[0][0][2] - pl[0][0][2], 2));

	cout << a << endl;

	return 0;
}