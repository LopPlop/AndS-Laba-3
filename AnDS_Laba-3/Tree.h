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
		Key key; // Ключ элемента
		Data data; // Значение элемента
		Node* left;	// Указатель на левую ветку
		Node* right;  // Указаель на правую ветку
		Node* parent;  //указатель родителя
		int n; //параметр балансировки
		Node(Key k = NULL, Data d = Data()) {//конструктор
			this->data = d;
			this->key = k;
			this->left = NULL;
			this->right = NULL;
			this->parent = NULL;
			this->n = 0;
		}
	};
	Node* root; //корневой элемент дерева
	int size; //размер дерева
	int count; //количество пройденных элемементов
public:
	class Iterator;
	class reIterator;
	Tree(); // Конструктор
	Tree(const Tree<Key, Data>& obj); // Конструктор копирования
	void copy(Node*); //функция копирования
	~Tree(); // Деструктор
	int GetSizeTree(); // Опрос размера дерева
	bool ClearTree(); // Очистка дерева
	bool EmptyTree(); //Проверка дерева на пустоту
	Data& GetElemTree(Key); //Чтение данных по ключу
	bool EditElemTree(Key, Data); //Запись данных по ключу
	bool AddElemTree(Key, Data); //Включение данных с заданным ключом
	bool DelElemTree(Key); //Удаление данных с заданным ключом
	void PrintKey(); //Вывод списка ключей
	int SearchNumKey(int); //Поиск количества узлов ключи которых больше заданного значения
	void PrintStructureTree(int); //Вывод структуры дерева
	void PrintStrKey(Node*, int, int); //вывод структуры поэлементно
	int Count();// Опрос количества пройденных узлов
	Node* SearchPredElem(Node*); //Ищем узел с предыдущим по значению ключом
	Node* SearchNextElem(Node*); //Ищем узел со следующим по значению ключом
	Node* SearchMaxKey(Node*);	// Ищем узел с максимальным ключом
	Node* SearchRightParent(Node*, Node*);// Поиск ближайшего правого родителя
	Node* SearchMinKey(Node*);// Ищем узел с минимальным ключом
	Node* SearchLeftParent(Node*, Node*);// Поиск ближайшего левого родителя

	class Iterator { //прямой итератор
		Tree* ptr;
		Node* cur;
	public:
		Iterator(Tree<Key, Data>& tr) { //конструктор
			ptr = &tr;
			cur = NULL;
		}
		Key& begin() { //установка прямого итератора на первый элемент
			if (ptr->root == NULL) throw Ex;
			cur = ptr->root;
			while (cur != NULL && cur->left != NULL) //спускаемся до низа левого дерева
				cur = cur->left;
			return cur->key;
		}
		Key& end() { //естановка прямого итератора на первый неоприделенный элемент
			if (ptr->root == NULL) throw Ex;
			cur = ptr->root;
			while (cur != NULL && cur->right != NULL) //спускаемся до низа правого дерева
				cur = cur->right;
			Node* temp = cur;
			cur = NULL;
			return temp->key;
		}
		Data& operator*() { //разименование итератора
			if (ptr->root == NULL || cur == NULL) throw Ex;

			return cur->data; //возвращаем текущий элемент
		}
		Key& operator++(int) { //операция перехода к следующему
			if (ptr->root == NULL || cur == NULL) throw Ex;
			cur = ptr->SearchNextElem(cur); //переходим по деверу вперед
		}
		Key& operator--(int) { //операция перехода к предыдущему
			if (ptr->root == NULL || cur == NULL) throw Ex;
			cur = ptr->SearchPredElem(cur);//переходим по дереву назад
		}
		bool operator==(Iterator& it) { //проверка на равенство
			if (ptr->root == NULL || cur == NULL) throw Ex;
			return cur == Iterator.cur;
		}
		bool operator!=(Iterator& it) { //проверка на неравенство
			if (ptr->root == NULL || cur == NULL) throw Ex;
			return cur != Iterator.cur;
		}
	};

	class reIterator {//обраный итератор
		Tree* ptr;
		Node* cur;
	public:
		reIterator(Tree<Key, Data>& rtr) {//конструктор
			ptr = &rtr;
			cur = NULL;
		}
		Key& rbegin() { //установка обратного опретора в начало
			if (ptr->root == NULL) throw Ex;
			cur = ptr->root;
			while (cur != NULL && cur->right != NULL)
				cur = cur->right;
			return cur->key;
		}
		Key& rend() { //установка обратного итератора на первый неустановленный элемент
			if (ptr->root == NULL) throw Ex;
			cur = ptr->root;
			while (cur != NULL && cur->left != NULL)
				cur = cur->left;
			Node* temp = cur;
			cur = NULL;
			return temp->key;
		}
		Data& operator*() { //операция разименования
			if (ptr->root == NULL || cur == NULL) throw Ex;
			return cur->data; //возвращаем текущее значение
		}
		Key& operator++(int) { //операция перехода к следующему
			if (ptr->root == NULL || cur == NULL) throw Ex;
			cur = ptr->SearchPredElem(cur); //переходим по дереву вперед
		}
		Key& operator--(int) { //операция перехода к предыдущему
			if (ptr->root == NULL || cur == NULL) throw Ex;
			cur = ptr->SearchNextElem(cur);//переходим по дереву назад
		}
		bool operator==(reIterator& rit) {//проверка на равенство
			if (ptr->root == NULL || cur == NULL) throw Ex;
			return cur == reIterator.cur;
		}
		bool operator!=(reIterator& rit) {//проверка на неравенство
			if (ptr->root == NULL || cur == NULL) throw Ex;
			return cur != reIterator.cur;
		}
	};
};

