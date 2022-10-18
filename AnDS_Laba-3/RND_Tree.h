#include <cstdlib> 
#include <time.h>
#include <cmath>
#include "Tree.h"

typedef unsigned long long INT_64;

using namespace std;

static int nn;

template <class K, class T>
class RND_Tree : public Tree<K, T>
{
public:
	bool AddElemTree(K, T); //���������� ������ �������� �� �����
	bool DelElemTree(K); //������� �������� �� �����
	//������� ��� ����������� ������ ����� �������� ��������
	Tree<K, T>::Node* joinLR(class Tree<K, T>::Node*, class Tree<K, T>::Node*);
	void fixN(class Tree<K, T>::Node*);
};

template<typename K, typename T>
bool RND_Tree<K, T>::AddElemTree(K key, T data) {
	class Tree<K, T>::Node* t = new class Tree<K, T>::Node(key, data), * temp = this->root, * subroot = nullptr;
	if (this->root == NULL) {//���� ������ �����
		this->root = t; //����� ���� ��� ������ ������
		this->size++; //����������� ����������� ������
		this->root->n++;
		return true; //�������� � �������� ���������� ��������
	}
	bool go_down = true, 
		AddElemTree_into_root = true, 
		loop = true; 
	while ((go_down) && (AddElemTree_into_root)) {
		if (rand() < RAND_MAX / (temp->n + 1)) { 
			subroot = temp;
			AddElemTree_into_root = false; 
		}
		else {
			if (t->key == temp->key) return false; //���� ����� ������ ��� ����������, ������� ��������� � ������
			if (t->key < temp->key) { //�������� �� ����� ����� ����
				this->count++;//����������� ������� ���������� �����
				if (temp->left == NULL) go_down = false;//���� �������� ����� �����, �� ���������� ����
				else temp = temp->left; //���� �� �����
			}
			if (t->key > temp->key) {//�������� �� ����� ����� ����
				this->count++; //����������� ������� ���������� �����
				if (temp->right == NULL) go_down = false; //���� �������� ����� �����, �� ���������� ����
				else temp = temp->right; //���� �� ������
			}
		}
	}
	while (1) { //������� � ����
		this->count++;//����������� ������� ���������� �����
		if (t->key == temp->key) return false;//���� ����� ������ ��� ����������, ������� ��������� � ������
		if (t->key > temp->key) //�������� ��������
			if (temp->right == NULL) {//�������� ����� �����
				temp->right = t;//��������� ������� � �����
				t->parent = temp; //���������� ��������
				this->size++; //����������� ������
				break;
			}
			else temp = temp->right; //�������� ������ �� ������ �����
		else {
			if (temp->left == NULL) {//�������� ����� �����
				temp->left = t;//��������� ������� � �����
				t->parent = temp;//���������� ��������
				this->size++;//����������� ������
				break;
			}
			else temp = temp->left; //�������� ������ �� ����� �����
		}
	}
	temp = t;
	while (temp != this->root) {//������������� ������� ����������� ����� ������� � ����
		temp = temp->parent;
		temp->n++;
	}
	if (AddElemTree_into_root == NULL) {//���� ������� ������� ��������� � ������ �� ���������� ��������
		loop = true;
		while (loop) {
			this->count++; //����������� ������� ���������� ���������
			temp = t->parent;
			if (t->parent == subroot) loop = false;
			if (t->key > temp->key) {//����� �������
				temp->right = t->left;
				if (t->left != NULL) t->left->parent = temp;
				t->left = temp;
			}
			else {//������ �������
				temp->left = t->right;
				if (t->right != NULL) t->right->parent = temp;
				t->right = temp;
			}
			if (temp->parent != NULL) {//������������� ������ ��������<->���
				if (temp->key > temp->parent->key) temp->parent->right = t;
				else temp->parent->left = t;
			}
			t->parent = temp->parent;
			temp->parent = t;
			if (temp == this->root) this->root = t;//���� ��� �������� ����� ������
			temp->n = 1;//������������� ������� ����������� ����� ��������
			if (temp->right != NULL) temp->n = temp->n + temp->right->n;
			if (temp->left != NULL) temp->n = temp->n + temp->left->n;
			t->n = 1;
			if (t->right != NULL) t->n = t->n + t->right->n;
			if (t->left != NULL) t->n = t->n + t->left->n;
		}
	}
	fixN(t);
	return true; //�������� � �������� ���������� ��������
}

