
#include "../classHead.h"

#include <cstdio>
#include <fstream>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

namespace {
int passedTests = 0;
int failedTests = 0;

void check(bool condition, const std::string &message) {
  if (!condition)
    throw std::runtime_error(message);
}

using StringPair = std::pair<std::string, std::string>;

void addWord(Vocabulary &dictionary, const std::string &english,
             const std::string &russian) {
  dictionary += StringPair{english, russian};
}

void expectMissingWord(Vocabulary &dictionary, const std::string &word) {
  bool threw = false;

  try {
    (void)dictionary[word];
  } catch (const std::out_of_range &) {
    threw = true;
  }

  check(threw, "Expected missing word: " + word);
}

class TemporaryFile {
private:
  std::string path;

public:
  TemporaryFile(const std::string &filename, const std::string &contents)
      : path(filename) {
    std::ofstream file(path, std::ios::binary);

    if (!file)
      throw std::runtime_error("Cannot create test file");

    file << contents;

    if (!file)
      throw std::runtime_error("Cannot write test file");
  }

  ~TemporaryFile() { std::remove(path.c_str()); }

  const std::string &getPath() const { return path; }
};

void testEmptyDictionary() {
  Vocabulary dictionary;

  check(dictionary.size() == 0, "New dictionary should be empty");
  expectMissingWord(dictionary, "unknown");

  dictionary -= std::string("unknown");

  check(dictionary.size() == 0, "Removing absent word changes nothing");
}

void testAddingBothStringTypes() {
  Vocabulary dictionary;

  dictionary += StringPair{"apple", "яблоко"};
  dictionary += std::pair<const char *, const char *>{"book", "книга"};

  check(dictionary.size() == 2, "Both overloads should insert words");
  check(dictionary["apple"] == "яблоко", "String overload failed");
  check(dictionary["book"] == "книга", "C-string overload failed");
}

void testDuplicateWords() {
  Vocabulary dictionary;

  addWord(dictionary, "apple", "яблоко");
  addWord(dictionary, "apple", "груша");
  addWord(dictionary, "", "пустой ключ");

  check(dictionary.size() == 1, "Duplicate or empty key was inserted");
  check(dictionary["apple"] == "яблоко", "Duplicate replaced translation");

  bool threw = false;

  try {
    dictionary += std::pair<const char *, const char *>{nullptr, "перевод"};
  } catch (const std::invalid_argument &) {
    threw = true;
  }

  check(threw, "Null C-string should be rejected");
}

void testTranslationReplacement() {
  Vocabulary dictionary;

  addWord(dictionary, "apple", "яблоко");
  dictionary["apple"] = "яблочко";

  check(dictionary["apple"] == "яблочко", "Translation was not replaced");
  expectMissingWord(dictionary, "pear");
}

void testRemovingLeaf() {
  Vocabulary dictionary;

  addWord(dictionary, "m", "м");
  addWord(dictionary, "c", "с");
  addWord(dictionary, "t", "т");

  dictionary -= std::string("c");

  check(dictionary.size() == 2, "Leaf removal failed");
  expectMissingWord(dictionary, "c");
  check(dictionary["m"] == "м", "Root was damaged");
  check(dictionary["t"] == "т", "Right subtree was damaged");
}

void testRemovingNodeWithOneChild() {
  Vocabulary rightChild;

  addWord(rightChild, "m", "м");
  addWord(rightChild, "c", "с");
  addWord(rightChild, "e", "е");

  rightChild -= std::string("c");

  check(rightChild.size() == 2, "Right-child removal size is incorrect");
  check(rightChild["e"] == "е", "Right child was lost");
  expectMissingWord(rightChild, "c");

  Vocabulary leftChild;

  addWord(leftChild, "m", "м");
  addWord(leftChild, "c", "с");
  addWord(leftChild, "a", "а");

  leftChild -= std::string("c");

  check(leftChild.size() == 2, "Left-child removal size is incorrect");
  check(leftChild["a"] == "а", "Left child was lost");
  expectMissingWord(leftChild, "c");
}

void testRemovingNodeWithTwoChildren() {
  Vocabulary dictionary;

  addWord(dictionary, "m", "м");
  addWord(dictionary, "c", "с");
  addWord(dictionary, "t", "т");

  dictionary -= std::string("m");

  check(dictionary.size() == 2, "Root with two children was not removed");
  check(dictionary["c"] == "с", "Left subtree was lost");
  check(dictionary["t"] == "т", "Successor was lost");
  expectMissingWord(dictionary, "m");

  Vocabulary deeperSuccessor;

  addWord(deeperSuccessor, "m", "м");
  addWord(deeperSuccessor, "c", "с");
  addWord(deeperSuccessor, "t", "т");
  addWord(deeperSuccessor, "p", "п");
  addWord(deeperSuccessor, "z", "з");

  deeperSuccessor -= std::string("m");

  check(deeperSuccessor.size() == 4, "Deep successor removal failed");
  check(deeperSuccessor["p"] == "п", "Successor was not reattached");
  check(deeperSuccessor["z"] == "з", "Right subtree was damaged");
}

void testCopyConstructionAndAssignment() {
  Vocabulary original;

  addWord(original, "m", "м");
  addWord(original, "b", "б");
  addWord(original, "t", "т");

  Vocabulary copied(original);

  copied["b"] = "Б";
  copied -= std::string("m");

  check(original.size() == 3, "Copy changed original size");
  check(original["b"] == "б", "Copy changed original translation");
  check(copied.size() == 2, "Copied dictionary removal failed");

  Vocabulary assigned;
  addWord(assigned, "old", "старое");

  assigned = original;
  assigned["t"] = "Т";

  check(assigned.size() == 3, "Copy assignment failed");
  check(original["t"] == "т", "Assignment was not a deep copy");

  assigned = assigned;

  check(assigned.size() == 3, "Self-assignment damaged dictionary");

  Vocabulary empty;
  assigned = empty;

  check(assigned.size() == 0, "Assignment from empty dictionary failed");
}

void testSuccessfulFileLoading() {
  TemporaryFile file("tests/vocabulary_valid.tmp",
                     "apple\tяблоко\r\nbook\tкнига\n\nhouse\tдом");

  Vocabulary dictionary;
  addWord(dictionary, "old", "старое");

  dictionary.loadFromFile(file.getPath());

  check(dictionary.size() == 3, "File loading returned wrong size");
  check(dictionary["apple"] == "яблоко", "First file entry failed");
  check(dictionary["book"] == "книга", "Second file entry failed");
  check(dictionary["house"] == "дом", "Last file entry failed");
  expectMissingWord(dictionary, "old");
}

void testFailedLoadingPreservesDictionary() {
  TemporaryFile malformed("tests/vocabulary_malformed.tmp",
                          "apple\tяблоко\ninvalid_line\n");

  Vocabulary dictionary;
  addWord(dictionary, "existing", "существующее");

  bool threw = false;

  try {
    dictionary.loadFromFile(malformed.getPath());
  } catch (const std::runtime_error &) {
    threw = true;
  }

  check(threw, "Malformed file should be rejected");
  check(dictionary.size() == 1, "Failed load changed dictionary");
  check(dictionary["existing"] == "существующее",
        "Failed load lost original word");

  bool missingFileThrew = false;

  try {
    dictionary.loadFromFile("tests/no_such_dictionary.tmp");
  } catch (const std::runtime_error &) {
    missingFileThrew = true;
  }

  check(missingFileThrew, "Missing file should be rejected");
}

void testInvalidFileRecords() {
  TemporaryFile duplicate("tests/vocabulary_duplicate.tmp",
                          "apple\tяблоко\napple\tяблоня\n");

  Vocabulary dictionary;
  addWord(dictionary, "existing", "старое");

  bool duplicateThrew = false;

  try {
    dictionary.loadFromFile(duplicate.getPath());
  } catch (const std::runtime_error &) {
    duplicateThrew = true;
  }

  check(duplicateThrew, "Duplicate file key should be rejected");
  check(dictionary.size() == 1, "Duplicate file changed original dictionary");

  TemporaryFile missingTranslation("tests/vocabulary_missing_translation.tmp",
                                   "apple\t\n");

  bool malformedThrew = false;

  try {
    dictionary.loadFromFile(missingTranslation.getPath());
  } catch (const std::runtime_error &) {
    malformedThrew = true;
  }

  check(malformedThrew, "Empty translation should be rejected");
  check(dictionary["existing"] == "старое", "Original entry was lost");
}

void runTest(const std::string &name, const std::function<void()> &test) {
  try {
    test();
    ++passedTests;
    std::cout << "[PASS] " << name << '\n';
  } catch (const std::exception &error) {
    ++failedTests;
    std::cout << "[FAIL] " << name << ": " << error.what() << '\n';
  } catch (...) {
    ++failedTests;
    std::cout << "[FAIL] " << name << ": unknown exception\n";
  }
}
} // namespace

int main() {
  runTest("empty dictionary", testEmptyDictionary);
  runTest("string and C-string insertion", testAddingBothStringTypes);
  runTest("duplicate words", testDuplicateWords);
  runTest("translation replacement", testTranslationReplacement);
  runTest("removing leaf", testRemovingLeaf);
  runTest("removing node with one child", testRemovingNodeWithOneChild);
  runTest("removing node with two children", testRemovingNodeWithTwoChildren);
  runTest("copy and assignment", testCopyConstructionAndAssignment);
  runTest("successful file loading", testSuccessfulFileLoading);
  runTest("failed file loading", testFailedLoadingPreservesDictionary);
  runTest("invalid file records", testInvalidFileRecords);

  std::cout << "\nPassed: " << passedTests << "\nFailed: " << failedTests
            << '\n';

  return failedTests == 0 ? 0 : 1;
}
