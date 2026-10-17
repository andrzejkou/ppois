
#pragma once

#include "Tree.h"
#include <cstddef>
#include <string>
#include <utility>

/**
 * @brief Представляет англо-русский словарь.
 *
 * Предоставляет операции добавления и удаления слов,
 * получения перевода по ключу, подсчёта записей
 * и загрузки словаря из файла.
 */
class Vocabulary {
private:
  Tree tree; ///< Дерево, хранящее записи словаря.

public:
  /**
   * @brief Создаёт пустой словарь.
   */
  Vocabulary();

  /**
   * @brief Создаёт копию другого словаря.
   * @param other Словарь-источник.
   */
  Vocabulary(const Vocabulary &other);

  /**
   * @brief Присваивает словарю содержимое другого словаря.
   * @param other Словарь-источник.
   * @return Ссылка на текущий словарь.
   */
  Vocabulary &operator=(const Vocabulary &other);

  /**
   * @brief Уничтожает словарь.
   */
  ~Vocabulary();

  /**
   * @brief Добавляет пару слов в словарь.
   * @param words Пара, содержащая английское слово
   * и его русский перевод.
   * @return Ссылка на текущий словарь.
   */
  Vocabulary &operator+=(const std::pair<std::string, std::string> &words);

  /**
   * @brief Добавляет пару слов, представленную C-строками.
   * @param words Пара указателей на английское слово
   * и русский перевод.
   * @return Ссылка на текущий словарь.
   */
  Vocabulary &operator+=(const std::pair<const char *, const char *> &words);

  /**
   * @brief Удаляет запись по английскому слову.
   * @param word Английское слово, которое требуется удалить.
   * @return Ссылка на текущий словарь.
   */
  Vocabulary &operator-=(const std::string &word);

  /**
   * @brief Получает доступ к переводу по английскому слову.
   * @param word Английское слово-ключ.
   * @return Ссылка на строку, связанную с указанным ключом.
   */
  std::string &operator[](const std::string &word);

  /**
   * @brief Возвращает количество записей в словаре.
   * @return Количество записей.
   */
  std::size_t size() const;

  /**
   * @brief Загружает данные словаря из файла.
   * @param filename Путь к файлу с данными словаря.
   */
  void loadFromFile(const std::string &filename);
};