template<typename K, typename T>
bool RND_Tree<K, T>::DelElemTree(K key) {
	this->count = 0;
	class Tree<K, T>::Node* prev, * h = this->root; //���� �������������� ����������
	while (1) {
		this->count++; //����������� ������� ���������� ���������
		if (h == 0) return false; //���� ������ ����� ���
		K w = h->key; //���������� ����
		if (key < w) h = h->left; //��������� �� ����� �����
		if (key > w) h = h->right; //��������� �� ������ �����
		if (key == w) {
			class Tree<K, T>::Node* t = h;
			if (h == this->root) { //���� ��������� � ����� ������
				this->root = h = joinLR(h->left, h->right);//������� ����� ������
				if (this->root) this->root->parent = NULL;
			}
			else {//�����
				prev = h->parent;
				if (prev->left == h) prev->left = h = joinLR(h->left, h->right);
				else prev->right = h = joinLR(h->left, h->right);
				if (h) h->parent = prev;
				if (!h) h = prev;
			}
			delete t; //������� ������
			this->size--; //��������� ������ ������
			//this->count = this->count + n; //���������� ���������� ���������
			break;
		}
	}
	while (h) { //���������� ������������
		fixN(h);
		h = h->parent;
	}
	return true;
}

template<typename K, typename T>
typename Tree<K, T>::Node* RND_Tree<K, T>::joinLR(class Tree<K, T>::Node* a, class Tree<K, T>::Node* b) {
	nn++;
	if (a == 0) return b;
	if (b == 0) return a;
	if ((rand() / (RAND_MAX / ((a->n + b->n) + 1))) < a->n) {
		a->right = joinLR(a->right, b);
		a->right->parent = a;
		fixN(a);
		return a;
	}
	else {
		b->left = joinLR(a, b->left);
		b->left->parent = b;
		fixN(b);
		return b;
	}
}

template<typename K, typename T>
void RND_Tree<K, T>::fixN(class Tree<K, T>::Node* t) { //���������� ������������
	if (t == NULL) return; //���� �������� ����� �����, �������
	if (t->left == 0 && t->right == NULL) { t->n = 1; return; }
	else {
		if (t->left == NULL) { t->n = t->right->n + 1; return; }
		if (t->right == NULL) { t->n = t->left->n + 1; return; }
	}
	t->n = t->left->n + t->right->n + 1;
}

//==========================================�����������===============================================

//���������� � ��������� ����������
static INT_64 RRand = 15750;
const INT_64 mRand = (1 << 63) - 1;
const INT_64 aRand = 6364136223846793005;
const INT_64 cRand = 1442695040888963407;

void sRand() { srand(time(0)); RRand = (INT_64)rand(); }

INT_64 LineRand()
{
	INT_64 y1, y2;
	y1 = (aRand * RRand + cRand) % mRand;
	y2 = (aRand * y1 + cRand) % mRand;
	RRand = y1 & 0xFFFFFFFF00000000LL ^ (y2 & 0xFFFFFFFF00000000LL) >> 32;
	return RRand;
}