template<typename Key, typename Data>
bool Tree<Key, Data>::AddElemTree(Key key, Data data) { //Добавить новые данные по ключу
	count = 0;  //обнуляем счетчик пройденных элементов
	if (root == NULL) { //если дерево пусто
		root = new Node(key, data);  //записываем новый элемент как корень
		size++; //увеличиваем размерность
		return true;  //сообщаем об успешном выполнении операции
	}
	Node* tmp = root, * pred = NULL; //создаем временные элементы
	while (tmp != NULL) {//пока не пройдем все дерево 
		pred = tmp; //запоминаем предыдущий элемент
		count++;   //сделали еще один шаг по дереву
		if (key == (tmp->key))  //если в дереве есть данные с таким ключем
			return false; //возвращаем сообщении о ошибке
		if (key < (tmp->key)) //выбераем по какой ветке идти
			tmp = tmp->left; //спускаемся по левой ветке
		else
			tmp = tmp->right; //спускаемся по правой ветке 
	}
	if (key < (pred->key)) //выбераем по какой ветке идти
		pred->left = new Node(key, data); //вставляем значение в конце левой ветки
	else
		pred->right = new Node(key, data);  //вставляем значение в конце правой ветки

	size++; //увеличиваем размерность
	count++; //сделали еще один шаг по дереву
	return true;  //сообщаем об успешном выполнении операции
}

template<typename Key, typename Data>
bool Tree<Key, Data>::EditElemTree(Key key, Data data) { //Изменение данных по ключу
	count = 0;  //обнуляем счетчик пройденных элементов
	if (root == NULL) //если дерево пусто
		return false;  //сообщаем об ошибке
	Node* tmp = root;  //создаем временные элементы	
	while (tmp != NULL) {//пока не пройдем все дерево 
		count++;   //сделали еще один шаг по дереву
		if (key == (tmp->key)) {  //если в дереве есть данные с таким ключем
			tmp->data = data; //изменяем значение
			return true; //сообщаем об успешном выполнении операции
		}
		if (key < (tmp->key)) //выбераем по какой ветке идти
			tmp = tmp->left; //спускаемся по левой ветке
		else
			tmp = tmp->right; //спускаемся по правой ветке 
	}
	return false; //если ключ так и не найден, возвращаем ошибку
}

