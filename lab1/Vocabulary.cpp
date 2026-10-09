#include "classHead.h"
#include <stdio.h>
#include <string>
#include <utility>
using namespace std;
Vocabulary::Vocabulary() { root = NULL; }

Tree *Vocabulary::searchRoot(string wordEx) {
  Tree *buff = root;
  while (buff != NULL) {
    if (buff->eng < wordEx)
      buff = buff->right;
    else if (buff->eng > wordEx)
      buff = buff->left;
    else
      return buff;
  }
  return NULL;
}

int Vocabulary::AddNode(string engEx, string rusEx) {

  Tree *newNode = new Tree{engEx, rusEx, nullptr, nullptr};
  if (root == NULL) {
    root = newNode;
  }
  Tree *current = root;
  while (current != NULL) {

    if (current->eng > engEx) {
      if (current->left == NULL) {
        current->left = newNode;
        return 0;
      }
      current = current->left;

    } else if (current->eng < engEx) {
      if (current->right == NULL) {
        current->right = newNode;
        return 0;
      }
      current = current->right;
    } else {
      delete newNode;
      puts("Vocabulary has already this word\n");
      return -1;
    }
    current = newNode;
  }
  return 0;
}
// добавление нового слова
Vocabulary &Vocabulary::operator+=(pair<string, string> words) {
  if (AddNode(words.first, words.second) == -1)
    printf("error\n");
  return *this;
}

// поиск перевода
string &Vocabulary::operator[](string &engEx) {
  Tree *searchNode = searchRoot(engEx);
  if (!searchNode) {
    printf("Слово не найдено\n");
    static string error = "";
    return error;
  }
  return searchNode->rus;
}
