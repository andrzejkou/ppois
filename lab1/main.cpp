#include "set/Set.h"
#include "vocabulary/Vocabulary.h"
#include "set/parser.h"
#include <exception>
#include <iostream>
#include <limits>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>

using namespace std;

using SetStorage = map<string, Set>;

void showSets(const SetStorage &sets) {
  if (sets.empty()) {
    cout << "Множества ещё не созданы.\n";
    return;
  }
  for (const auto &entry : sets)
    cout << entry.first << " = " << entry.second.toString() << '\n';
}
void showCardinality(const SetStorage &sets) {
  string name;

  cout << "Имя множества: ";
  getline(cin >> ws, name);

  auto found = sets.find(name);

  if (found == sets.end()) {
    cout << "Множество не найдено.\n";
    return;
  }

  cout << "Кардинальность множества " << name << " = "
       << found->second.cardinality() << '\n';
}
void checkEmpty(const SetStorage &sets) {
  string name;

  cout << "Имя множества: ";
  getline(cin >> ws, name);

  auto found = sets.find(name);

  if (found == sets.end()) {
    cout << "Множество не найдено.\n";
    return;
  }

  if (found->second.isEmpty())
    cout << name << " — пустое множество.\n";
  else
    cout << name << " — непустое множество.\n";
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
    const auto inserted = sets.emplace(name, newSet);

    cout << inserted.first->first << " = " << inserted.first->second.toString()
         << '\n';
  } catch (const exception &error) {
    cout << "Ошибка: " << error.what() << '\n';
  }
}

void showOneSet(const SetStorage &sets) {
  string name;

  cout << "Имя множества: ";
  getline(cin >> ws, name);

  const auto found = sets.find(name);

  if (found == sets.end()) {
    cout << "Множество не найдено.\n";
    return;
  }
  cout << name << " = " << found->second.toString() << '\n';
}

void checkMembership(const SetStorage &sets) {
  string name;
  string expression;

  cout << "Имя множества: ";
  getline(cin >> ws, name);

  const auto found = sets.find(name);

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

  const auto first = sets.find(firstName);
  const auto second = sets.find(secondName);

  if (first == sets.end() || second == sets.end()) {
    cout << "Одно из множеств не найдено.\n";
    return;
  }

  cout << boolalpha << (first->second == second->second) << '\n';
}

void addElement(SetStorage &sets) {
  string name;
  string expression;

  cout << "Имя множества: ";
  getline(cin >> ws, name);

  auto found = sets.find(name);

  if (found == sets.end()) {
    cout << "Множество не найдено.\n";
    return;
  }

  cout << "Элемент для добавления: ";
  getline(cin >> ws, expression);

  try {
    Element element = set_parser::parseElement(expression);

    if (found->second[element]) {
      cout << "Такой элемент уже есть в множестве.\n";
      return;
    }

    found->second.add(element);

    cout << "Элемент добавлен. Результат: " << found->second.toString() << '\n';
  } catch (const exception &error) {
    cout << "Ошибка: " << error.what() << '\n';
  }
}
void removeElement(SetStorage &sets) {
  string name;
  string expression;

  cout << "Имя множества: ";
  getline(cin >> ws, name);

  auto found = sets.find(name);

  if (found == sets.end()) {
    cout << "Множество не найдено.\n";
    return;
  }

  cout << "Элемент для удаления: ";
  getline(cin >> ws, expression);

  try {
    const Element element = set_parser::parseElement(expression);

    if (found->second.remove(element))
      cout << "Элемент удалён. Результат: " << found->second.toString() << '\n';
    else
      cout << "Такого элемента нет в множестве.\n";
  } catch (const exception &error) {
    cout << "Ошибка: " << error.what() << '\n';
  }
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

  Set result;

  if (operation == '+')
    result = first->second + second->second;
  else if (operation == '-')
    result = first->second - second->second;
  else if (operation == '*')
    result = first->second * second->second;
  else
    return;
  const auto inserted = sets.emplace(resultName, result);
  cout << inserted.first->first << " = " << inserted.first->second.toString()
       << '\n';
}
void showPowerSet(SetStorage &sets) {
  string name;

  cout << "Имя исходного множества: ";
  getline(cin >> ws, name);

  auto found = sets.find(name);

  if (found == sets.end()) {
    cout << "Множество не найдено.\n";
    return;
  }
  const Set result = found->second.powerSet();
  cout << "P(" << name << ") = " << result.toString() << '\n';
}

