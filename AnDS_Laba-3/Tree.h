#include <iostream>
#include <stack>
#include <time.h>

#define Ex "Exception!"

using namespace std;

template <typename Key, typename Data>
class Tree {
protected:
	class Node
	{
	public:
		Key key; // ���� ��������
		Data data; // �������� ��������
		Node* left;	// ��������� �� ����� �����
		Node* right;  // �������� �� ������ �����
		Node* parent;  //��������� ��������
		int n; //�������� ������������
		Node(Key k = NULL, Data d = Data()) {//�����������
			this->data = d;
			this->key = k;
			this->left = NULL;
			this->right = NULL;
			this->parent = NULL;
			this->n = 0;
		}
	};
	Node* root; //�������� ������� ������
	int size; //������ ������
	int count; //���������� ���������� �����������
public:
	class Iterator;
	class reIterator;
	Tree(); // �����������
	Tree(const Tree<Key, Data>& obj); // ����������� �����������
	void copy(Node*); //������� �����������
	~Tree(); // ����������
	int GetSizeTree(); // ����� ������� ������
	bool ClearTree(); // ������� ������
	bool EmptyTree(); //�������� ������ �� �������
	Data& GetElemTree(Key); //������ ������ �� �����
	bool EditElemTree(Key, Data); //������ ������ �� �����
	bool AddElemTree(Key, Data); //��������� ������ � �������� ������
	bool DelElemTree(Key); //�������� ������ � �������� ������
	void PrintKey(); //����� ������ ������
	int SearchNumKey(int); //����� ���������� ����� ����� ������� ������ ��������� ��������
	void PrintStructureTree(int); //����� ��������� ������
	void PrintStrKey(Node*, int, int); //����� ��������� �����������
	int Count();// ����� ���������� ���������� �����
	Node* SearchPredElem(Node*); //���� ���� � ���������� �� �������� ������
	Node* SearchNextElem(Node*); //���� ���� �� ��������� �� �������� ������
	Node* SearchMaxKey(Node*);	// ���� ���� � ������������ ������
	Node* SearchRightParent(Node*, Node*);// ����� ���������� ������� ��������
	Node* SearchMinKey(Node*);// ���� ���� � ����������� ������
	Node* SearchLeftParent(Node*, Node*);// ����� ���������� ������ ��������

	class Iterator { //������ ��������
		Tree* ptr;
		Node* cur;
	public:
		Iterator(Tree<Key, Data>& tr) { //�����������
			ptr = &tr;
			cur = NULL;
		}
		Key& begin() { //��������� ������� ��������� �� ������ �������
			if (ptr->root == NULL) throw Ex;
			cur = ptr->root;
			while (cur != NULL && cur->left != NULL) //���������� �� ���� ������ ������
				cur = cur->left;
			return cur->key;
		}
		Key& end() { //��������� ������� ��������� �� ������ �������������� �������
			if (ptr->root == NULL) throw Ex;
			cur = ptr->root;
			while (cur != NULL && cur->right != NULL) //���������� �� ���� ������� ������
				cur = cur->right;
			Node* temp = cur;
			cur = NULL;
			return temp->key;
		}
		Data& operator*() { //������������� ���������
			if (ptr->root == NULL || cur == NULL) throw Ex;

			return cur->data; //���������� ������� �������
		}
		Key& operator++(int) { //�������� �������� � ����������
			if (ptr->root == NULL || cur == NULL) throw Ex;
			cur = ptr->SearchNextElem(cur); //��������� �� ������ ������
		}
		Key& operator--(int) { //�������� �������� � �����������
			if (ptr->root == NULL || cur == NULL) throw Ex;
			cur = ptr->SearchPredElem(cur);//��������� �� ������ �����
		}
		bool operator==(Iterator& it) { //�������� �� ���������
			if (ptr->root == NULL || cur == NULL) throw Ex;
			return cur == Iterator.cur;
		}
		bool operator!=(Iterator& it) { //�������� �� �����������
			if (ptr->root == NULL || cur == NULL) throw Ex;
			return cur != Iterator.cur;
		}
	};

