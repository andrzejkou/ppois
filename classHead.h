#pragma once
#include <cstddef>
#include <string>
#include <vector>

using namespace std;
struct Tree {
  string eng;
  string rus;
  Tree *left;
  Tree *right;
};
class Vocabulary {
private:
  Tree *root;
  Tree *searchRoot(string wordEx);
  int AddNode(string engEx, string rusEx);

public:
  Vocabulary();
  ~Vocabulary();
  Vocabulary &operator+=(pair<string, string> words);
  Vocabulary &operator-=(string engEx);
  string &operator[](string &engEx);
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

  void print() const;
  Element(const Element &other);
  ~Element();
  bool operator==(const Element &secEl) const;
};
class Set {
private:
  vector<Element> setEx;

public:
  bool isEmpty() const;
  bool remove(const Element &element);
  std::size_t cardinality() const;
  Set operator+(const Set &other) const;
  Set operator-(const Set &other) const;
  Set operator*(const Set &other) const;
  Set powerSet() const;
  void print() const;
  void add(const Element &elEx);
  Set();
  ~Set();

  Set &operator+=(const Set &set2);
  Set &operator-=(const Set &set2);
  Set &operator*=(const Set &set2);
  bool operator[](const Element &elSearch) const;
  bool operator==(const Set &secSet) const;
};
