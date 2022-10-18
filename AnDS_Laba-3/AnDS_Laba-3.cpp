#include <iostream>
#include "RND_Tree.h"

#define developer false

using namespace std;

int main() {
	setlocale(LC_ALL, "Russian");
	RND_Tree<int, int> tree;
	RND_Tree<int, int>::Iterator itr(tree);
	RND_Tree<int, int>::reIterator r_itr(tree);
	int tmp1, tmp2;
	bool out = true;
	while (out) {
		//system("cls");
		cout << endl << endl;
		if (developer == true) {
			cout << "\t\t\tМеню разработчика:" << endl;
			cout << "Структура дерева с данными: " << endl;
			try
			{
				tree.PrintStructureTree(4);
			}
			catch (...)
			{
				cout << Ex << endl;
			}
			cout << "============================================================" << endl;
		}
		cout << "Меню Списка:" << endl;
		cout << "01 - Добавить новые данные" << endl;
		cout << "02 - Изменить данные по ключу" << endl;
		cout << "03 - Получить данные по ключу" << endl;
		cout << "04 - Удалить данные по ключу" << endl;
		cout << "05 - Узнать размерность дерева" << endl;
		cout << "06 - Проверка дерева на пустоту" << endl;
		cout << "07 - Вывести скисок ключей" << endl;
		cout << "08 - Очистить дерево" << endl;
		cout << "09 - Вывод структуры дерева" << endl;
		cout << "10 - Узнать количество узлов с ключем > заданного значения" << endl;
		cout << "11 - Количество пройденных элементов" << endl;
		cout << "12 - begin() - узел с минимальным ключем" << endl;
		cout << "13 - end()" << endl;
		cout << "-->14 - Разименование узла" << endl;
		cout << "-->15 - Изменение узла" << endl;
		cout << "-->16 - Перейти к следующему узлу" << endl;
		cout << "-->17 - Перейти к предыдущему узлу" << endl;
		cout << "-->26 - Изменение значения итератора" << endl;
		cout << "18 - rbegin() - узел с максимальным ключем" << endl;
		cout << "19 - rend()" << endl;
		cout << "-->20 - Разименование узла" << endl;
		cout << "-->21 - Изменение узла" << endl;
		cout << "-->22 - Перейти к следующему узлу" << endl;
		cout << "-->23 - Перейти к предыдущему узлу" << endl;
		cout << "-->27 - Изменение значения итератора" << endl;
		cout << "24 - Тест трудоёмкости рандомного дерава" << endl;
		cout << "25 - Тест трудоёмкости вырожденного дерава" << endl;
		cout << "-1 - Выйти из меню" << endl;
		cout << endl << "Ввод>> ";
		cin >> tmp1;
		switch (tmp1) {
		case 1:
			cout << "Введите новый ключ: ";
			cin >> tmp1;
			cout << "Введите новые данные: ";
			cin >> tmp2;
			cout << tree.AddElemTree(tmp1, tmp2) << endl;
			system("pause");
			break;
		case 2:
			cout << "Введите ключ: ";
			cin >> tmp1;
			cout << "Введите новые данные: ";
			cin >> tmp2;
			cout << tree.EditElemTree(tmp1, tmp2) << endl;
			system("pause");
			break;
		case 3:
			cout << "Введите ключ: ";
			cin >> tmp1;
			try
			{
				cout << tree.GetElemTree(tmp1) << endl;
			}
			catch (...)
			{
				cout << Ex << endl;
			}
			system("pause");
			break;
		case 4:
			cout << "Введите ключ: ";
			cin >> tmp1;
			cout << tree.DelElemTree(tmp1) << endl;
			system("pause");
			break;
		case 5:
			cout << "Размерность дерева: ";
			cout << tree.GetSizeTree() << endl;
			system("pause");
			break;
		case 6:
			cout << tree.EmptyTree() << endl;
			system("pause");
			break;
		case 7:
			cout << "Список ключей: ";
			try
			{
				tree.PrintKey();
				cout << endl;
			}
			catch (...)
			{
				cout << Ex << endl;
			}
			system("pause");
			break;
		case 8:
			cout << tree.ClearTree() << endl;
			system("pause");
			break;
		case 9:
			cout << "01 - Структура с данными и ключами" << endl;
			cout << "02 - Структура только с ключами" << endl;
			cout << "03 - Структура с ключами, данными и балансировкой" << endl;
			cout << "04 - Структура с ключами и балансировкой" << endl;
			cout << endl << "\t-> ";
			cin >> tmp1;
			try
			{
				tree.PrintStructureTree(tmp1);
			}
			catch (...)
			{
				cout << Ex << endl;
			}
			system("pause");
			break;
		case 10:
			cout << "Введите число: ";
			cin >> tmp1;
			cout << "Количество узлов больших заданного значения: ";
			try
			{
				cout << tree.SearchNumKey(tmp1) << endl;
			}
			catch (...)
			{
				cout << Ex << endl;
			}
			system("pause");
			break;
		case 11:
			cout << "Количество просмотренных узлов: ";
			cout << tree.Count() << endl;
			system("pause");
			break;
		case 12:
			try
			{
				itr.begin();
			}
			catch (...)
			{
				cout << Ex << endl;
				system("pause");
			}
			break;
		case 13:
			try
			{
				itr.end();
			}
			catch (...)
			{
				cout << Ex << endl;
				system("pause");
			}
			break;
		case 14:
			try {
				cout << "Данные текущего узла: " << *(itr) << endl;
			}
			catch (...) {
				cout << Ex << endl;
			}
			system("pause");
			break;
		case 15:
			cout << "Введите новые данные: ";
			cin >> tmp1;
			try {
				*(itr) = tmp1;
			}
			catch (...) {
				cout << Ex << endl;
				system("pause");
			}
			break;
		case 16:
			try {
				itr++;
			}
			catch (...) {
				cout << Ex << endl;
				system("pause");
			}
			break;
		case 17:
			try {
				itr++;
			}
			catch (...) {
				cout << Ex << endl;
				system("pause");
			}
			break;
		case 18:
			try
			{
				r_itr.rbegin();
			}
			catch (...)
			{
				cout << Ex << endl;
				system("pause");
			}
			break;
		case 19:
			try
			{
				r_itr.rend();
			}
			catch (...)
			{
				cout << Ex << endl;
				system("pause");
			}
			break;
		case 20:
			try {
				cout << "Данные текущего узла: " << *(r_itr) << endl;
			}
			catch (...) {
				cout << Ex << endl;
			}
			system("pause");
			break;
		case 21:
			cout << "Введите новые данные: ";
			cin >> tmp1;
			try {
				*(r_itr) = tmp1;
			}
			catch (...) {
				cout << Ex << endl;
				system("pause");
			}
			break;
		case 22:
			try {
				r_itr++;
			}
			catch (...) {
				cout << Ex << endl;
				system("pause");
			}
			break;
		case 23:
			try {
				r_itr++;
			}
			catch (...) {
				cout << Ex << endl;
				system("pause");
			}
			break;
		case 24:
			cout << "Введите количество узлов: ";
			cin >> tmp1;
			test_rand(tmp1);
			system("pause");
			break;
		case 25:
			cout << "Введите количество узлов: ";
			cin >> tmp1;
			test_ord(tmp1);
			system("pause");
			break;
		case 26:
		{
			cout << "Введите значение: ";
			cin >> *itr;
			cout << *itr;
			break;
		}
		case 27:
		{
			cout << "Введите значение: ";
			cin >> *r_itr;
			cout << *r_itr;
			break;
		}
		case -1:
			out = false;
			break;
		default:
			break;
		}
	}
}
