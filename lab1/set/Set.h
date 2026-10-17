
#pragma once

#include <cstddef>
#include <string>
#include <vector>

class Set;

/**
 * @brief Представляет элемент множества.
 *
 * Элемент может хранить отдельный символ или ссылаться
 * на вложенное множество.
 */
class Element {
  friend class Set;

private:
  char el;       ///< Символ, представляющий одиночный элемент.
  Set *setEx;    ///< Указатель на вложенное множество.
  bool isSingle; ///< Признак одиночного элемента.

public:
  /**
   * @brief Создаёт одиночный элемент.
   * @param value Символ элемента.
   */
  Element(char value);

  /**
   * @brief Создаёт элемент на основе вложенного множества.
   * @param set Указатель на множество.
   */
  Element(Set *set);

  /**
   * @brief Создаёт копию другого элемента.
   * @param other Элемент, который копируется.
   */
  Element(const Element &other);

  /**
   * @brief Присваивает элементу значение другого элемента.
   * @param other Элемент-источник.
   * @return Ссылка на текущий объект.
   */
  Element &operator=(const Element &other);

  /**
   * @brief Уничтожает элемент и освобождает связанные ресурсы,
   * если это предусмотрено реализацией.
   */
  ~Element();

  /**
   * @brief Сравнивает два элемента на равенство.
   * @param secEl Элемент для сравнения.
   * @return true, если элементы равны, иначе false.
   */
  bool operator==(const Element &secEl) const;

  /**
   * @brief Преобразует элемент в строковое представление.
   * @return Строковое представление элемента.
   */
  std::string toString() const;
};

/**
 * @brief Представляет множество элементов.
 *
 * Поддерживает добавление и удаление элементов,
 * проверку принадлежности, вычисление мощности,
 * а также операции над множествами.
 */
class Set {
private:
  std::vector<Element> setEx; ///< Элементы множества.

public:
  /**
   * @brief Создаёт пустое множество.
   */
  Set();

  /**
   * @brief Уничтожает множество.
   */
  ~Set();

  /**
   * @brief Возвращает строковое представление множества.
   * @return Строковое представление множества.
   */
  std::string toString() const;

  /**
   * @brief Проверяет, пусто ли множество.
   * @return true, если множество пусто.
   */
  bool isEmpty() const;

  /**
   * @brief Удаляет элемент из множества.
   * @param element Удаляемый элемент.
   * @return Результат удаления согласно реализации метода.
   */
  bool remove(const Element &element);

  /**
   * @brief Вычисляет количество элементов множества.
   * @return Мощность множества.
   */
  std::size_t cardinality() const;

  /**
   * @brief Добавляет элемент в множество.
   * @param elEx Добавляемый элемент.
   */
  void add(const Element &elEx);

  /**
   * @brief Вычисляет объединение двух множеств.
   * @param other Второе множество.
   * @return Новое множество, являющееся результатом операции.
   */
  Set operator+(const Set &other) const;

  /**
   * @brief Вычисляет разность множеств.
   * @param other Множество, элементы которого исключаются.
   * @return Новое множество, являющееся результатом операции.
   */
  Set operator-(const Set &other) const;

  /**
   * @brief Вычисляет пересечение двух множеств.
   * @param other Второе множество.
   * @return Новое множество, являющееся результатом операции.
   */
  Set operator*(const Set &other) const;

  /**
   * @brief Строит булеан множества.
   * @return Множество всех подмножеств исходного множества.
   */
  Set powerSet() const;

  /**
   * @brief Выполняет объединение с другим множеством.
   * @param set2 Множество для объединения.
   * @return Ссылка на текущий объект.
   */
  Set &operator+=(const Set &set2);

  /**
   * @brief Выполняет разность с другим множеством.
   * @param set2 Множество, элементы которого исключаются.
   * @return Ссылка на текущий объект.
   */
  Set &operator-=(const Set &set2);

  /**
   * @brief Выполняет пересечение с другим множеством.
   * @param set2 Второе множество.
   * @return Ссылка на текущий объект.
   */
  Set &operator*=(const Set &set2);

  /**
   * @brief Проверяет принадлежность элемента множеству.
   * @param elSearch Искомый элемент.
   * @return true, если элемент содержится в множестве.
   */
  bool operator[](const Element &elSearch) const;

  /**
   * @brief Сравнивает два множества на равенство.
   * @param secSet Множество для сравнения.
   * @return true, если множества равны.
   */
  bool operator==(const Set &secSet) const;
};
