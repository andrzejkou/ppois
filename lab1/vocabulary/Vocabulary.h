
#pragma once

#include <cstddef>
#include <string>
#include <utility>

class Vocabulary {
private:
  struct Tree {
    std::string eng;
    std::string rus;
    Tree *left;
    Tree *right;
  };

  Tree *root;

  Tree *searchRoot(const std::string &wordEx);
  int AddNode(const std::string &eng, const std::string &rus);

  static void destroyTree(Tree *node);
  static Tree *cloneTree(const Tree *node);
  static Tree *detachMinimum(Tree *&subtree);
  static Tree *removeNode(Tree *node, const std::string &word, bool &removed);
  static std::size_t countNodes(const Tree *node);

public:
  Vocabulary();
  Vocabulary(const Vocabulary &other);
  Vocabulary &operator=(const Vocabulary &other);
  ~Vocabulary();

  Vocabulary &operator+=(const std::pair<std::string, std::string> &words);
  Vocabulary &operator+=(const std::pair<const char *, const char *> &words);
  Vocabulary &operator-=(const std::string &word);

  std::string &operator[](const std::string &word);

  std::size_t size() const;
  void loadFromFile(const std::string &filename);
};
