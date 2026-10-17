
#pragma once

#include "Tree.h"
#include <cstddef>
#include <string>
#include <utility>
class Vocabulary {
private:
  Tree tree;

public:
  Vocabulary();
  Vocabulary(const Vocabulary &other);
  Vocabulary &operator=(const Vocabulary &other);
  ~Vocabulary();

  Vocabulary &operator+=(const std::pair<std::string, std::string> &words);
  Vocabulary &operator+=(const std::pair<const char *, const char *> &words);
  Vocabulary &operator-=(const std::string &word);

  std::string &operator[](const std::string &word);

  std::size_t size() const;
  void loadFromFile(const std::string &filename);
};
