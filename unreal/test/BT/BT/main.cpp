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


	queue<pair<int, int>> bfs;
	queue<pair<int, int>> bypass;

	root->BFS(root, 0, &bfs);

	int cnt = 0;
	int currentLevel = 0;
	while (bfs.size() != 0) {
		if (currentLevel == bfs.front().second) {
			cout << bfs.front().first << "->";
			bfs.pop();
			++cnt;

			if (pow(2, currentLevel) <= cnt) {
				++currentLevel;
				cnt = 0;
			}

			queue<pair<int, int>> tmp;
			while (bypass.size() != 0) {
				tmp.push(bypass.front());
				bypass.pop();
			}
			while (bfs.size() != 0) {
				tmp.push(bfs.front());
				bfs.pop();
			}
			bfs = tmp;
		}
		else {
			bypass.push(bfs.front());
			bfs.pop();
		}
	}
	cout << endl;
	
	return 0;
}