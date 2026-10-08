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