template<typename Key, typename Data>
Data& Tree<Key, Data>::GetElemTree(Key key) { //получить значение по ключу
	if (root == NULL) throw Ex; //если древо пусто, выводим исключение
	count = 0;  //обнуляем счетчик пройденных элементов
	Node* tmp = root;//создаем временные элементы	
	while (tmp != NULL && key != (tmp->key)) {
		if (key < (tmp->key)) //выбераем по какой ветке идти
			tmp = tmp->left; //спускаемся по левой ветке
		else
			tmp = tmp->right; //спускаемся по правой ветке 
		count++;   //сделали еще один шаг по дереву
	}
	if (tmp == NULL) throw Ex; //если не найденны данные с заданным ключем, то выводим исключение
	return tmp->data; //возвращаем данные
}

template<typename Key, typename Data>
bool Tree<Key, Data>::DelElemTree(Key key) { //удалить данные по ключу
	if (root == NULL) //если дерево пусто
		return false;  //сообщаем об ошибке
	Node* tmp = root, * pred = NULL, * next = NULL, * tmp_0 = NULL; //создаем временные элементы	
	count = 0; //обнуляем счетчик пройденных элементов
	while (tmp != NULL && tmp->key != key) { //пока не пройдем все дерево и текущий ключ не равен искомоу
		pred = tmp;//запоминаем предыдущий элемент
		if (key < tmp->key)//выбераем по какой ветке идти
			tmp = tmp->left;//спускаемся по левой ветке
		else
			tmp = tmp->right;//спускаемся по правой ветке
		count++;//сделали еще один шаг по дереву
	}
	if (tmp == NULL) //если такого элемента в дереве нет
		return false; //сообщаем об ошибке
	if (tmp->left != NULL && tmp->right != NULL) { //если удаление из серидины
		tmp_0 = tmp;
		pred = tmp;
		tmp = tmp->right; //идем по правой ветке
		while (tmp->left != NULL) { //пока не достгнем конца левой ветки
			count++; //сделали еще один шаг по дереву
			pred = tmp; //запомнили предыдущий элемент
			tmp = tmp->left; //прошли далше по левой ветке
		}
		//запоминаем в временных параметрах
		tmp_0->key = tmp->key;
		tmp_0->data = tmp->data;
		next = tmp->right;
	}
	else { //удаляем из конца
		if (tmp->left == NULL && tmp->right == NULL) //удаляем последний элемент ветки
			next = NULL;
		else {
			if (tmp->left == NULL)
				next = tmp->right; //запомнаем следующий элемент удаляемого значения правой ветки
			else if (tmp->right == NULL)
				next = tmp->left; //запомнаем следующий элемент удаляемого значения левой ветки
		}
	}
	if (pred == NULL)//если остается один элемент
		root = next; //корневому элменту присваеваем последний оставшийся
	else
		if (tmp->key < pred->key)//выбераем по какой ветке идти
			pred->left = next; //вставляем значение в левую ветку
		else
			pred->right = next; //вставляем значение в правую ветку
	delete tmp; //очищаем память
	size--; //уменьшаем размерность
	return true;  //сообщаем об успешном выполнении операции
}

template<typename Key, typename Data>
int Tree<Key, Data>::GetSizeTree() { //узнать размерность
	return size; //возвращаем размерность
}

template<typename Key, typename Data>
bool Tree<Key, Data>::EmptyTree() { //проверка на пустоту
	if (size)
		return false; //0 если дерево пусто
	return true; //1 если есть элементы
}

