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
	bool AddElemTree(K, T); //добавление нового элемента по ключу
	bool DelElemTree(K); //удалить значение по ключу
	//Функции для перестройки дерева после удаления элемента
	Tree<K, T>::Node* joinLR(class Tree<K, T>::Node*, class Tree<K, T>::Node*);
	void fixN(class Tree<K, T>::Node*);
};

template<typename K, typename T>
bool RND_Tree<K, T>::AddElemTree(K key, T data) {
	class Tree<K, T>::Node* t = new class Tree<K, T>::Node(key, data), * temp = this->root, * subroot = nullptr;
	if (this->root == NULL) {//если дерево пусто
		this->root = t; //новый ключ как корень дерева
		this->size++; //увеличиваем размерность дерева
		this->root->n++;
		return true; //сообщаем о успешном выполнении операции
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
			if (t->key == temp->key) return false; //если такой корень уже существует, выводим сообщение о ошибке
			if (t->key < temp->key) { //выбираем по какой ветке идти
				this->count++;//увеличиваем счетчик пройденных шагов
				if (temp->left == NULL) go_down = false;//если достигли конца ветки, то сбрасываем ключ
				else temp = temp->left; //идем по левой
			}
			if (t->key > temp->key) {//выбираем по какой ветке идти
				this->count++; //увеличиваем счетчик пройденных шагов
				if (temp->right == NULL) go_down = false; //если достигли конца ветки, то сбрасываем ключ
				else temp = temp->right; //идем по правой
			}
		}
	}
	while (1) { //вставка в лист
		this->count++;//увеличиваем счетчик пройденных шагов
		if (t->key == temp->key) return false;//если такой корень уже существует, выводим сообщение о ошибке
		if (t->key > temp->key) //выбераем подветку
			if (temp->right == NULL) {//достигли конца ветки
				temp->right = t;//добавляем элемент в конец
				t->parent = temp; //запоминаем родителя
				this->size++; //увеличиваем размер
				break;
			}
			else temp = temp->right; //проходим дальше по правой ветке
		else {
			if (temp->left == NULL) {//достигли конца ветки
				temp->left = t;//добавляем элемент в конец
				t->parent = temp;//запоминаем родителя
				this->size++;//увеличиваем размер
				break;
			}
			else temp = temp->left; //проходим дальше по левой ветке
		}
	}
	temp = t;
	while (temp != this->root) {//корректировка размера поддеревьев после вставки в лист
		temp = temp->parent;
		temp->n++;
	}
	if (AddElemTree_into_root == NULL) {//если принято решение вставлять в корень то производим вращения
		loop = true;
		while (loop) {
			this->count++; //увеличиваем счетчик пройденных элементов
			temp = t->parent;
			if (t->parent == subroot) loop = false;
			if (t->key > temp->key) {//левый поворот
				temp->right = t->left;
				if (t->left != NULL) t->left->parent = temp;
				t->left = temp;
			}
			else {//правый поворот
				temp->left = t->right;
				if (t->right != NULL) t->right->parent = temp;
				t->right = temp;
			}
			if (temp->parent != NULL) {//корректировка связей родитель<->сын
				if (temp->key > temp->parent->key) temp->parent->right = t;
				else temp->parent->left = t;
			}
			t->parent = temp->parent;
			temp->parent = t;
			if (temp == this->root) this->root = t;//если при повороте нашли корень
			temp->n = 1;//корректировка размера поддеревьев после вращения
			if (temp->right != NULL) temp->n = temp->n + temp->right->n;
			if (temp->left != NULL) temp->n = temp->n + temp->left->n;
			t->n = 1;
			if (t->right != NULL) t->n = t->n + t->right->n;
			if (t->left != NULL) t->n = t->n + t->left->n;
		}
	}
	fixN(t);
	return true; //сообщаем о успешном выполнении операции
}