void test_rand(int n) {
	//�������� ������ ��� 64 � ��������� ������ ���� INT_64
	Tree< INT_64, int > tree;
	RND_Tree< INT_64, int > tree1;
	//������ ��� ������, ������� ������������ � ������ 
	INT_64* m = new INT_64[n];
	//��������� ������� ���������� �����
	sRand();
	//���������� ������ � ������� ���������� �� ���������� �������
	for (int i = 0; i < n; i++)
	{
		m[i] = LineRand();
		tree.AddElemTree(m[i], 1);
		try {
			tree1.AddElemTree(m[i], 1);
		}
		catch (...) {}
	}
	//����� ������� ������ �� �����
	cout << "��� BST ������ (�� �����):" << endl;
	cout << "������ ������:" << n << endl;
	cout << "��� ����������������� ������ (�� �����):" << endl;
	cout << "������ ������:" << n << endl;
	//��������� ��������� ����������� �������, �������� � ������
	double I = 0;
	double D = 0;
	double S = 0;
	double I1 = 0;
	double D1 = 0;
	double S1 = 0;
	//��������� ������ ��������, 10% - ������� ��������
	for (int i = 0; i < n / 2; i++)
		if (i % 10 == 0)		//10% ��������
		{
			tree.DelElemTree(LineRand());
			D += tree.Count();
			tree.AddElemTree(m[rand() % n], 1);
			I += tree.Count();
			try {
				tree.GetElemTree(LineRand());
				S += tree.Count();
			}
			catch (...) { S += tree.Count(); }
			try {
				tree1.DelElemTree(LineRand());
				D1 += tree1.Count();
			}
			catch (...) { D1 += tree1.Count(); }
			try {
				tree1.AddElemTree(m[rand() % n], 1);
				I1 += tree1.Count();
			}
			catch (...) { I1 += tree1.Count(); }
			try {
				tree1.GetElemTree(LineRand());
				S1 += tree1.Count();
			}
			catch (...) { S1 += tree1.Count(); }
		}
		else  //90% �������� ��������
		{
			int ind = rand() % n;
			tree.DelElemTree(m[ind]);
			D += tree.Count();
			INT_64 key = LineRand();
			tree.AddElemTree(key, 1);
			I += tree.Count();
			m[ind] = key;
			try {
				tree.GetElemTree(m[rand() % n]);
				S += tree.Count();
			}
			//��������� ���������� ��� ������ �������� ������
			catch (...) { S += tree.Count(); }				//��������� ���������� ��� ������ �������� ������
															//��������� ���������� ��� ������ �������� ������

			int ind1 = rand() % n;
			try {
				tree1.DelElemTree(m[ind]);					//��������� ���������� ��� ������ �������� ������
				D1 += tree1.Count();
			}
			catch (...) {
				D1 += tree1.Count();						//��������� ���������� ��� ������ �������� ������
			}
			INT_64 key1 = LineRand();
			try {
				tree1.AddElemTree(key1, 1);
				I1 += tree1.Count();
			}
			catch (...) { I1 += tree1.Count(); }
			m[ind] = key1;
			try {
				tree1.GetElemTree(m[rand() % n]);
				S1 += tree1.Count();
			}
			//��������� ���������� ��� ������ �������� ������
			catch (...) { S1 += tree1.Count(); }
		}
	cout << "��� BST ������ (����� �����):" << endl;
	cout << "������ ������:" << n << endl;
	//������������� ������ ����������� �������� BST
	cout << "1.39*log2(n)=" << 1.39 * (log((double)n) / log(2.0)) << endl;
	//����������������� ������ ����������� �������
	cout << "���������� �������: " << I / (n / 2) << endl;
	//����������������� ������ ����������� ��������
	cout << "���������� ��������: " << D / (n / 2) << endl;
	//����������������� ������ ����������� ������
	cout << "���������� ������: " << S / (n / 2) << endl << endl;

	cout << "��� ������������������ ������ (����� �����):" << endl;
	cout << "������ ������:" << n << endl;
	//������������� ������ ����������� �������� BST
	cout << "log2(n) + 0.25 =" << (log((double)n) / log(2.0)) + 0.25 << endl;
	//����������������� ������ ����������� �������
	cout << "���������� �������: " << I1 / (n / 2) << endl;
	//����������������� ������ ����������� ��������
	cout << "���������� ��������: " << D1 / (n / 2) << endl;
	//����������������� ������ ����������� ������
	cout << "���������� ������: " << S1 / (n / 2) << endl << endl;
	//������������ ������ ������� m[]
	delete[] m;
}