template<typename Key, typename Data>
void Tree<Key, Data>::PrintKey() { //получить список ключей
	if (root == NULL) throw Ex; //если древо пусто, выводим исключение
	stack<Node*> stack1; //создаем стек
	Node* tmp; //создаем временные элементы
	stack1.push(root); //вставить корневой элемент в стек
	while (!stack1.empty()) { //пока стек не пуст
		tmp = stack1.top(); //переход к следующему элменту
		cout << tmp->key << " "; //выводим все элементы
		stack1.pop(); //удалить верхний уровень
		if (tmp->right != NULL) //если ветка не закончилась
			stack1.push(tmp->right); //вставляем следующий элемент по правой ветке в стек
		if (tmp->left != NULL) //если ветка не закончилась
			stack1.push(tmp->left); //вставляем следующий элемент по левой ветке в стек
	}
}

template<typename Key, typename Data>
bool Tree<Key, Data>::ClearTree() { //очистка дерева
	if (root == NULL) //если дерево пусто
		return false;  //сообщаем об ошибке
	stack<Node*> stack1, stack2; //создаем стеки
	Node* tmp; //создаем временные элементы
	stack1.push(root); //вставить корневой элемент в стек
	while (!stack1.empty()) {//пока стек не пуст
		tmp = stack1.top();//переход к следующему элменту
		stack1.pop();//удалить верхний уровень
		stack2.push(tmp);//вставляем следующий элемент tmp в стек
		if (tmp->left != NULL)//если ветка не закончилась
			stack1.push(tmp->left);//вставляем следующий элемент по левой ветке в стек
		if (tmp->right != NULL)//если ветка не закончилась
			stack1.push(tmp->right); //вставляем следующий элемент по правой ветке в стек
		while (!stack2.empty()) {//пока стек не пуст
			tmp = stack2.top();//переход к следующему элменту
			stack2.pop();//удалить верхний уровень
			delete tmp;//очищаем память
			size--; //уменьшаем размерность
		}
	}
	root = NULL; //ставим метку в корне дерева
	return true;//сообщаем об успешном выполнении операции
}

template<typename Key, typename Data>
void Tree<Key, Data>::PrintStructureTree(int num) { //вывод структуры дерева
	if (root == NULL) throw Ex; //если древо пусто, выводим исключение
	PrintStrKey(root, 0, num); //вызываем рукурсивную функцию вывода дерева
}

template<typename Key, typename Data>
inline void Tree<Key, Data>::PrintStrKey(Node* r, int level, int num) { //функция вывода дерева поэлементно
	if (r == NULL) return; //если прошли дерево, возвращаемся
	PrintStrKey(r->right, level + num, num); //проходим по правой ветке
	if (r->key == root->key) cout << endl;
	for (int i = 0; i < 3 * level; i++) cout << " ";
	if (num == 4)
		cout << r->key << " (" << r->n << ")" << endl;
	if (num == 3)
		cout << r->key << "[" << r->data << "] (" << r->n << ")" << endl;
	if (num == 2)
		cout << r->key << endl;
	if (num == 1)
		cout << r->key << "[" << r->data << "]" << endl; //выводим ключи с значениями
	if (r->key == root->key) cout << endl;
	PrintStrKey(r->left, level + num, num); //проходим по левой ветке
}

template<typename Key, typename Data>
int Tree<Key, Data>::SearchNumKey(int num) {//поиск количества узлов с ключами больше заданного числа
	if (root == NULL) throw Ex; //если древо пусто, выводим исключение
	count = 0; //обнуляем счетчик пройденных элементов
	stack<Node*> stack1;//создаем стек
	Node* tmp;//создаем временные элементы
	stack1.push(root);//вставить корневой элемент в стек
	while (!stack1.empty()) {//пока стек не пуст
		tmp = stack1.top();//переход к следующему элменту
		if (tmp->key > num) count++; //если данный ключ удовлетворяет условию, то увеличиваем на 1 счетчик
		stack1.pop();//удалить верхний уровень
		if (tmp->right != NULL)//если ветка не закончилась
			stack1.push(tmp->right);//вставляем следующий элемент по правой ветке в стек
		if (tmp->left != NULL)//если ветка не закончилась
			stack1.push(tmp->left);//вставляем следующий элемент по левой ветке в стек
	}
	return count; //возвращаем количество подходящих по условию ключей
}

