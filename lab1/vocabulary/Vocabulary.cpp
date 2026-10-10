#include "Vocabulary.h"
#include <fstream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <utility>
using namespace std;
Vocabulary::Vocabulary() { root = NULL; }
Vocabulary::Tree *Vocabulary::searchRoot(const string &word) {
  Tree *current = root;

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
int Vocabulary::AddNode(const string &eng, const string &rus) {
  if (eng.empty())
    return -1;

  if (root == nullptr) {
    root = new Tree{eng, rus, nullptr, nullptr};
    return 0;
  }

  Tree *current = root;

  while (true) {
    if (eng == current->eng)
      return -1;

    if (eng < current->eng) {
      if (current->left == nullptr) {
        current->left = new Tree{eng, rus, nullptr, nullptr};
        return 0;
      }

      current = current->left;
    } else {
      if (current->right == nullptr) {
        current->right = new Tree{eng, rus, nullptr, nullptr};
        return 0;
      }

      current = current->right;
    }
  }
}
// добавление нового слова
Vocabulary &Vocabulary::operator+=(const pair<string, string> &words) {
  AddNode(words.first, words.second);
  return *this;
}

Vocabulary &
Vocabulary::operator+=(const pair<const char *, const char *> &words) {
  if (words.first == nullptr || words.second == nullptr)
    throw invalid_argument("Null C-string");

  return *this += pair<string, string>{words.first, words.second};
}
string &Vocabulary::operator[](const string &engEx) {
  Tree *searchNode = searchRoot(engEx);
  if (searchNode == nullptr)
    throw out_of_range("Word not found: " + engEx);
  return searchNode->rus;
}
Vocabulary::Tree *Vocabulary::detachMinimum(Tree *&subtree) {
  if (subtree->left == nullptr) {
    Tree *minimum = subtree;
    subtree = subtree->right;
    minimum->right = nullptr;
    return minimum;
  }

  return detachMinimum(subtree->left);
}
Vocabulary::Tree *Vocabulary::removeNode(Tree *node, const string &word,
                                         bool &removed) {
  if (node == nullptr)
    return nullptr;

  if (word < node->eng) {
    node->left = removeNode(node->left, word, removed);
  } else if (word > node->eng) {
    node->right = removeNode(node->right, word, removed);
  } else {
    removed = true;

    if (node->left == nullptr) {
      Tree *right = node->right;
      delete node;
      return right;
    }

    if (node->right == nullptr) {
      Tree *left = node->left;
      delete node;
      return left;
    }

    Tree *left = node->left;
    Tree *right = node->right;
    Tree *successor = detachMinimum(right);

    successor->left = left;
    successor->right = right;

    delete node;
    return successor;
  }

  return node;
}

Vocabulary &Vocabulary::operator-=(const string &word) {
  bool removed = false;
  root = removeNode(root, word, removed);
  return *this;
}

size_t Vocabulary::countNodes(const Tree *node) {
  if (node == nullptr)
    return 0;

  return 1 + countNodes(node->left) + countNodes(node->right);
}
size_t Vocabulary::size() const { return countNodes(root); }

void Vocabulary::destroyTree(Tree *node) {
  if (node == nullptr)
    return;

  destroyTree(node->left);
  destroyTree(node->right);
  delete node;
}

Vocabulary::Tree *Vocabulary::cloneTree(const Tree *node) {
  if (node == nullptr)
    return nullptr;

  Tree *copy = new Tree{node->eng, node->rus, nullptr, nullptr};

  try {
    copy->left = cloneTree(node->left);
    copy->right = cloneTree(node->right);
  } catch (...) {
    destroyTree(copy);
    throw;
  }

  return copy;
}

Vocabulary::Vocabulary(const Vocabulary &other) : root(cloneTree(other.root)) {}

Vocabulary &Vocabulary::operator=(const Vocabulary &other) {
  if (this == &other)
    return *this;

  Tree *copiedRoot = cloneTree(other.root);
  destroyTree(root);
  root = copiedRoot;

  return *this;
}

Vocabulary::~Vocabulary() { destroyTree(root); }

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

    if (loaded.AddNode(english, russian) != 0) {
      throw runtime_error("Duplicate word at line: " + to_string(lineNumber));
    }
  }

  if (file.bad())
    throw runtime_error("Error while reading: " + filename);

  swap(root, loaded.root);
}