template<typename K, typename T>
bool RND_Tree<K, T>::DelElemTree(K key) {
	this->count = 0;
	class Tree<K, T>::Node* prev, * h = this->root; //ввод дополнительных переменных
	while (1) {
		this->count++; //увеличиваем счетчик пройденных элементов
		if (h == 0) return false; //если такого ключа нет
		K w = h->key; //запоминаем ключ
		if (key < w) h = h->left; //переходим по левой ветви
		if (key > w) h = h->right; //переходим по правой ветви
		if (key == w) {
			class Tree<K, T>::Node* t = h;
			if (h == this->root) { //если находимся в корне дерева
				this->root = h = joinLR(h->left, h->right);//находим новый корень
				if (this->root) this->root->parent = NULL;
			}
			else {//иначе
				prev = h->parent;
				if (prev->left == h) prev->left = h = joinLR(h->left, h->right);
				else prev->right = h = joinLR(h->left, h->right);
				if (h) h->parent = prev;
				if (!h) h = prev;
			}
			delete t; //очищаем данные
			this->size--; //уменьшаем размер дерева
			//this->count = this->count + n; //количество пройденных элементов
			break;
		}
	}
	while (h) { //перерасчет балансировки
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
void RND_Tree<K, T>::fixN(class Tree<K, T>::Node* t) { //перерасчет балансировки
	if (t == NULL) return; //если достигли конца метви, выходим
	if (t->left == 0 && t->right == NULL) { t->n = 1; return; }
	else {
		if (t->left == NULL) { t->n = t->right->n + 1; return; }
		if (t->right == NULL) { t->n = t->left->n + 1; return; }
	}
	t->n = t->left->n + t->right->n + 1;
}

//==========================================Тестировщик===============================================

//переменная и константы генератора
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
	//создание дерева для 64 – разрядных ключей типа INT_64
	Tree< INT_64, int > tree;
	RND_Tree< INT_64, int > tree1;
	//массив для ключей, которые присутствуют в дереве 
	INT_64* m = new INT_64[n];
	//установка первого случайного числа
	sRand();
	//заполнение дерева и массива элементами со случайными ключами
	for (int i = 0; i < n; i++)
	{
		m[i] = LineRand();
		tree.AddElemTree(m[i], 1);
		try {
			tree1.AddElemTree(m[i], 1);
		}
		catch (...) {}
	}
	//вывод размера дерева до теста
	cout << "Для BST дерева (до теста):" << endl;
	cout << "Размер дерева:" << n << endl;
	cout << "Для Рандомизировнного дерева (до теста):" << endl;
	cout << "Размер дерева:" << n << endl;
	//обнуление счётчиков трудоёмкости вставки, удаления и поиска
	double I = 0;
	double D = 0;
	double S = 0;
	double I1 = 0;
	double D1 = 0;
	double S1 = 0;
	//генерация потока операций, 10% - промахи операций
	for (int i = 0; i < n / 2; i++)
		if (i % 10 == 0)		//10% промахов
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
		else  //90% успешных операций
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
			//обработка исключения при ошибке операции поиска
			catch (...) { S += tree.Count(); }				//обработка исключения при ошибке операции поиска
															//обработка исключения при ошибке операции поиска

			int ind1 = rand() % n;
			try {
				tree1.DelElemTree(m[ind]);					//обработка исключения при ошибке операции поиска
				D1 += tree1.Count();
			}
			catch (...) {
				D1 += tree1.Count();						//обработка исключения при ошибке операции поиска
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
			//обработка исключения при ошибке операции поиска
			catch (...) { S1 += tree1.Count(); }
		}
	cout << "Для BST дерева (после теста):" << endl;
	cout << "Размер дерева:" << n << endl;
	//теоретической оценки трудоёмкости операций BST
	cout << "1.39*log2(n)=" << 1.39 * (log((double)n) / log(2.0)) << endl;
	//экспериментальной оценки трудоёмкости вставки
	cout << "Количество вставок: " << I / (n / 2) << endl;
	//экспериментальной оценки трудоёмкости удаления
	cout << "Количество удалений: " << D / (n / 2) << endl;
	//экспериментальной оценки трудоёмкости поиска
	cout << "Количество поиска: " << S / (n / 2) << endl << endl;

	cout << "Для рандомизированного дерева (после теста):" << endl;
	cout << "Размер дерева:" << n << endl;
	//теоретической оценки трудоёмкости операций BST
	cout << "log2(n) + 0.25 =" << (log((double)n) / log(2.0)) + 0.25 << endl;
	//экспериментальной оценки трудоёмкости вставки
	cout << "Количество вставок: " << I1 / (n / 2) << endl;
	//экспериментальной оценки трудоёмкости удаления
	cout << "Количество удалений: " << D1 / (n / 2) << endl;
	//экспериментальной оценки трудоёмкости поиска
	cout << "Количество поиска: " << S1 / (n / 2) << endl << endl;
	//освобождение памяти массива m[]
	delete[] m;
}

void test_ord(int n) {
	//создание дерева для 64 – разрядных ключей типа INT_64
	Tree< INT_64, int > tree;
	RND_Tree< INT_64, int > tree1;
	//массив для ключей, которые присутствуют в дереве 
	INT_64* m = new INT_64[n];

	//заполнение дерева и массива элементами с возрастающими чётными //ключами на интервале [0, 10000, 20000, ... ,10000*n]
	for (int i = 0; i < n; i++) {
		m[i] = i * 10000;
		tree.AddElemTree(m[i], 1);
		tree1.AddElemTree(m[i], 1);
	}
	//вывод размера дерева до теста
	cout << "Для BST дерева (до теста):" << endl;
	cout << "Размер дерева:" << n << endl;
	cout << "Для рандомизированного дерева (до теста):" << endl;
	cout << "Размер дерева:" << n << endl;
	//обнуление счётчиков трудоёмкости вставки, удаления и поиска
	double I = 0;
	double D = 0;
	double S = 0;
	double I1 = 0;
	double D1 = 0;
	double S1 = 0;
	//установка первого случайного числа 
	sRand();
	//генерация потока операций, 10% - промахи операций
	for (int i = 0; i < n / 2; i++)
	{
		if (i % 10 == 0)		//10% промахов
		{
			int k = LineRand() % (10000 * n);
			k = k + !(k % 2);	//случайный нечётный ключ
			tree.DelElemTree(k);
			D += tree.Count();
			tree.AddElemTree(m[rand() % n], 1);
			I += tree.Count();
			k = LineRand() % (10000 * n);
			k = k + !(k % 2);	// случайный нечётный ключ
			try {
				tree.GetElemTree(k);
				S += tree.Count();
			}
			//обработка исключения при ошибке операции поиска
			catch (...) { S += tree.Count(); }
			int k1 = LineRand() % (10000 * n);
			k1 = k1 + !(k1 % 2);	//случайный нечётный ключ
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
			k1 = k1 + !(k1 % 2);	// случайный нечётный ключ
			try {
				tree1.GetElemTree(k1);
				S1 += tree1.Count();
			}
			//обработка исключения при ошибке операции поиска
			catch (...) {
				S1 += tree1.Count();
			}
		}
		else  //90% успешных операций
		{
			int ind = rand() % n;
			tree.DelElemTree(m[ind]);
			D += tree.Count();
			int k = LineRand() % (10000 * n);
			k = k + k % 2;		// случайный чётный ключ
			tree.AddElemTree(k, 1);
			I += tree.Count();;
			m[ind] = k;
			try {
				tree.GetElemTree(m[rand() % n]);
				S += tree.Count();
			}
			//обработка исключения при ошибке операции поиска
			catch (...) { S += tree.Count(); }
			int ind1 = rand() % n;
			try {
				tree1.DelElemTree(m[ind1]);
				D1 += tree1.Count();
			}
			catch (...) { D1 += tree1.Count(); }
			int k1 = LineRand() % (10000 * n);
			k1 = k1 + k1 % 2;		// случайный чётный ключ
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
			//обработка исключения при ошибке операции поиска
			catch (...) { S1 += tree1.Count(); }
		}
	}

	//вывод результатов:
	// вывод размера дерева после теста
	cout << endl << endl;
	cout << "Для BST дерева (после теста):" << endl;
	cout << "Размер дерева:" << n << endl;
	//теоретической оценки трудоёмкости операций BST
	cout << "n/2 =" << n / 2 << endl;
	//экспериментальной оценки трудоёмкости вставки
	cout << "Количество вставок: " << I / (n / 2) << endl;
	//экспериментальной оценки трудоёмкости удаления
	cout << "Количество удалений: " << D / (n / 2) << endl;
	//экспериментальной оценки трудоёмкости поиска
	cout << "Количество поиска: " << S / (n / 2) << endl << endl << endl;
	cout << "Для рандомизированного дерева (после теста):" << endl;
	// вывод размера дерева после теста
	cout << "Размер дерева:" << n << endl;
	//теоретической оценки трудоёмкости операций BST
	cout << "log2(n) + 0.25 =" << (log((double)n) / log(2.0)) + 0.25 << endl;
	//экспериментальной оценки трудоёмкости вставки
	cout << "Количество вставок: " << I1 / (n / 2) << endl;
	//экспериментальной оценки трудоёмкости удаления
	cout << "Количество удалений: " << D1 / (n / 2) << endl;
	//экспериментальной оценки трудоёмкости поиска
	cout << "Количество поиска: " << S1 / (n / 2) << endl;
	//освобождение памяти массива m[]
	delete[] m;
}	//конец теста