#include "classHead.h"
#include <iostream>
using namespace std;
Element::Element(char value) {
  el = value;
  setEx = nullptr;
  isSingle = true;
}
Element::Element(Set *set) {
  setEx = set;
  el = '\0';
  isSingle = false;
}
Element::Element(const Element &other) {
  el = other.el;
  isSingle = other.isSingle;

  if (other.isSingle) {
    setEx = nullptr;
  } else {
    setEx = new Set(*other.setEx);
  }
}
Element::~Element() { delete setEx; }

void Set::add(Element elEx) { setEx.push_back(elEx); }
Set Set::initInSet(string &strEx, int &pos) {
  Set result;
  while (strEx[pos] != '}') {
    if (strEx[pos] == '{') {
      pos++;
      result.add(Element(new Set(initInSet(strEx, pos))));
    } else if (strEx[pos] != ',') {
      Element elEx(strEx[pos]);
      result.add(elEx);
    }
    pos++;
  }
  return result;
}
bool Set::operator[](const Element &element) const {
  for (const Element &currentElement : setEx) {
    if (currentElement == element)
      return true;
  }

  return false;
}
bool Set::operator==(const Set &secSet) const {
  if (setEx.size() != secSet.setEx.size())
    return false;
  for (const Element &element : setEx)
    if (!secSet[element])
      return false;
  return true;
}
bool Element::operator==(const Element &secEl) const {
  if (isSingle && secEl.isSingle)
    return el == secEl.el;
  if (!isSingle && !secEl.isSingle)
    return *setEx == *secEl.setEx;
  return false;
}
Set &Set::operator-=(const Set &secSet) {
  for (auto curr = setEx.begin(); curr != setEx.end();) {
    if (secSet[*curr])
      curr = setEx.erase(curr);
    else
      ++curr;
  }

  return *this;
}
void Element::print() {
  if (isSingle)
    cout << el;
  else
    setEx->print();
}
void Set::print() {
  cout << "{";

  for (int i = 0; i < setEx.size(); i++) {
    setEx[i].print();

    if (i < setEx.size() - 1)
      cout << ",";
  }

  cout << "}";
}

Set::Set() {}

Set::~Set() {}
