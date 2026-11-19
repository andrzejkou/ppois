#pragma once

#include <utility>

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
  const std::string &email() const { return email_; }
  void setEmail(std::string value) { email_ = std::move(value); }
  const std::string &phone() const { return phone_; }
  void setPhone(std::string value) { phone_ = std::move(value); }

protected:
  std::string fullName_;
  int age_;
  std::string email_;
  std::string phone_;
};
