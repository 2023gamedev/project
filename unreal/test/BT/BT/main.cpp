#include "Tree.h"
#include <iostream>
#include <vector>

using namespace std;

class A {
public:
	static float a;
	static const float c;
	const float d = 5.f;
	const static float f;
	static vector<vector<vector<int>>> v;

	A() {
		cout << a << "->";
	}

	A(int b) {
		a = b;

		cout << a << "->";
	}

	void p() {
		a++;

		cout << a << "->";
	}

	void pv() {
		for (int i = 0; i < v.size(); i++) {
			for (int j = 0; j < v[i].size(); j++) {
				for (int k = 0; k < v[i][j].size(); k++) {
					cout << v[i][j][k] << " ";
				}
				cout << endl;
			}
		}
	}

	void set(int b) {
		a = b;

		cout << a << "->";
	}

	void set_v() {
		v.push_back({ { 1, 2, 3 }, { 3, 2, 1 } });
		v.push_back({ { 4, 5, 6 }, { 6, 5, 4 } });

		pv();
	}

	~A() {}
};

float A::a = 10.f;
const float A::c = 5.f;
const float A::f = 50.f;
vector<vector<vector<int>>> A::v = vector<vector<vector<int>>>{ { { 6, 5, 4 }, { 4, 5, 6 }, { 3, 2, 1 }, { 1, 2, 3 } } };//(5, vector<vector<int>>(2, vector<int>(5, 3)));

int main()
{
	/*Node* root = NULL;

	root = root->CreateNode(1);

	root = root->InsertNode(2);
	root = root->InsertNode(3);
	root = root->InsertNode(4);
	root = root->InsertNode(5);
	root = root->InsertNode(6);
	root = root->InsertNode(7);
	root = root->InsertNode(8);
	root = root->InsertNode(9);
	root = root->InsertNode(10);
	root = root->InsertNode(11);
	root = root->InsertNode(12);

	
	------------------------------------------
		     1			//Root Node 
		  /    |
		 2     3					
		/ |   / |
	   4  5   6 7					
	 / | / |  |
    8 9 10 11 12		//Leaf Nodes 
	------------------------------------------

	root->DFS(root);
	cout << endl;

	root->BFS(root);
	cout << endl;
	*/

	vector<int> j = vector<int>{5, 89, 456, 987, 666, 547};

	cout << j[4] << endl;

	vector<vector<int>> p = vector<vector<int>>{ {5, 89}, {456, 987}, {666, 547} };

	cout << p[1][1] << endl;

	vector<int> j_ = vector<int>{ 999, 111 };

	j = j_;

	cout << j[0] << endl;

	A a;

	a.p();
	a.p();
	a.p();
	a.p();

	cout << endl;

	a.a = -5;

	a.p();
	a.p();
	a.p();
	a.p();

	cout << endl;

	A b(-5);

	b.p();
	b.p();
	b.p();
	b.p();

	cout << endl;

	b.set(6);

	cout << endl;

	a.p();

	cout << endl;

	//a.pv();

	a.set_v();

	return 0;
}