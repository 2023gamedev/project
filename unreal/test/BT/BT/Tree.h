#pragma once

#include <iostream>
#include <queue>
#include <cmath>

using namespace std;

class Node {
public:
    int data = 0;
    Node* left = NULL;
    Node* right = NULL;

	Node()
	{
		data = 0;
		left = right = NULL;
	}

    Node(int val)
    {
        data = val;
        left = right = NULL;
    }

	Node* CreateNode(int data)
	{
		Node* newNode = new Node();
		if (!newNode) {
			cout << "Memory error\n";
			return NULL;
		}
		newNode->data = data;
		newNode->left = newNode->right = NULL;
		return newNode;
	}

	Node* InsertNode(int data)
	{
		Node* root = this;

		if (root == NULL) {
			root = CreateNode(data);
			return root;
		}

		queue<Node*> q;
		q.push(root);

		while (!q.empty()) {
			Node* temp = q.front();
			q.pop();

			if (temp->left != NULL)
				q.push(temp->left);
			else {
				temp->left = CreateNode(data);
				return root;
			}

			if (temp->right != NULL)
				q.push(temp->right);
			else {
				temp->right = CreateNode(data);
				return root;
			}
		}
	}

	void DFS(Node* node) {
		if (node == NULL) return;

		cout << node->data << "->";

		DFS(node->left);
		DFS(node->right);
	}

	void MakeBFSQue(Node* node, int level, queue<pair<int, int>>* bfs) {
		if (node == NULL) return;

		bfs->push(pair<int, int>(node->data, level));

		MakeBFSQue(node->left, level + 1, bfs);
		MakeBFSQue(node->right, level + 1, bfs);
	}

	void BFS(Node* node) {
		if (node == NULL) return;

		queue<pair<int, int>> bfs;

		MakeBFSQue(node, 0, &bfs);
		
		queue<pair<int, int>> bypass;

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

	}
};
