#pragma once
#include <cstddef>
#include <string>
#include <utility>
#include <vector>
using namespace std;

class Vocabulary {
private:
  struct Tree {
    string eng;
    string rus;
    Tree *left;
    Tree *right;
  };
  Tree *root;
  Tree *searchRoot(const string &wordEx);
  int AddNode(const string &eng, const string &rus);
  static void destroyTree(Tree *node);
  static Tree *cloneTree(const Tree *node);
  static Tree *detachMinimum(Tree *&subtree);

  static Tree *removeNode(Tree *node, const string &word, bool &removed);

  static size_t countNodes(const Tree *node);

public:
  Vocabulary();
  Vocabulary(const Vocabulary &other);
  Vocabulary &operator=(const Vocabulary &other);
  ~Vocabulary();

  Vocabulary &operator+=(const pair<string, string> &words);

  Vocabulary &operator+=(const pair<const char *, const char *> &words);

  Vocabulary &operator-=(const string &word);

  string &operator[](const string &word);

  size_t size() const;
  void loadFromFile(const string &filename);
};
class Set;
class Element {
  friend class Set;

private:
  char el;
  Set *setEx;
  bool isSingle;

public:
  Element(char value);
  Element(Set *set);
  Element &operator=(const Element &other);
  string toString() const;

  Element(const Element &other);
  ~Element();
  bool operator==(const Element &secEl) const;
};
class Set {
private:
  vector<Element> setEx;

public:
  string toString() const;
  bool isEmpty() const;
  bool remove(const Element &element);
  std::size_t cardinality() const;
  Set operator+(const Set &other) const;
  Set operator-(const Set &other) const;
  Set operator*(const Set &other) const;
  Set powerSet() const;

  void add(const Element &elEx);
  Set();
  ~Set();

  Set &operator+=(const Set &set2);
  Set &operator-=(const Set &set2);
  Set &operator*=(const Set &set2);
  bool operator[](const Element &elSearch) const;
  bool operator==(const Set &secSet) const;
};
