#pragma once

#include <iostream>
#include <queue>

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

	void BFS(Node* node) {
		if (node == NULL) return;

		if (node->left != NULL) {
			cout << node->data << "->";
			cout << node->left->data << "->";
			cout << node->right->data << "->";
			BFS(node->left);
			BFS(node->right);
		}
	}
};
