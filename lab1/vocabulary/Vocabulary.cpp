#include "Vocabulary.h"
#include "Node.h"
#include <fstream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <utility>
using namespace std;
Vocabulary::Vocabulary() = default;
Tree::Tree() : root(nullptr) {}
Node *Tree::searchRoot(const string &word) {
  Node *current = root;

  while (current != nullptr) {
    if (word == current->eng)
      return current;

    if (word < current->eng)
      current = current->left;
    else
      current = current->right;
  }

  return nullptr;
}
int Tree::AddNode(const string &eng, const string &rus) {
  if (eng.empty())
    return -1;

  if (root == nullptr) {
    root = new Node{eng, rus};
    return 0;
  }

  Node *current = root;

  while (true) {
    if (eng == current->eng)
      return -1;

    if (eng < current->eng) {
      if (current->left == nullptr) {
        current->left = new Node{eng, rus};
        return 0;
      }

      current = current->left;
    } else {
      if (current->right == nullptr) {
        current->right = new Node{eng, rus};
        return 0;
      }

      current = current->right;
    }
  }
}
// добавление нового слова
Vocabulary &
Vocabulary::operator+=(const std::pair<std::string, std::string> &words) {
  tree.AddNode(words.first, words.second);
  return *this;
}

Vocabulary &
Vocabulary::operator+=(const std::pair<const char *, const char *> &words) {
  if (words.first == nullptr || words.second == nullptr)
    throw invalid_argument("Null C-string");

  return *this += pair<string, string>{words.first, words.second};
}
string &Vocabulary::operator[](const string &engEx) {
  Node *searchNode = tree.searchRoot(engEx);
  if (searchNode == nullptr)
    throw out_of_range("Word not found: " + engEx);
  return searchNode->rus;
}
Node *Tree::detachMinimum(Node *&subtree) {
  if (subtree->left == nullptr) {
    Node *minimum = subtree;
    subtree = subtree->right;
    minimum->right = nullptr;
    return minimum;
  }

  return Tree::detachMinimum(subtree->left);
}
Node *Tree::removeNode(Node *node, const string &word, bool &removed) {
  if (node == nullptr)
    return nullptr;

  if (word < node->eng) {
    node->left = Tree::removeNode(node->left, word, removed);
  } else if (word > node->eng) {
    node->right = Tree::removeNode(node->right, word, removed);
  } else {
    removed = true;

    if (node->left == nullptr) {
      Node *right = node->right;
      delete node;
      return right;
    }

    if (node->right == nullptr) {
      Node *left = node->left;
      delete node;
      return left;
    }

    Node *left = node->left;
    Node *right = node->right;
    Node *successor = Tree::detachMinimum(right);

    successor->left = left;
    successor->right = right;

    delete node;
    return successor;
  }

  return node;
}

Vocabulary &Vocabulary::operator-=(const string &word) {
  bool removed = false;
  tree.root = tree.removeNode(tree.root, word, removed);
  return *this;
}

size_t Tree::countNodes(const Node *node) {
  if (node == nullptr)
    return 0;

  return 1 + countNodes(node->left) + countNodes(node->right);
}
size_t Vocabulary::size() const { return Tree::countNodes(tree.root); }

void Tree::destroyTree(Node *node) {
  if (node == nullptr)
    return;

  destroyTree(node->left);
  destroyTree(node->right);
  delete node;
}
Tree::~Tree() { destroyTree(root); }
Node *Tree::cloneTree(const Node *node) {
  if (node == nullptr)
    return nullptr;

  Node *copy = new Node{node->eng, node->rus};

  try {
    copy->left = cloneTree(node->left);
    copy->right = cloneTree(node->right);
  } catch (...) {
    Tree::destroyTree(copy);
    throw;
  }

  return copy;
}

Vocabulary::Vocabulary(const Vocabulary &other) : tree(other.tree) {}
Tree::Tree(const Tree &other) : root(cloneTree(other.root)) {}
Tree &Tree::operator=(const Tree &other) {
  if (this == &other)
    return *this;

  Node *copiedRoot = cloneTree(other.root);

  destroyTree(root);
  root = copiedRoot;

  return *this;
}
Vocabulary &Vocabulary::operator=(const Vocabulary &other) {
  if (this == &other)
    return *this;

  tree = other.tree;

  return *this;
}
Vocabulary::~Vocabulary() = default;

void Vocabulary::loadFromFile(const string &filename) {
  ifstream file(filename);

  if (!file.is_open())
    throw runtime_error("Cannot open file: " + filename);

  Vocabulary loaded;
  string line;
  size_t lineNumber = 0;

  while (getline(file, line)) {
    ++lineNumber;

    if (!line.empty() && line.back() == '\r')
      line.pop_back();

    if (line.empty())
      continue;

    size_t separator = line.find('\t');

    if (separator == string::npos || separator == 0 ||
        separator == line.size() - 1) {
      throw runtime_error("Invalid dictionary line: " + to_string(lineNumber));
    }

    string english = line.substr(0, separator);
    string russian = line.substr(separator + 1);

    if (loaded.tree.AddNode(english, russian) != 0) {
      throw runtime_error("Duplicate word at line: " + to_string(lineNumber));
    }
  }

  if (file.bad())
    throw runtime_error("Error while reading: " + filename);

  swap(tree.root, loaded.tree.root);
}
