#include <vector>
class Student {
public:
  int id;
};
class Group {
private:
  std::vector<Student> students;

public:
  int number;
  void addStudent(const Student &student) { students.push_back(student); }
  bool removeStudent(int idEx) {
    for (auto curr = students.begin(); curr != students.end(); curr++) {
      if (curr->id == idEx) {
        students.erase(curr);
        return true;
      }
    }
    return false;
  }
};

class Worker {
protected:
  int mobNumber;

public:
  char Name;
  int giveNum(const char Name);
};
class Dean : Worker {
private:
};
class Bugaltery {};
class DeanOffice {
private:
  std::vector<Group> groups;
  Bugaltery;
  std::vector<Worker> workers;

public:
  bool addStudent(int groupNum, const Student &student) {
    for (Group &group : groups) {
      if (group.number == groupNum) {
        group.addStudent(student);
        return true;
      }
    }
    return false;
  }
  bool removeStudent(int idEx) {
    for (Group &group : groups) {
      if (group.removeStudent(idEx)) {
        return true;
      }
    }
    return false;
  }
};
