#pragma once
#include <algorithm>
#include <string>
#include <vector>

template <typename T> void updateField(T &field, T value) {
  field = std::move(value);
}
template <typename Container, typename Predicate>
auto findIn(Container &container, Predicate condition) {
  auto it = std::find_if(container.begin(), container.end(), condition);
  return it != container.end() ? &(*it) : nullptr;
}
template <typename Container, typename Predicate>

auto findIn(const Container &container, Predicate condition) {
  auto it = std::find_if(container.begin(), container.end(), condition);
  return it != container.end() ? &(*it) : nullptr;
}

template <typename Container, typename Predicate>
auto findAll(const Container &container, Predicate condition) {
  std::vector<typename Container::value_type *> result;
  auto it = container.begin();
  while (it != container.end()) {
    it = std::find_if(it, container.end(), condition);
    if (it != container.end()) {
      result.push_back(&(*it));
      it++;
    }
  }
  return result;
}

template <typename Container, typename Predicate, typename TrueAct,
          typename FalseAct>
void actionForMatches(Container &container, Predicate condition,
                      TrueAct actionT, FalseAct actionF) {
  auto it = container.begin();
  while (it != container.end()) {
    if (condition(*it)) {
      actionT(*it);

    } else
      actionF(*it);
    it++;
  }
}
