#include "classHead.h"
#include "parser.h"

#include <iostream>
#include <limits>
#include <map>
#include <string>

using namespace std;

using SetStorage = map<string, Set>;

void showSets(const SetStorage &sets) {
  if (sets.empty()) {
    cout << "Множества ещё не созданы.\n";
    return;
  }

  for (const auto &entry : sets) {
    cout << entry.first << " = ";
    entry.second.print();
    cout << '\n';
  }
}

void createSet(SetStorage &sets) {
  string name;
  string expression;

  cout << "Имя множества: ";
  getline(cin >> ws, name);

  if (sets.find(name) != sets.end()) {
    cout << "Такое имя уже занято.\n";
    return;
  }

  cout << "Введите множество: ";
  getline(cin >> ws, expression);

  try {
    Set newSet = set_parser::parse(expression);
    sets.emplace(name, newSet);

    cout << name << " = ";
    sets.find(name)->second.print();
    cout << '\n';
  } catch (const exception &error) {
    cout << "Ошибка: " << error.what() << '\n';
  }
}

void showOneSet(const SetStorage &sets) {
  string name;

  cout << "Имя множества: ";
  getline(cin >> ws, name);

  auto found = sets.find(name);

  if (found == sets.end()) {
    cout << "Множество не найдено.\n";
    return;
  }

  cout << name << " = ";
  found->second.print();
  cout << '\n';
}

void checkMembership(const SetStorage &sets) {
  string name;
  string expression;

  cout << "Имя множества: ";
  getline(cin >> ws, name);

  auto found = sets.find(name);

  if (found == sets.end()) {
    cout << "Множество не найдено.\n";
    return;
  }

  cout << "Введите элемент: ";
  getline(cin >> ws, expression);

  try {
    Element element = set_parser::parseElement(expression);
    cout << boolalpha << found->second[element] << '\n';
  } catch (const exception &error) {
    cout << "Ошибка: " << error.what() << '\n';
  }
}

void compareSets(const SetStorage &sets) {
  string firstName;
  string secondName;

  cout << "Первое множество: ";
  getline(cin >> ws, firstName);

  cout << "Второе множество: ";
  getline(cin >> ws, secondName);

  auto first = sets.find(firstName);
  auto second = sets.find(secondName);

  if (first == sets.end() || second == sets.end()) {
    cout << "Одно из множеств не найдено.\n";
    return;
  }

  cout << boolalpha << (first->second == second->second) << '\n';
}

void performOperation(SetStorage &sets, char operation) {
  string firstName;
  string secondName;
  string resultName;

  cout << "Первое множество: ";
  getline(cin >> ws, firstName);

  cout << "Второе множество: ";
  getline(cin >> ws, secondName);

  auto first = sets.find(firstName);
  auto second = sets.find(secondName);

  if (first == sets.end() || second == sets.end()) {
    cout << "Одно из множеств не найдено.\n";
    return;
  }

  cout << "Имя результата: ";
  getline(cin >> ws, resultName);

  if (sets.find(resultName) != sets.end()) {
    cout << "Такое имя уже занято.\n";
    return;
  }

  Set result(first->second);

  if (operation == '+')
    result += second->second;
  else if (operation == '*')
    result *= second->second;
  else if (operation == '-')
    result -= second->second;

  sets.emplace(resultName, result);

  cout << resultName << " = ";
  sets.find(resultName)->second.print();
  cout << '\n';
}

int main() {
  SetStorage sets;
  int choice;

  while (true) {
    cout << "\n===== МЕНЮ МНОЖЕСТВ =====\n"
         << "1. Показать все множества\n"
         << "2. Создать множество\n"
         << "3. Показать одно множество\n"
         << "4. Проверить принадлежность элемента\n"
         << "5. Сравнить два множества\n"
         << "6. Объединение\n"
         << "7. Пересечение\n"
         << "8. Разность\n"
         << "0. Выход\n"
         << "Выбор: ";

    if (!(cin >> choice)) {
      if (cin.eof())
        break;

      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Введите номер пункта меню.\n";
      continue;
    }

    switch (choice) {
    case 1:
      showSets(sets);
      break;
    case 2:
      createSet(sets);
      break;
    case 3:
      showOneSet(sets);
      break;
    case 4:
      checkMembership(sets);
      break;
    case 5:
      compareSets(sets);
      break;
    case 6:
      performOperation(sets, '+');
      break;
    case 7:
      performOperation(sets, '*');
      break;
    case 8:
      performOperation(sets, '-');
      break;
    case 0:
      return 0;
    default:
      cout << "Такого пункта нет.\n";
    }
  }

  return 0;
}