	class reIterator {//������� ��������
		Tree* ptr;
		Node* cur;
	public:
		reIterator(Tree<Key, Data>& rtr) {//�����������
			ptr = &rtr;
			cur = NULL;
		}
		Key& rbegin() { //��������� ��������� �������� � ������
			if (ptr->root == NULL) throw Ex;
			cur = ptr->root;
			while (cur != NULL && cur->right != NULL)
				cur = cur->right;
			return cur->key;
		}
		Key& rend() { //��������� ��������� ��������� �� ������ ��������������� �������
			if (ptr->root == NULL) throw Ex;
			cur = ptr->root;
			while (cur != NULL && cur->left != NULL)
				cur = cur->left;
			Node* temp = cur;
			cur = NULL;
			return temp->key;
		}
		Data& operator*() { //�������� �������������
			if (ptr->root == NULL || cur == NULL) throw Ex;
			return cur->data; //���������� ������� ��������
		}
		Key& operator++(int) { //�������� �������� � ����������
			if (ptr->root == NULL || cur == NULL) throw Ex;
			cur = ptr->SearchPredElem(cur); //��������� �� ������ ������
		}
		Key& operator--(int) { //�������� �������� � �����������
			if (ptr->root == NULL || cur == NULL) throw Ex;
			cur = ptr->SearchNextElem(cur);//��������� �� ������ �����
		}
		bool operator==(reIterator& rit) {//�������� �� ���������
			if (ptr->root == NULL || cur == NULL) throw Ex;
			return cur == reIterator.cur;
		}
		bool operator!=(reIterator& rit) {//�������� �� �����������
			if (ptr->root == NULL || cur == NULL) throw Ex;
			return cur != reIterator.cur;
		}
	};
};

template<typename Key, typename Data>
bool Tree<Key, Data>::AddElemTree(Key key, Data data) { //�������� ����� ������ �� �����
	count = 0;  //�������� ������� ���������� ���������
	if (root == NULL) { //���� ������ �����
		root = new Node(key, data);  //���������� ����� ������� ��� ������
		size++; //����������� �����������
		return true;  //�������� �� �������� ���������� ��������
	}
	Node* tmp = root, * pred = NULL; //������� ��������� ��������
	while (tmp != NULL) {//���� �� ������� ��� ������ 
		pred = tmp; //���������� ���������� �������
		count++;   //������� ��� ���� ��� �� ������
		if (key == (tmp->key))  //���� � ������ ���� ������ � ����� ������
			return false; //���������� ��������� � ������
		if (key < (tmp->key)) //�������� �� ����� ����� ����
			tmp = tmp->left; //���������� �� ����� �����
		else
			tmp = tmp->right; //���������� �� ������ ����� 
	}
	if (key < (pred->key)) //�������� �� ����� ����� ����
		pred->left = new Node(key, data); //��������� �������� � ����� ����� �����
	else
		pred->right = new Node(key, data);  //��������� �������� � ����� ������ �����

	size++; //����������� �����������
	count++; //������� ��� ���� ��� �� ������
	return true;  //�������� �� �������� ���������� ��������
}

template<typename Key, typename Data>
bool Tree<Key, Data>::EditElemTree(Key key, Data data) { //��������� ������ �� �����
	count = 0;  //�������� ������� ���������� ���������
	if (root == NULL) //���� ������ �����
		return false;  //�������� �� ������
	Node* tmp = root;  //������� ��������� ��������	
	while (tmp != NULL) {//���� �� ������� ��� ������ 
		count++;   //������� ��� ���� ��� �� ������
		if (key == (tmp->key)) {  //���� � ������ ���� ������ � ����� ������
			tmp->data = data; //�������� ��������
			return true; //�������� �� �������� ���������� ��������
		}
		if (key < (tmp->key)) //�������� �� ����� ����� ����
			tmp = tmp->left; //���������� �� ����� �����
		else
			tmp = tmp->right; //���������� �� ������ ����� 
	}
	return false; //���� ���� ��� � �� ������, ���������� ������
}

