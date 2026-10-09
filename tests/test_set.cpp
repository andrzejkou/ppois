#include "../classHead.h"
#include "../parser.h"
#include <exception>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {
int passedTests = 0;
int failedTests = 0;

void check(bool condition, const std::string &message) {
  if (!condition)
    throw std::runtime_error(message);
}

Set parse(const std::string &expression) {
  return set_parser::parse(expression);
}

Element parseElement(const std::string &expression) {
  return set_parser::parseElement(expression);
}

void expectInvalidSet(const std::string &expression) {
  bool threw = false;

  try {
    (void)parse(expression);
  } catch (const std::exception &) {
    threw = true;
  }

  check(threw, "Expected invalid set expression to be rejected: " + expression);
}

void expectInvalidElement(const std::string &expression) {
  bool threw = false;

  try {
    (void)parseElement(expression);
  } catch (const std::exception &) {
    threw = true;
  }

  check(threw,
        "Expected invalid element expression to be rejected: " + expression);
}

void testEmptySet() {
  Set empty = parse("{}");

  check(empty.isEmpty(), "Empty set should report isEmpty() == true");
  check(empty.cardinality() == 0, "Empty set should have cardinality zero");
  check(empty.toString() == "{}", "Empty set should stringify as {}");
  check(!empty[Element('a')], "Empty set must not contain an atom");
}

void testParsingNestedSets() {
  Set value = parse("{ a, {b, c}, {} }");

  check(value.cardinality() == 3,
        "Nested sets count as one outer element each");
  check(value[Element('a')], "Outer set should contain a");
  check(value[parseElement("{b,c}")], "Outer set should contain nested {b,c}");
  check(value[parseElement("{}")], "Outer set should contain the empty set");
  check(!value[Element('b')],
        "Nested atom b must not be treated as an outer element");
}

void testDuplicateElementsAreNotAdded() {
  Set value = parse("{a,a,b}");
  value.add(Element('a'));
  value.add(Element('c'));

  check(value.cardinality() == 3, "Set must not contain duplicate elements");
  check(value == parse("{a,b,c}"),
        "Adding an existing element should not change the set");
}

void testRemoveElement() {
  Set value = parse("{a,b,{c,d}}");

  check(value.remove(Element('b')),
        "remove() should return true when atom exists");
  check(!value.remove(Element('b')),
        "remove() should return false when atom is absent");
  check(value == parse("{a,{c,d}}"),
        "remove() should remove only the requested atom");
  check(value.remove(parseElement("{c,d}")),
        "remove() should remove a nested set");
  check(value == parse("{a}"), "Nested set should be removed as one element");
}

void testMembership() {
  Set value = parse("{a,{b,c}}");

  check(value[Element('a')], "Membership should find an atom");
  check(value[parseElement("{b,c}")], "Membership should find a nested set");
  check(!value[Element('x')], "Membership should reject an absent atom");
  check(!value[parseElement("{b}")], "A different nested set should not match");
}

void testSetEqualityIsUnordered() {
  Set first = parse("{a,b,c}");
  Set sameElements = parse("{c,a,b}");
  Set different = parse("{a,b,d}");

  check(first == sameElements,
        "Set equality must not depend on insertion order");
  check(!(first == different),
        "Sets with different elements must not be equal");
  check(!(parse("{a}") == parse("{{a}}")),
        "Atom and singleton nested set are different elements");
  check(parse("{a,{b,c}}") == parse("{{c,b},a}"),
        "Nested set equality should be recursive and unordered");
}

