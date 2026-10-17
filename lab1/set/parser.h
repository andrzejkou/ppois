
#pragma once

#include "Set.h"
#include <string>

/**
 * @brief Содержит функции разбора строкового представления множеств.
 */
namespace set_parser {

/**
 * @brief Разбирает строку и создаёт множество.
 * @param input Строка с представлением множества.
 * @return Множество, полученное в результате разбора.
 */
Set parse(const std::string &input);

/**
 * @brief Разбирает строку и создаёт элемент.
 * @param input Строка с представлением элемента.
 * @return Элемент, полученный в результате разбора.
 */
Element parseElement(const std::string &input);

} // namespace set_parser
