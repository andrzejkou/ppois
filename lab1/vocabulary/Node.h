
#pragma once

#include <string>

class Tree;
class Vocabulary;

/**
 * @brief Представляет узел бинарного дерева словаря.
 *
 * Узел хранит английское слово, его русский перевод
 * и указатели на левое и правое поддеревья.
 */
class Node {
private:
  std::string eng; ///< Английское слово.
  std::string rus; ///< Русский перевод слова.

  Node *left;  ///< Указатель на левый дочерний узел.
  Node *right; ///< Указатель на правый дочерний узел.

  friend class Tree;
  friend class Vocabulary;

public:
  /**
   * @brief Создаёт узел с английским словом и переводом.
   * @param eng Английское слово.
   * @param rus Русский перевод.
   */
  Node(const std::string &eng, const std::string &rus);
};
