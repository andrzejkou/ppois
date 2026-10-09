#include "classHead.h"

using namespace std;
Element::Element(char value) : el(value), setEx(nullptr), isSingle(true) {}

Element::Element(Set *set) : el('\0'), setEx(set), isSingle(false) {}

Element::Element(const Element &other)
    : el(other.el), setEx(nullptr), isSingle(other.isSingle) {
  if (!isSingle)
    setEx = new Set(*other.setEx);
}
Set Set::operator+(const Set &other) const {
  Set result(*this);
  result += other;
  return result;
}

Set Set::operator-(const Set &other) const {
  Set result(*this);
  result -= other;
  return result;
}

Set Set::operator*(const Set &other) const {
  Set result(*this);
  result *= other;
  return result;
}
bool Set::isEmpty() const { return setEx.empty(); }
Element::~Element() { delete setEx; }

void Set::add(const Element &elEx) {
  if (!(*this)[elEx])
    setEx.push_back(elEx);
}
Element &Element::operator=(const Element &other) {
  if (this == &other)
    return *this;

  Set *copiedSet = nullptr;

  if (!other.isSingle)
    copiedSet = new Set(*other.setEx);

  delete setEx;

  el = other.el;
  isSingle = other.isSingle;
  setEx = copiedSet;

  return *this;
}

bool Set::remove(const Element &element) {
  for (auto curr = setEx.begin(); curr != setEx.end(); ++curr) {
    if (*curr == element) {
      setEx.erase(curr);
      return true;
    }
  }

  return false;
}
std::size_t Set::cardinality() const { return setEx.size(); }
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
Set &Set::operator+=(const Set &secSet) {
  for (auto curr = secSet.setEx.begin(); curr != secSet.setEx.end(); curr++) {
    if (!(*this)[*curr])
      add(*curr);
  }
  return *this;
}
Set &Set::operator*=(const Set &secSet) {
  for (auto curr = setEx.begin(); curr != setEx.end();) {
    if (!secSet[*curr])
      curr = setEx.erase(curr);
    else
      curr++;
  }
  return *this;
}
std::string Element::toString() const {
  if (isSingle)
    return std::string(1, el);

  return setEx->toString();
}
std::string Set::toString() const {
  std::string result = "{";

  for (std::size_t index = 0; index < setEx.size(); ++index) {
    if (index > 0)
      result += ",";

    result += setEx[index].toString();
  }

  result += "}";
  return result;
}
Set Set::powerSet() const {
  Set result;

  result.add(Element(new Set()));

  for (const Element &element : setEx) {
    Set expanded;

    for (const Element &subsetElement : result.setEx) {
      Set subset(*subsetElement.setEx);
      subset.add(element);

      Element newSubset(new Set(subset));
      expanded.add(newSubset);
    }

    result += expanded;
  }

  return result;
}

Set::Set() {}

Set::~Set() {}
