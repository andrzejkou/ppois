#include "parser.h"

#include <cctype>
#include <stdexcept>
#include <string>

namespace {
void skipSpaces(const std::string &input, size_t &position) {
  while (position < input.size() &&
         std::isspace(static_cast<unsigned char>(input[position]))) {
    ++position;
  }
}

bool validateSet(const std::string &input, size_t &position) {
  skipSpaces(input, position);

  if (position >= input.size() || input[position] != '{')
    return false;

  ++position;
  skipSpaces(input, position);

  if (position < input.size() && input[position] == '}') {
    ++position;
    return true;
  }

  while (position < input.size()) {
    if (input[position] == '{') {
      if (!validateSet(input, position))
        return false;
    } else {
      if (input[position] == ',' || input[position] == '}')
        return false;

      ++position;
    }

    skipSpaces(input, position);

    if (position >= input.size())
      return false;

    if (input[position] == '}') {
      ++position;
      return true;
    }

    if (input[position] != ',')
      return false;

    ++position;
    skipSpaces(input, position);

    if (position >= input.size() || input[position] == '}' ||
        input[position] == ',') {
      return false;
    }
  }

  return false;
}

bool isValidSet(const std::string &input) {
  size_t position = 0;

  if (!validateSet(input, position))
    return false;

  skipSpaces(input, position);
  return position == input.size();
}

Set parseSetAt(const std::string &input, size_t &position) {
  skipSpaces(input, position);
  ++position; // Пропускаем открывающую скобку.

  Set result;
  skipSpaces(input, position);

  if (input[position] == '}') {
    ++position;
    return result;
  }

  while (true) {
    skipSpaces(input, position);

    if (input[position] == '{') {
      Set nestedSet = parseSetAt(input, position);
      result.add(Element(new Set(nestedSet)));
    } else {
      result.add(Element(input[position]));
      ++position;
    }

    skipSpaces(input, position);

    if (input[position] == '}') {
      ++position;
      return result;
    }

    ++position; // Пропускаем запятую.
  }
}

std::string trim(const std::string &input) {
  const size_t first = input.find_first_not_of(" \t\n\r");

  if (first == std::string::npos)
    return {};

  const size_t last = input.find_last_not_of(" \t\n\r");
  return input.substr(first, last - first + 1);
}
} // namespace

Set set_parser::parse(const std::string &input) {
  if (!isValidSet(input))
    throw std::invalid_argument("Некорректная запись множества");

  size_t position = 0;
  return parseSetAt(input, position);
}

Element set_parser::parseElement(const std::string &input) {
  const std::string expression = trim(input);

  if (expression.empty())
    throw std::invalid_argument("Элемент не может быть пустым");

  if (expression.front() == '{')
    return Element(new Set(parse(expression)));

  if (expression.size() == 1 && expression.front() != '}' &&
      expression.front() != ',') {
    return Element(expression.front());
  }

  throw std::invalid_argument("Некорректная запись элемента");
}