template <class K, class tmp>
int Tree<K, tmp>::Count() { //количество пройденных элементов
	int temp = count; //запоминаем
	count = 0; //обнуляем
	return temp; //возвращаем количество проденных элементов
}

template<typename Key, typename Data>
Tree<Key, Data>::Tree() { //конструктор
	root = NULL; //задаем параметры по умолчанию
	count = 0;
}

template<typename Key, typename Data>
Tree<Key, Data>::Tree(const Tree<Key, Data>& obj) { //конструктор копирования
	root = NULL;
	size = 0;
	copy(obj.root);
}

template<typename Key, typename Data>
void Tree<Key, Data>::copy(typename Tree<Key, Data>::Node* r) { //функция копирования 
	if (r == NULL) //если закончилась ветка
		return;
	AddElemTree(r->key, r->data); //добавляем элемент
	copy(r->left); //проверяем левую ветку
	copy(r->right); //проверяем правую ветку
}

template<typename Key, typename Data>
Tree<Key, Data>::~Tree() { //деструктор
	ClearTree(); //вызываем функцию очистки дерева
}

template<typename Key, typename Data>
typename Tree<Key, Data>::Node* Tree<Key, Data>::SearchPredElem(Node* x) {//Ищем узел с предыдущим по значению ключом
	if (x->left != NULL) return SearchMaxKey(x->left); //если следующий элемент не является конечным, то предыдущий элемент является масимальным ключем
	else return SearchRightParent(root, x); //иначе ищем правого родителя
}

template<typename Key, typename Data>
typename Tree<Key, Data>::Node* Tree<Key, Data>::SearchNextElem(Node* x) {//Ищем узел с следующим по значению ключом
	if (x->right != NULL) return SearchMinKey(x->right); //если следующий элемент не является конечным, то предыдущий элемент является минимальным ключем
	else return SearchLeftParent(root, x);//иначе ищем левого родителя
}

template<typename Key, typename Data>
typename Tree<Key, Data>::Node* Tree<Key, Data>::SearchMaxKey(Node* tmp) { //поиск масимального ключа
	if (tmp == NULL) return NULL; //если закончилось дерево
	while (tmp->right != NULL) tmp = tmp->right; //проходим по правой ветви
	return tmp; //возвращаем максимальный ключ
}

template<typename Key, typename Data>
typename Tree<Key, Data>::Node* Tree<Key, Data>::SearchMinKey(Node* tmp) { //поиск минимального ключа
	if (tmp == NULL) return NULL; //если дерево закончилось
	while (tmp->left != NULL) tmp = tmp->left; //проходим по левой ветви
	return tmp; //возвращаем минимальный ключ
}

template<typename Key, typename Data>
typename Tree<Key, Data>::Node* Tree<Key, Data>::SearchRightParent(Node* tmp, Node* x) {//Поиск ближайшего правого родителя
	if (tmp == x) return NULL; //если дерево закончалось
	if (x->key > tmp->key) { //сравниваем ключи
		Node* rp = SearchRightParent(tmp->right, x); //ищем правого родителя
		if (rp != NULL) return rp; //если родительнайден, то возвращаем его
		else return tmp;
	}
	else return SearchRightParent(tmp->left, x);
}

template<typename Key, typename Data>
typename Tree<Key, Data>::Node* Tree<Key, Data>::SearchLeftParent(Node* tmp, Node* x) {//Поиск ближайшего левого родителя
	if (tmp == x) return NULL;//если дерево закончалось
	if (x->key < tmp->key) {//сравниваем ключи
		Node* lp = SearchLeftParent(tmp->left, x); //ищем левого родителя
		if (lp != NULL) return lp;//если родительнайден, то возвращаем его
		else return tmp;
	}
	else return SearchLeftParent(tmp->right, x);
}