template<typename Key, typename Data>
Data& Tree<Key, Data>::GetElemTree(Key key) { //�������� �������� �� �����
	if (root == NULL) throw Ex; //���� ����� �����, ������� ����������
	count = 0;  //�������� ������� ���������� ���������
	Node* tmp = root;//������� ��������� ��������	
	while (tmp != NULL && key != (tmp->key)) {
		if (key < (tmp->key)) //�������� �� ����� ����� ����
			tmp = tmp->left; //���������� �� ����� �����
		else
			tmp = tmp->right; //���������� �� ������ ����� 
		count++;   //������� ��� ���� ��� �� ������
	}
	if (tmp == NULL) throw Ex; //���� �� �������� ������ � �������� ������, �� ������� ����������
	return tmp->data; //���������� ������
}

template<typename Key, typename Data>
bool Tree<Key, Data>::DelElemTree(Key key) { //������� ������ �� �����
	if (root == NULL) //���� ������ �����
		return false;  //�������� �� ������
	Node* tmp = root, * pred = NULL, * next = NULL, * tmp_0 = NULL; //������� ��������� ��������	
	count = 0; //�������� ������� ���������� ���������
	while (tmp != NULL && tmp->key != key) { //���� �� ������� ��� ������ � ������� ���� �� ����� �������
		pred = tmp;//���������� ���������� �������
		if (key < tmp->key)//�������� �� ����� ����� ����
			tmp = tmp->left;//���������� �� ����� �����
		else
			tmp = tmp->right;//���������� �� ������ �����
		count++;//������� ��� ���� ��� �� ������
	}
	if (tmp == NULL) //���� ������ �������� � ������ ���
		return false; //�������� �� ������
	if (tmp->left != NULL && tmp->right != NULL) { //���� �������� �� ��������
		tmp_0 = tmp;
		pred = tmp;
		tmp = tmp->right; //���� �� ������ �����
		while (tmp->left != NULL) { //���� �� �������� ����� ����� �����
			count++; //������� ��� ���� ��� �� ������
			pred = tmp; //��������� ���������� �������
			tmp = tmp->left; //������ ����� �� ����� �����
		}
		//���������� � ��������� ����������
		tmp_0->key = tmp->key;
		tmp_0->data = tmp->data;
		next = tmp->right;
	}
	else { //������� �� �����
		if (tmp->left == NULL && tmp->right == NULL) //������� ��������� ������� �����
			next = NULL;
		else {
			if (tmp->left == NULL)
				next = tmp->right; //��������� ��������� ������� ���������� �������� ������ �����
			else if (tmp->right == NULL)
				next = tmp->left; //��������� ��������� ������� ���������� �������� ����� �����
		}
	}
	if (pred == NULL)//���� �������� ���� �������
		root = next; //��������� ������� ����������� ��������� ����������
	else
		if (tmp->key < pred->key)//�������� �� ����� ����� ����
			pred->left = next; //��������� �������� � ����� �����
		else
			pred->right = next; //��������� �������� � ������ �����
	delete tmp; //������� ������
	size--; //��������� �����������
	return true;  //�������� �� �������� ���������� ��������
}

template<typename Key, typename Data>
int Tree<Key, Data>::GetSizeTree() { //������ �����������
	return size; //���������� �����������
}

template<typename Key, typename Data>
bool Tree<Key, Data>::EmptyTree() { //�������� �� �������
	if (size)
		return false; //0 ���� ������ �����
	return true; //1 ���� ���� ��������
}

