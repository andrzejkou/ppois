#pragma once

#include <string>

class Node {
private:
  std::string eng;
  std::string rus;

  Node *left;
  Node *right;

  friend class Tree;
  friend class Vocabulary;

public:
  Node(const std::string &eng, const std::string &rus);
};
