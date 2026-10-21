#include <string>
#include <vector>

class People {
public:
  int id;
  std::string name;
};

class Curriculum {
private:
public:
};
class Student : public People {
private:
};

class Group {
private:
  std::vector<Student> students;
  Curriculum curriculum;

public:
  int number;

  void addStudent(const Student &student) { students.push_back(student); }
  void checkurriculum() {}
  bool removeStudent(int idEx) {
    for (auto curr = students.begin(); curr != students.end(); ++curr) {
      if (curr->id == idEx) {
        students.erase(curr);
        return true;
      }
    }
    return false;
  }
};

class Specialty {
private:
  Group group;

public:
};

class Projects {};

class CourseProjects : Projects {};

class Worker : public People {
protected:
  std::string mobNumber;

public:
  int giveNum(const std::string name);
};

class Dean : public Worker {};

class Bugaltery {};

class DeanOffice {
private:
  std::vector<Group> groups;
  Bugaltery bugaltery;
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

  bool removeWorker(int workerId) {
    for (auto curr = workers.begin(); curr != workers.end(); ++curr) {
      if (curr->id == workerId) {
        workers.erase(curr);
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
