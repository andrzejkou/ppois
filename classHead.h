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
private:
  char el;
  Set *setEx;
  bool isSingle;

public:
  Element(char value);
  Element(Set *set);
  void print();
  Element(const Element &other);
  ~Element();
  bool operator==(const Element &secEl) const;
};
class Set {
private:
  vector<Element> setEx;

public:
  void print();
  bool isContain(Element elEx) const;
  void add(Element elEx);
  Set initInSet(string &strEx, int &pos);
  Set();
  ~Set();
  Set &operator+=(Set set2);
  Set &operator-=(const Set &set2);
  bool operator[](const Element &elSearch) const;
  bool operator==(const Set &secSet) const;
};
