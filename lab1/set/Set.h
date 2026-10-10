
#pragma once

#include <cstddef>
#include <string>
#include <vector>

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
  Element(const Element &other);
  Element &operator=(const Element &other);
  ~Element();

  bool operator==(const Element &secEl) const;
  std::string toString() const;
};

class Set {
private:
  std::vector<Element> setEx;

public:
  Set();
  ~Set();

  std::string toString() const;
  bool isEmpty() const;
  bool remove(const Element &element);
  std::size_t cardinality() const;

  void add(const Element &elEx);

  Set operator+(const Set &other) const;
  Set operator-(const Set &other) const;
  Set operator*(const Set &other) const;
  Set powerSet() const;

  Set &operator+=(const Set &set2);
  Set &operator-=(const Set &set2);
  Set &operator*=(const Set &set2);

  bool operator[](const Element &elSearch) const;
  bool operator==(const Set &secSet) const;
};