template<typename Key, typename Data>
void Tree<Key, Data>::PrintKey() { //�������� ������ ������
	if (root == NULL) throw Ex; //���� ����� �����, ������� ����������
	stack<Node*> stack1; //������� ����
	Node* tmp; //������� ��������� ��������
	stack1.push(root); //�������� �������� ������� � ����
	while (!stack1.empty()) { //���� ���� �� ����
		tmp = stack1.top(); //������� � ���������� �������
		cout << tmp->key << " "; //������� ��� ��������
		stack1.pop(); //������� ������� �������
		if (tmp->right != NULL) //���� ����� �� �����������
			stack1.push(tmp->right); //��������� ��������� ������� �� ������ ����� � ����
		if (tmp->left != NULL) //���� ����� �� �����������
			stack1.push(tmp->left); //��������� ��������� ������� �� ����� ����� � ����
	}
}

template<typename Key, typename Data>
bool Tree<Key, Data>::ClearTree() { //������� ������
	if (root == NULL) //���� ������ �����
		return false;  //�������� �� ������
	stack<Node*> stack1, stack2; //������� �����
	Node* tmp; //������� ��������� ��������
	stack1.push(root); //�������� �������� ������� � ����
	while (!stack1.empty()) {//���� ���� �� ����
		tmp = stack1.top();//������� � ���������� �������
		stack1.pop();//������� ������� �������
		stack2.push(tmp);//��������� ��������� ������� tmp � ����
		if (tmp->left != NULL)//���� ����� �� �����������
			stack1.push(tmp->left);//��������� ��������� ������� �� ����� ����� � ����
		if (tmp->right != NULL)//���� ����� �� �����������
			stack1.push(tmp->right); //��������� ��������� ������� �� ������ ����� � ����
		while (!stack2.empty()) {//���� ���� �� ����
			tmp = stack2.top();//������� � ���������� �������
			stack2.pop();//������� ������� �������
			delete tmp;//������� ������
			size--; //��������� �����������
		}
	}
	root = NULL; //������ ����� � ����� ������
	return true;//�������� �� �������� ���������� ��������
}

template<typename Key, typename Data>
void Tree<Key, Data>::PrintStructureTree(int num) { //����� ��������� ������
	if (root == NULL) throw Ex; //���� ����� �����, ������� ����������
	PrintStrKey(root, 0, num); //�������� ����������� ������� ������ ������
}

template<typename Key, typename Data>
inline void Tree<Key, Data>::PrintStrKey(Node* r, int level, int num) { //������� ������ ������ �����������
	if (r == NULL) return; //���� ������ ������, ������������
	PrintStrKey(r->right, level + num, num); //�������� �� ������ �����
	if (r->key == root->key) cout << endl;
	for (int i = 0; i < 3 * level; i++) cout << " ";
	if (num == 4)
		cout << r->key << " (" << r->n << ")" << endl;
	if (num == 3)
		cout << r->key << "[" << r->data << "] (" << r->n << ")" << endl;
	if (num == 2)
		cout << r->key << endl;
	if (num == 1)
		cout << r->key << "[" << r->data << "]" << endl; //������� ����� � ����������
	if (r->key == root->key) cout << endl;
	PrintStrKey(r->left, level + num, num); //�������� �� ����� �����
}

template<typename Key, typename Data>
int Tree<Key, Data>::SearchNumKey(int num) {//����� ���������� ����� � ������� ������ ��������� �����
	if (root == NULL) throw Ex; //���� ����� �����, ������� ����������
	count = 0; //�������� ������� ���������� ���������
	stack<Node*> stack1;//������� ����
	Node* tmp;//������� ��������� ��������
	stack1.push(root);//�������� �������� ������� � ����
	while (!stack1.empty()) {//���� ���� �� ����
		tmp = stack1.top();//������� � ���������� �������
		if (tmp->key > num) count++; //���� ������ ���� ������������� �������, �� ����������� �� 1 �������
		stack1.pop();//������� ������� �������
		if (tmp->right != NULL)//���� ����� �� �����������
			stack1.push(tmp->right);//��������� ��������� ������� �� ������ ����� � ����
		if (tmp->left != NULL)//���� ����� �� �����������
			stack1.push(tmp->left);//��������� ��������� ������� �� ����� ����� � ����
	}
	return count; //���������� ���������� ���������� �� ������� ������
}

