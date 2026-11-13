#pragma once

#include <string>

class People {
public:
  People(std::string fullName, int age);
  People(People &&other) noexcept;
  People(const People &) = default;
  People &operator=(const People &) = default;
  People &operator=(People &&) noexcept = default;
  virtual ~People() = default;
  virtual void printInfo() const;
  const std::string &fullName() const;
  int age() const;

  virtual std::string role() const;
  void changeName(std::string newName);

protected:
  std::string fullName_;
  int age_;
};