void addVocabularyWord(Vocabulary &dictionary) {
  string english;
  string russian;

  cout << "Английское слово: ";
  getline(cin >> ws, english);

  cout << "Русский перевод: ";
  getline(cin >> ws, russian);

  if (english.empty() || russian.empty()) {
    cout << "Слово и перевод не должны быть пустыми.\n";
    return;
  }

  size_t oldSize = dictionary.size();
  dictionary += pair<string, string>{english, russian};

  if (dictionary.size() == oldSize)
    cout << "Слово уже существует.\n";
  else
    cout << "Слово добавлено.\n";
}

void removeVocabularyWord(Vocabulary &dictionary) {
  string english;

  cout << "Английское слово для удаления: ";
  getline(cin >> ws, english);

  size_t oldSize = dictionary.size();
  dictionary -= english;

  if (dictionary.size() < oldSize)
    cout << "Слово удалено.\n";
  else
    cout << "Слово не найдено.\n";
}

void findTranslation(Vocabulary &dictionary) {
  string english;

  cout << "Английское слово: ";
  getline(cin >> ws, english);

  try {
    cout << "Перевод: " << dictionary[english] << '\n';
  } catch (const out_of_range &) {
    cout << "Слово не найдено.\n";
  }
}

void replaceTranslation(Vocabulary &dictionary) {
  string english;
  string russian;

  cout << "Английское слово: ";
  getline(cin >> ws, english);

  cout << "Новый перевод: ";
  getline(cin >> ws, russian);

  if (russian.empty()) {
    cout << "Перевод не должен быть пустым.\n";
    return;
  }

  try {
    dictionary[english] = russian;
    cout << "Перевод изменён.\n";
  } catch (const out_of_range &) {
    cout << "Слово не найдено. Сначала добавьте его.\n";
  }
}

void loadVocabulary(Vocabulary &dictionary) {
  string filename;

  cout << "Путь к файлу словаря: ";
  getline(cin >> ws, filename);

  try {
    dictionary.loadFromFile(filename);
    cout << "Загружено слов: " << dictionary.size() << '\n';
  } catch (const exception &error) {
    cout << "Ошибка загрузки: " << error.what() << '\n';
  }
}

void runVocabularyMenu(Vocabulary &dictionary) {
  int choice;

  while (true) {
    cout << "\n===== АНГЛО-РУССКИЙ СЛОВАРЬ =====\n"
         << "1. Добавить слово\n"
         << "2. Удалить слово\n"
         << "3. Найти перевод\n"
         << "4. Изменить перевод\n"
         << "5. Количество слов\n"
         << "6. Загрузить из файла\n"
         << "0. Вернуться в главное меню\n"
         << "Выбор: ";

    if (!(cin >> choice)) {
      if (cin.eof())
        return;

      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "Введите номер пункта меню.\n";
      continue;
    }

    switch (choice) {
    case 1:
      addVocabularyWord(dictionary);
      break;
    case 2:
      removeVocabularyWord(dictionary);
      break;
    case 3:
      findTranslation(dictionary);
      break;
    case 4:
      replaceTranslation(dictionary);
      break;
    case 5:
      cout << "Количество слов: " << dictionary.size() << '\n';
      break;
    case 6:
      loadVocabulary(dictionary);
      break;
    case 0:
      return;
    default:
      cout << "Такого пункта нет.\n";
    }
  }
}

void runSetMenu(SetStorage &sets) {
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
         << "9. Проверить пустоту множества\n"
         << "10. Построить булеан\n"
         << "11. Удалить элемент\n"
         << "12. Определить кардинальность множества\n"
         << "13. Добавить элемент\n"
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
    case 9:
      checkEmpty(sets);
      break;
    case 10:
      showPowerSet(sets);
      break;
    case 11:
      removeElement(sets);
      break;
    case 12:
      showCardinality(sets);
      break;
    case 13:
      addElement(sets);
      break;
    case 0:
      return;
    default:
      cout << "Такого пункта нет.\n";
    }
  }
}
int main() {
  SetStorage sets;
  Vocabulary dictionary;
  int choice;
  while (true) {
    cout << "\n===== ГЛАВНОЕ МЕНЮ =====\n"
         << "1. Работа с множествами\n"
         << "2. Англо-русский словарь\n"
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
      runSetMenu(sets);
      break;
    case 2:
      runVocabularyMenu(dictionary);
      break;
    case 0:
      return 0;
    default:
      cout << "Такого пункта нет.\n";
    }
  }
  return 0;
}
