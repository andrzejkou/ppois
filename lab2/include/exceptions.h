#pragma once

#include <stdexcept>
#include <string>
#include <utility>

class DeanOfficeException : public std::runtime_error {
public:
  explicit DeanOfficeException(const std::string &message)
      : std::runtime_error(message) {}
};

class InvalidPersonException : public DeanOfficeException {
public:
  explicit InvalidPersonException(const std::string &message)
      : DeanOfficeException(message) {}
};
class InvalidStudentException : public DeanOfficeException {
public:
  explicit InvalidStudentException(const std::string &message)
      : DeanOfficeException(message) {}
};
class InvalidGroupException : public DeanOfficeException {
public:
  explicit InvalidGroupException(const std::string &message)
      : DeanOfficeException(message) {}
};
class GroupCapacityException : public DeanOfficeException {
public:
  explicit GroupCapacityException(const std::string &message)
      : DeanOfficeException(message) {}
};
class DuplicateStudentException : public DeanOfficeException {
public:
  explicit DuplicateStudentException(const std::string &message)
      : DeanOfficeException(message) {}
};
class SpecialtyMismatchException : public DeanOfficeException {
public:
  explicit SpecialtyMismatchException(const std::string &message)
      : DeanOfficeException(message) {}
};
class InvalidCurriculumException : public DeanOfficeException {
public:
  explicit InvalidCurriculumException(const std::string &message)
      : DeanOfficeException(message) {}
};
class InvalidScheduleException : public DeanOfficeException {
public:
  explicit InvalidScheduleException(const std::string &message)
      : DeanOfficeException(message) {}
};
class ClassroomUnavailableException : public DeanOfficeException {
public:
  explicit ClassroomUnavailableException(const std::string &message)
      : DeanOfficeException(message) {}
};
class InvalidAcademicWorkException : public DeanOfficeException {
public:
  explicit InvalidAcademicWorkException(const std::string &message)
      : DeanOfficeException(message) {}
};
class SubmissionDeadlineException : public DeanOfficeException {
public:
  explicit SubmissionDeadlineException(const std::string &message)
      : DeanOfficeException(message) {}
};
class InvalidGradeException : public DeanOfficeException {
public:
  explicit InvalidGradeException(const std::string &message)
      : DeanOfficeException(message) {}
};
class InvalidScholarshipException : public DeanOfficeException {
public:
  explicit InvalidScholarshipException(const std::string &message)
      : DeanOfficeException(message) {}
};
class FinancialOperationException : public DeanOfficeException {
public:
  explicit FinancialOperationException(const std::string &message)
      : DeanOfficeException(message) {}
};
