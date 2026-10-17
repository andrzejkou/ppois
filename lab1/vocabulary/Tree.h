#include "Node.h"
#include <cstddef>
#include <string>
class Tree {
private:
  Node *root;
  friend class Vocabulary;

public:
  Tree();
  Tree(const Tree &other);
  Tree &operator=(const Tree &other);
  ~Tree();
  Node *searchRoot(const std::string &wordEx);
  int AddNode(const std::string &eng, const std::string &rus);

  static void destroyTree(Node *node);
  static Node *cloneTree(const Node *node);
  static Node *detachMinimum(Node *&subtree);
  static Node *removeNode(Node *node, const std::string &word, bool &removed);
  static std::size_t countNodes(const Node *node);
};
