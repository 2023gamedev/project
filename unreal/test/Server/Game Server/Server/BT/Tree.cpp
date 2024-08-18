#include <iostream>
#include <queue>

using namespace std;

class Node {
public:
    int data;
    Node* left;
    Node* right;

	Node();

    Node(int val)
    {
        data = val;

        left = nullptr;
        right = nullptr;
    }
};

Node* CreateNode(int data)
{
	Node* newNode = new Node();
	if (!newNode) {
		cout << "Memory error\n";
		return nullptr;
	}
	newNode->data = data;
	newNode->left = newNode->right = nullptr;
	return newNode;
}

Node* InsertNode(Node* root, int data)
{
	if (root == nullptr) {
		root = CreateNode(data);
		return root;
	}

	queue<Node*> q;
	q.push(root);

	while (!q.empty()) {
		Node* temp = q.front();
		q.pop();

		if (temp->left != nullptr)
			q.push(temp->left);
		else {
			temp->left = CreateNode(data);
			return root;
		}

		if (temp->right != nullptr)
			q.push(temp->right);
		else {
			temp->right = CreateNode(data);
			return root;
		}
	}
}

void preorder(struct Node* node) {
	if (node == nullptr) return;
	cout << node->data << "->";
	preorder(node->left);
	preorder(node->right);
}

void inorder(struct Node* node) {
	if (node == nullptr) return;
	inorder(node->left);
	cout << node->data << "->";
	inorder(node->right);
}

void postorder(struct Node* node) {
	if (node == nullptr) return;
	postorder(node->left);
	postorder(node->right);
	cout << node->data << "->";
}


//int main()
//{
//	Node* root = CreateNode(1);
//	root = InsertNode(root, 2);
//	root = InsertNode(root, 3);
//	root = InsertNode(root, 4);
//	preorder(root);
//	cout << endl;
//	inorder(root);
//	cout << endl;
//	postorder(root);
//	cout << endl;
//	return 0;
//}