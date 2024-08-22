#include "Tree.h"
#include <iostream>

using namespace std;

class A {
public:
	static float a;
	static const float c;
	const float d = 5.f;
	const static float f;

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

	void set(int b) {
		a = b;

		cout << a << "->";
	}

	~A() {}
};

float A::a = 10.f;
const float A::c = 5.f;
const float A::f = 50.f;

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

	return 0;
}