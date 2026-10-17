
#pragma once

#include "Node.h"
#include <cstddef>
#include <string>

class Vocabulary;

/**
 * @brief Реализует бинарное дерево узлов словаря.
 *
 * Класс отвечает за управление деревом, поиск слов,
 * добавление и удаление узлов, а также копирование дерева.
 */
class Tree {
private:
  Node *root; ///< Корень дерева.

  friend class Vocabulary;

public:
  /**
   * @brief Создаёт пустое дерево.
   */
  Tree();

  /**
   * @brief Создаёт копию другого дерева.
   * @param other Дерево-источник.
   */
  Tree(const Tree &other);

  /**
   * @brief Присваивает дереву содержимое другого дерева.
   * @param other Дерево-источник.
   * @return Ссылка на текущее дерево.
   */
  Tree &operator=(const Tree &other);

  /**
   * @brief Уничтожает дерево.
   */
  ~Tree();

  /**
   * @brief Ищет узел по английскому слову.
   * @param wordEx Искомое слово.
   * @return Указатель на найденный узел либо nullptr,
   * если узел не найден.
   */
  Node *searchRoot(const std::string &wordEx);

  /**
   * @brief Добавляет слово и его перевод в дерево.
   * @param eng Английское слово.
   * @param rus Русский перевод.
   * @return Целочисленный результат операции добавления,
   * определяемый реализацией метода.
   */
  int AddNode(const std::string &eng, const std::string &rus);

  /**
   * @brief Рекурсивно уничтожает поддерево.
   * @param node Корень уничтожаемого поддерева.
   */
  static void destroyTree(Node *node);

  /**
   * @brief Создаёт копию поддерева.
   * @param node Корень копируемого поддерева.
   * @return Указатель на корень копии либо nullptr,
   * если исходный указатель равен nullptr.
   */
  static Node *cloneTree(const Node *node);

  /**
   * @brief Отсоединяет узел с минимальным ключом
   * от указанного поддерева.
   * @param subtree Ссылка на указатель корня поддерева.
   * @return Указатель на отсоединённый узел.
   */
  static Node *detachMinimum(Node *&subtree);

  /**
   * @brief Удаляет узел с заданным словом из поддерева.
   * @param node Корень обрабатываемого поддерева.
   * @param word Слово, которое требуется удалить.
   * @param removed Флаг, отражающий результат удаления.
   * @return Новый корень обработанного поддерева.
   */
  static Node *removeNode(Node *node, const std::string &word, bool &removed);

  /**
   * @brief Подсчитывает количество узлов поддерева.
   * @param node Корень поддерева.
   * @return Количество узлов.
   */
  static std::size_t countNodes(const Node *node);
};