template <class K, class tmp>
int Tree<K, tmp>::Count() { //���������� ���������� ���������
	int temp = count; //����������
	count = 0; //��������
	return temp; //���������� ���������� ��������� ���������
}

template<typename Key, typename Data>
Tree<Key, Data>::Tree() { //�����������
	root = NULL; //������ ��������� �� ���������
	count = 0;
}

template<typename Key, typename Data>
Tree<Key, Data>::Tree(const Tree<Key, Data>& obj) { //����������� �����������
	root = NULL;
	size = 0;
	copy(obj.root);
}

template<typename Key, typename Data>
void Tree<Key, Data>::copy(typename Tree<Key, Data>::Node* r) { //������� ����������� 
	if (r == NULL) //���� ����������� �����
		return;
	AddElemTree(r->key, r->data); //��������� �������
	copy(r->left); //��������� ����� �����
	copy(r->right); //��������� ������ �����
}

template<typename Key, typename Data>
Tree<Key, Data>::~Tree() { //����������
	ClearTree(); //�������� ������� ������� ������
}

template<typename Key, typename Data>
typename Tree<Key, Data>::Node* Tree<Key, Data>::SearchPredElem(Node* x) {//���� ���� � ���������� �� �������� ������
	if (x->left != NULL) return SearchMaxKey(x->left); //���� ��������� ������� �� �������� ��������, �� ���������� ������� �������� ����������� ������
	else return SearchRightParent(root, x); //����� ���� ������� ��������
}

template<typename Key, typename Data>
typename Tree<Key, Data>::Node* Tree<Key, Data>::SearchNextElem(Node* x) {//���� ���� � ��������� �� �������� ������
	if (x->right != NULL) return SearchMinKey(x->right); //���� ��������� ������� �� �������� ��������, �� ���������� ������� �������� ����������� ������
	else return SearchLeftParent(root, x);//����� ���� ������ ��������
}

template<typename Key, typename Data>
typename Tree<Key, Data>::Node* Tree<Key, Data>::SearchMaxKey(Node* tmp) { //����� ������������ �����
	if (tmp == NULL) return NULL; //���� ����������� ������
	while (tmp->right != NULL) tmp = tmp->right; //�������� �� ������ �����
	return tmp; //���������� ������������ ����
}

template<typename Key, typename Data>
typename Tree<Key, Data>::Node* Tree<Key, Data>::SearchMinKey(Node* tmp) { //����� ������������ �����
	if (tmp == NULL) return NULL; //���� ������ �����������
	while (tmp->left != NULL) tmp = tmp->left; //�������� �� ����� �����
	return tmp; //���������� ����������� ����
}

template<typename Key, typename Data>
typename Tree<Key, Data>::Node* Tree<Key, Data>::SearchRightParent(Node* tmp, Node* x) {//����� ���������� ������� ��������
	if (tmp == x) return NULL; //���� ������ �����������
	if (x->key > tmp->key) { //���������� �����
		Node* rp = SearchRightParent(tmp->right, x); //���� ������� ��������
		if (rp != NULL) return rp; //���� ��������������, �� ���������� ���
		else return tmp;
	}
	else return SearchRightParent(tmp->left, x);
}

template<typename Key, typename Data>
typename Tree<Key, Data>::Node* Tree<Key, Data>::SearchLeftParent(Node* tmp, Node* x) {//����� ���������� ������ ��������
	if (tmp == x) return NULL;//���� ������ �����������
	if (x->key < tmp->key) {//���������� �����
		Node* lp = SearchLeftParent(tmp->left, x); //���� ������ ��������
		if (lp != NULL) return lp;//���� ��������������, �� ���������� ���
		else return tmp;
	}
	else return SearchLeftParent(tmp->right, x);
}

