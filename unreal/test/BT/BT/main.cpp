#include "Tree.h"

int main()
{
	Node* root = NULL;

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

	/*
	------------------------------------------
		     1			//Root Node
		  /    |
		 2     3
		/ |   / |
	   4  5   6 7 
	 / | / |  |
    8 9 10 11 12		//Leaf Nodes
	------------------------------------------
	*/

	root->DFS(root);
	cout << endl;

	//root->BFS(root);
	cout << endl;
	
	return 0;
}