void testUnionOperators() {
  Set first = parse("{a,b}");
  Set second = parse("{b,c}");
  Set firstOriginal(first);
  Set secondOriginal(second);

  Set result = first + second;
  check(result == parse("{a,b,c}"), "Binary + should return the union");
  check(first == firstOriginal, "Binary + must not modify its left operand");
  check(second == secondOriginal, "Binary + must not modify its right operand");

  first += second;
  check(first == parse("{a,b,c}"),
        "+= should update the left operand to the union");

  first += first;
  check(first == parse("{a,b,c}"), "Self union should not add duplicates");
}
void testStringFormatting() {
  Set atoms = parse("{a,b,c}");

  check(atoms.toString() == "{a,b,c}",
        "String formatting should correctly represent atomic elements");

  Set nested = parse("{a,{b,c},{}}");

  check(nested.toString() == "{a,{b,c},{}}",
        "String formatting should handle nested and empty sets");
}
void testIntersectionOperators() {
  Set first = parse("{a,b,c}");
  Set second = parse("{b,c,d}");
  Set firstOriginal(first);
  Set secondOriginal(second);

  Set result = first * second;
  check(result == parse("{b,c}"), "Binary * should return the intersection");
  check(first == firstOriginal, "Binary * must not modify its left operand");
  check(second == secondOriginal, "Binary * must not modify its right operand");

  first *= second;
  check(first == parse("{b,c}"),
        "*= should update the left operand to the intersection");

  first *= first;
  check(first == parse("{b,c}"), "Self intersection should preserve the set");
}

void testDifferenceOperators() {
  Set first = parse("{a,b,c}");
  Set second = parse("{b,c,d}");
  Set firstOriginal(first);
  Set secondOriginal(second);

  Set result = first - second;
  check(result == parse("{a}"), "Binary - should return the difference");
  check(first == firstOriginal, "Binary - must not modify its left operand");
  check(second == secondOriginal, "Binary - must not modify its right operand");

  first -= second;
  check(first == parse("{a}"),
        "-= should update the left operand to the difference");

  first -= first;
  check(first.isEmpty(),
        "Subtracting a set from itself should yield the empty set");
}

void testPowerSet() {
  Set value = parse("{a,b}");
  Set result = value.powerSet();
  Set expected = parse("{{},{a},{b},{a,b}}");

  check(result.cardinality() == 4,
        "Bulean of a two-element set must have four subsets");
  check(result == expected,
        "Bulean must contain every subset, including the empty set");
  check(value == parse("{a,b}"),
        "Building the bulean must not modify the source set");

  Set emptyPowerSet = parse("{}").powerSet();
  check(emptyPowerSet.cardinality() == 1,
        "Bulean of empty set must contain one subset");
  check(emptyPowerSet[parseElement("{}")],
        "Bulean of empty set must contain the empty set");
}

void testCopyingNestedElements() {
  Set original = parse("{a,{b,c}}");
  Set copied(original);

  check(copied == original, "Copy construction should preserve set contents");
  check(copied.remove(parseElement("{b,c}")),
        "Copied set should allow removing nested element");
  check(original[parseElement("{b,c}")],
        "Removing from a copy must not alter the original");

  Element source = parseElement("{x,y}");
  Element destination('z');
  destination = source;
  check(destination == source,
        "Element copy assignment should copy nested sets");
  destination = destination;
  check(destination == source, "Self-assignment should preserve an Element");
}

void testInvalidParserInput() {
  const std::vector<std::string> invalidSets = {
      "", "a", "{a,,b}", "{a,}", "{a,{b}", "{a,b}}", "{a b}"};

  for (const std::string &expression : invalidSets)
    expectInvalidSet(expression);

  expectInvalidElement("");
  expectInvalidElement("ab");
  expectInvalidElement("{a,}");
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
  runTest("empty set", testEmptySet);
  runTest("parsing nested sets", testParsingNestedSets);
  runTest("set uniqueness", testDuplicateElementsAreNotAdded);
  runTest("remove element", testRemoveElement);
  runTest("membership", testMembership);
  runTest("unordered equality", testSetEqualityIsUnordered);
  runTest("union operators", testUnionOperators);
  runTest("intersection operators", testIntersectionOperators);
  runTest("difference operators", testDifferenceOperators);
  runTest("power set", testPowerSet);
  runTest("string formatting", testStringFormatting);
  runTest("copying nested elements", testCopyingNestedElements);
  runTest("invalid parser input", testInvalidParserInput);

  std::cout << "\nPassed: " << passedTests << "\nFailed: " << failedTests
            << '\n';

  return failedTests == 0 ? 0 : 1;
}