void test_ord(int n) {
	//�������� ������ ��� 64 � ��������� ������ ���� INT_64
	Tree< INT_64, int > tree;
	RND_Tree< INT_64, int > tree1;
	//������ ��� ������, ������� ������������ � ������ 
	INT_64* m = new INT_64[n];

	//���������� ������ � ������� ���������� � ������������� ������� //������� �� ��������� [0, 10000, 20000, ... ,10000*n]
	for (int i = 0; i < n; i++) {
		m[i] = i * 10000;
		tree.AddElemTree(m[i], 1);
		tree1.AddElemTree(m[i], 1);
	}
	//����� ������� ������ �� �����
	cout << "��� BST ������ (�� �����):" << endl;
	cout << "������ ������:" << n << endl;
	cout << "��� ������������������ ������ (�� �����):" << endl;
	cout << "������ ������:" << n << endl;
	//��������� ��������� ����������� �������, �������� � ������
	double I = 0;
	double D = 0;
	double S = 0;
	double I1 = 0;
	double D1 = 0;
	double S1 = 0;
	//��������� ������� ���������� ����� 
	sRand();
	//��������� ������ ��������, 10% - ������� ��������
	for (int i = 0; i < n / 2; i++)
	{
		if (i % 10 == 0)		//10% ��������
		{
			int k = LineRand() % (10000 * n);
			k = k + !(k % 2);	//��������� �������� ����
			tree.DelElemTree(k);
			D += tree.Count();
			tree.AddElemTree(m[rand() % n], 1);
			I += tree.Count();
			k = LineRand() % (10000 * n);
			k = k + !(k % 2);	// ��������� �������� ����
			try {
				tree.GetElemTree(k);
				S += tree.Count();
			}
			//��������� ���������� ��� ������ �������� ������
			catch (...) { S += tree.Count(); }
			int k1 = LineRand() % (10000 * n);
			k1 = k1 + !(k1 % 2);	//��������� �������� ����
			try {
				tree1.DelElemTree(k1);
				D1 += tree1.Count();
			}
			catch (...) { D1 += tree1.Count(); }
			try {
				tree1.AddElemTree(m[rand() % n], 1);
				I1 += tree1.Count();
			}
			catch (...) { I1 += tree1.Count(); }
			k1 = LineRand() % (10000 * n);
			k1 = k1 + !(k1 % 2);	// ��������� �������� ����
			try {
				tree1.GetElemTree(k1);
				S1 += tree1.Count();
			}
			//��������� ���������� ��� ������ �������� ������
			catch (...) {
				S1 += tree1.Count();
			}
		}
		else  //90% �������� ��������
		{
			int ind = rand() % n;
			tree.DelElemTree(m[ind]);
			D += tree.Count();
			int k = LineRand() % (10000 * n);
			k = k + k % 2;		// ��������� ������ ����
			tree.AddElemTree(k, 1);
			I += tree.Count();;
			m[ind] = k;
			try {
				tree.GetElemTree(m[rand() % n]);
				S += tree.Count();
			}
			//��������� ���������� ��� ������ �������� ������
			catch (...) { S += tree.Count(); }
			int ind1 = rand() % n;
			try {
				tree1.DelElemTree(m[ind1]);
				D1 += tree1.Count();
			}
			catch (...) { D1 += tree1.Count(); }
			int k1 = LineRand() % (10000 * n);
			k1 = k1 + k1 % 2;		// ��������� ������ ����
			try {
				tree1.AddElemTree(k1, 1);
				I1 += tree1.Count();
			}
			catch (...) { I1 += tree1.Count(); }
			m[ind1] = k1;
			try {
				tree1.GetElemTree(m[rand() % n]);
				S1 += tree1.Count();
			}
			//��������� ���������� ��� ������ �������� ������
			catch (...) { S1 += tree1.Count(); }
		}
	}

	//����� �����������:
	// ����� ������� ������ ����� �����
	cout << endl << endl;
	cout << "��� BST ������ (����� �����):" << endl;
	cout << "������ ������:" << n << endl;
	//������������� ������ ����������� �������� BST
	cout << "n/2 =" << n / 2 << endl;
	//����������������� ������ ����������� �������
	cout << "���������� �������: " << I / (n / 2) << endl;
	//����������������� ������ ����������� ��������
	cout << "���������� ��������: " << D / (n / 2) << endl;
	//����������������� ������ ����������� ������
	cout << "���������� ������: " << S / (n / 2) << endl << endl << endl;
	cout << "��� ������������������ ������ (����� �����):" << endl;
	// ����� ������� ������ ����� �����
	cout << "������ ������:" << n << endl;
	//������������� ������ ����������� �������� BST
	cout << "log2(n) + 0.25 =" << (log((double)n) / log(2.0)) + 0.25 << endl;
	//����������������� ������ ����������� �������
	cout << "���������� �������: " << I1 / (n / 2) << endl;
	//����������������� ������ ����������� ��������
	cout << "���������� ��������: " << D1 / (n / 2) << endl;
	//����������������� ������ ����������� ������
	cout << "���������� ������: " << S1 / (n / 2) << endl;
	//������������ ������ ������� m[]
	delete[] m;
}	//����� �����