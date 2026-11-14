#include "AcademicOrder.h"
#include "AccountingDepartment.h"
#include "Classroom.h"
#include "Curator.h"
#include "Day.h"
#include "Dean.h"
#include "DeanOffice.h"
#include "Exam.h"
#include "FinancialReport.h"
#include "Group.h"
#include "Lecture.h"
#include "PaymentRecord.h"
#include "Schedule.h"
#include "Secretary.h"
#include "Seminar.h"
#include "Student.h"
#include "Subject.h"
#include "Teacher.h"
#include "Week.h"
#include "Worker.h"

#include <initializer_list>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>

// ---------- Общие функции ввода ----------

std::string readString(const std::string &prompt) {
  std::string value;

  while (true) {
    std::cout << prompt;

    if (!std::getline(std::cin, value)) {
      throw std::runtime_error("Ввод завершён.");
    }

    if (!value.empty()) {
      return value;
    }

    std::cout << "Поле не должно быть пустым.\n";
  }
}

int readInt(const std::string &prompt, int minValue = 0) {
  while (true) {
    const std::string input = readString(prompt);

    try {
      std::size_t pos = 0;
      const int value = std::stoi(input, &pos);

      if (pos == input.size() && value >= minValue) {
        return value;
      }
    } catch (const std::exception &) {
    }

    std::cout << "Введите целое число не меньше " << minValue << ".\n";
  }
}

double readDouble(const std::string &prompt, double minValue = 0.01) {
  while (true) {
    const std::string input = readString(prompt);

    try {
      std::size_t pos = 0;
      const double value = std::stod(input, &pos);

      if (pos == input.size() && value >= minValue) {
        return value;
      }
    } catch (const std::exception &) {
    }

    std::cout << "Введите число не меньше " << minValue << ".\n";
  }
}

int readChoice(int minValue, int maxValue) {
  while (true) {
    const int choice = readInt("Ваш выбор: ", minValue);

    if (choice <= maxValue) {
      return choice;
    }

    std::cout << "Нет такого пункта меню.\n";
  }
}

void printMenu(const std::string &title,
               const std::initializer_list<std::string> &items) {
  std::cout << "\n========== " << title << " ==========\n";

  int number = 1;

  for (const auto &item : items) {
    std::cout << number++ << ". " << item << '\n';
  }

  std::cout << "0. Назад\n";
}

// ============================================================
// РАЗДЕЛ 1. СТУДЕНТЫ И УЧЕБНЫЕ ГРУППЫ
// ============================================================

void createGroup(DeanOffice &office) {
  const std::string code = readString("Код группы: ");
  const std::string specialty = readString("Код специальности: ");
  const int capacity = readInt("Вместимость группы: ", 1);

  if (office.findGroup(code) != nullptr) {
    std::cout << "Группа с таким кодом уже существует.\n";
    return;
  }

  office.addGroup(Group(code, specialty, static_cast<std::size_t>(capacity)));

  std::cout << "Группа создана.\n";
}

void enrollStudent(DeanOffice &office) {
  const std::string groupCode = readString("Код группы: ");

  if (office.findGroup(groupCode) == nullptr) {
    std::cout << "Группа не найдена.\n";
    return;
  }

  const std::string fullName = readString("ФИО: ");
  const int age = readInt("Возраст: ", 1);
  const std::string studentId = readString("Номер студенческого билета: ");
  const int year = readInt("Курс: ", 1);

  Student student(fullName, age, studentId, year);

  if (office.enrollStudent(groupCode, student)) {
    std::cout << "Студент зачислен.\n";
  } else {
    std::cout << "Не удалось зачислить студента. "
                 "Проверьте его ID и места в группе.\n";
  }
}

void showGroup(DeanOffice &office) {
  const std::string code = readString("Код группы: ");

  const Group *group = office.findGroup(code);

  if (group == nullptr) {
    std::cout << "Группа не найдена.\n";
    return;
  }

  std::cout << "\nГруппа: " << group->code()
            << "\nСпециальность: " << group->specialtyCode()
            << "\nСтудентов: " << group->size()
            << "\nСвободных мест: " << group->availableSeats() << '\n';

  if (group->students().empty()) {
    std::cout << "Список студентов пуст.\n";
    return;
  }

  for (const auto &student : group->students()) {
    std::cout << "-------------------------\n";
    student->printInfo();
  }
}

void transferStudent(DeanOffice &office) {
  const std::string from = readString("Исходная группа: ");
  const std::string to = readString("Целевая группа: ");
  const std::string id = readString("Номер студенческого билета: ");

  if (office.transferStudent(from, to, id)) {
    std::cout << "Студент переведён.\n";
  } else {
    std::cout << "Перевод не выполнен. "
                 "Проверьте группы, студента и свободные места.\n";
  }
}

void studentSection(DeanOffice &office) {
  while (true) {
    printMenu("СТУДЕНТЫ И ГРУППЫ",
              {"Создать группу", "Зачислить студента",
               "Показать группу и её студентов",
               "Перевести студента в другую группу", "Отчислить студента",
               "Удалить студента только из группы",
               "Назначить стипендию студенту",
               "Показать общее количество студентов"});

    switch (readChoice(0, 8)) {
    case 0:
      return;

    case 1:
      createGroup(office);
      break;

    case 2:
      enrollStudent(office);
      break;

    case 3:
      showGroup(office);
      break;

    case 4:
      transferStudent(office);
      break;

    case 5: {
      const std::string id = readString("Номер студенческого билета: ");

      std::cout << (office.expelStudent(id) ? "Студент отчислен.\n"
                                            : "Студент не найден в группах.\n");

      break;
    }

    case 6: {
      const std::string group = readString("Код группы: ");

      const std::string id = readString("Номер студенческого билета: ");

      std::cout << (office.removeFromGroup(group, id)
                        ? "Студент удалён из группы, но остаётся в реестре.\n"
                        : "Не удалось удалить студента из группы.\n");

      break;
    }

    case 7: {
      const std::string id = readString("Номер студенческого билета: ");

      try {
        office.giveScholarship(id);
        std::cout << "Стипендия назначена.\n";
      } catch (const std::exception &e) {
        std::cout << "Операция не выполнена: " << e.what() << '\n';
      }

      break;
    }

    case 8:
      std::cout << "Студентов в группах: " << office.totalStudents() << '\n';
      break;
    }
  }
}

// ============================================================
// РАЗДЕЛ 2. СОТРУДНИКИ ДЕКАНАТА
// ============================================================

void addWorker(DeanOffice &office) {
  std::cout << "\nТип сотрудника:\n"
            << "1. Декан\n"
            << "2. Преподаватель\n"
            << "3. Секретарь\n"
            << "4. Куратор\n";

  const int type = readChoice(0, 4);

  if (type == 0) {
    return;
  }

  const std::string name = readString("ФИО: ");
  const int age = readInt("Возраст: ", 1);
  const std::string id = readString("Табельный номер: ");

  std::shared_ptr<Worker> worker;

  switch (type) {
  case 1: {
    const std::string position = readString("Должность: ");

    worker = std::make_shared<Dean>(name, age, id, position);

    break;
  }

  case 2: {
    const std::string department = readString("Кафедра: ");

    worker = std::make_shared<Teacher>(name, age, id, department);

    break;
  }

  case 3:
    worker = std::make_shared<Secretary>(name, age, id);
    break;

  case 4: {
    const std::string groupCode = readString("Код курируемой группы: ");

    const Group *group = office.findGroup(groupCode);

    if (group == nullptr) {
      std::cout << "Сначала создайте группу.\n";
      return;
    }

    worker = std::make_shared<Curator>(name, age, id, *group);

    break;
  }

  default:
    return;
  }

  office.addWorker(worker);

  std::cout << "Сотрудник добавлен.\n";
}

void showWorkers(const DeanOffice &office) {
  if (office.workers().empty()) {
    std::cout << "Сотрудников пока нет.\n";
    return;
  }

  for (const auto &worker : office.workers()) {
    std::cout << "-------------------------\n";

    worker->printInfo();

    std::cout << "Роль: " << worker->role() << '\n';
  }
}

void workerSection(DeanOffice &office) {
  while (true) {
    printMenu("СОТРУДНИКИ",
              {"Добавить сотрудника", "Показать всех сотрудников",
               "Найти сотрудника по табельному номеру", "Удалить сотрудника",
               "Изменить должность", "Назначить преподавателю дисциплину",
               "Зарегистрировать документ секретарём",
               "Утвердить академический приказ деканом",
               "Показать количество сотрудников"});

    switch (readChoice(0, 9)) {
    case 0:
      return;

    case 1:
      try {
        addWorker(office);
      } catch (const std::exception &e) {
        std::cout << "Не удалось добавить сотрудника: " << e.what() << '\n';
      }
      break;

    case 2:
      showWorkers(office);
      break;

    case 3: {
      const std::string id = readString("Табельный номер: ");

      const Worker *worker = office.findWorker(id);

      if (worker) {
        worker->printInfo();
      } else {
        std::cout << "Сотрудник не найден.\n";
      }

      break;
    }

    case 4: {
      const std::string id = readString("Табельный номер: ");

      std::cout << (office.removeWorker(id) ? "Сотрудник удалён.\n"
                                            : "Сотрудник не найден.\n");

      break;
    }

    case 5: {
      const std::string id = readString("Табельный номер: ");

      Worker *worker = office.findWorker(id);

      if (!worker) {
        std::cout << "Сотрудник не найден.\n";
        break;
      }

      const std::string position = readString("Новая должность: ");

      worker->changePosition(position);

      std::cout << "Должность изменена.\n";
      break;
    }

    case 6: {
      const std::string id = readString("Табельный номер преподавателя: ");

      auto *teacher = dynamic_cast<Teacher *>(office.findWorker(id));

      if (!teacher) {
        std::cout << "Преподаватель не найден.\n";
        break;
      }

      const std::string course = readString("Название дисциплины: ");

      teacher->assignCourse(course);

      std::cout << "Дисциплина назначена.\n";
      break;
    }

    case 7: {
      const std::string id = readString("Табельный номер секретаря: ");

      auto *secretary = dynamic_cast<Secretary *>(office.findWorker(id));

      if (!secretary) {
        std::cout << "Секретарь не найден.\n";
        break;
      }

      const std::string document = readString("Название документа: ");

      std::cout << secretary->registerDocument(document) << '\n';

      break;
    }

    case 8: {
      const std::string id = readString("Табельный номер декана: ");

      auto *dean = dynamic_cast<Dean *>(office.findWorker(id));

      if (!dean) {
        std::cout << "Декан не найден.\n";
        break;
      }

      const int orderNumber = readInt("Номер приказа: ", 1);

      const std::string orderTitle = readString("Название приказа: ");

      AcademicOrder order(orderNumber, orderTitle);

      dean->approveOrder(order);

      std::cout << (order.isApproved() ? "Приказ утверждён.\n"
                                       : "Приказ не утверждён.\n");

      break;
    }

    case 9:
      std::cout << "Сотрудников: " << office.totalWorkers() << '\n';
      break;
    }
  }
}

// ============================================================
// РАЗДЕЛ 3. УЧЕБНЫЙ ПРОЦЕСС И РАСПИСАНИЕ
// ============================================================

void createSchedule(DeanOffice &office,
                    std::map<std::string, Schedule> &schedules) {
  const std::string groupCode = readString("Код группы: ");

  if (office.findGroup(groupCode) == nullptr) {
    std::cout << "Такой группы нет в деканате.\n";
    return;
  }

  auto scheduleIt = schedules.find(groupCode);

  if (scheduleIt == schedules.end()) {
    scheduleIt =
        schedules
            .emplace(std::piecewise_construct, std::forward_as_tuple(groupCode),
                     std::forward_as_tuple(groupCode))
            .first;
  }

  const int weekNumber = readInt("Номер учебной недели: ", 1);

  Week week(weekNumber);

  const int dayCount = readInt("Количество дней в этой неделе: ", 1);

  for (int d = 0; d < dayCount; ++d) {
    const std::string dayName = readString("День недели: ");

    const std::string date = readString("Дата: ");

    Day day(dayName, date);

    const int lessonCount = readInt("Количество занятий в этот день: ", 1);

    for (int i = 0; i < lessonCount; ++i) {
      std::cout << "\nЗанятие " << (i + 1)
                << " (1 — лекция, 2 — семинар, 3 — экзамен)\n";

      const int type = readChoice(1, 3);

      const std::string subjectCode = readString("Код дисциплины: ");

      const std::string subjectName = readString("Название дисциплины: ");

      const int credits = readInt("Количество зачётных единиц: ", 1);

      Subject subject(subjectCode, subjectName, credits);

      const std::string teacherId =
          readString("Табельный номер преподавателя: ");

      const int startHour = readInt("Час начала: ", 0);

      const int duration = readInt("Продолжительность в часах: ", 1);

      const std::string roomNumber = readString("Номер аудитории: ");

      const int roomCapacity = readInt("Вместимость аудитории: ", 1);

      Classroom classroom(roomNumber, roomCapacity);

      std::shared_ptr<Lesson> lesson;

      if (type == 1) {
        lesson = std::make_shared<Lecture>(subject, teacherId, dayName,
                                           startHour, duration, classroom);
      } else if (type == 2) {
        lesson = std::make_shared<Seminar>(subject, teacherId, dayName,
                                           startHour, duration, classroom);
      } else {
        const std::string examType = readString("Тип экзамена: ");

        lesson = std::make_shared<Exam>(subject, teacherId, dayName, startHour,
                                        duration, classroom, examType);
      }

      day.addLesson(lesson);
    }

    week.addDay(day);
  }

  scheduleIt->second.addWeek(week);

  std::cout << "Неделя добавлена в расписание группы.\n";
}

void studySection(DeanOffice &office,
                  std::map<std::string, Schedule> &schedules) {
  while (true) {
    printMenu("УЧЕБНЫЙ ПРОЦЕСС",
              {"Добавить неделю расписания для группы",
               "Показать количество занятий в расписании группы",
               "Изменить расписание группы (вызвать changeSchedule)",
               "Создать и утвердить академический приказ"});

    switch (readChoice(0, 4)) {
    case 0:
      return;

    case 1:
      try {
        createSchedule(office, schedules);
      } catch (const std::exception &e) {
        std::cout << "Не удалось добавить расписание: " << e.what() << '\n';
      }
      break;

    case 2: {
      const std::string code = readString("Код группы: ");

      const auto it = schedules.find(code);

      if (it == schedules.end()) {
        std::cout << "Расписание не найдено.\n";
      } else {
        std::cout << "Занятий в расписании: " << it->second.lessonCount()
                  << '\n';
      }

      break;
    }

    case 3: {
      const std::string code = readString("Код группы: ");

      const auto it = schedules.find(code);

      if (it == schedules.end()) {
        std::cout << "Расписание не найдено.\n";
      } else {
        it->second.changeSchedule();

        std::cout << "Метод изменения расписания выполнен.\n";
      }

      break;
    }

    case 4: {
      const std::string id = readString("Табельный номер декана: ");

      auto *dean = dynamic_cast<Dean *>(office.findWorker(id));

      if (!dean) {
        std::cout << "Декан не найден.\n";
        break;
      }

      const int orderNumber = readInt("Номер приказа: ", 1);

      const std::string orderTitle = readString("Название приказа: ");

      AcademicOrder order(orderNumber, orderTitle);

      dean->approveOrder(order);

      std::cout << (order.isApproved() ? "Приказ утверждён.\n"
                                       : "Приказ не утверждён.\n");

      break;
    }
    }
  }
}

// ============================================================
// РАЗДЕЛ 4. БУХГАЛТЕРИЯ
// ============================================================

void accountingSection(AccountingDepartment &accounting) {
  while (true) {
    printMenu("БУХГАЛТЕРИЯ",
              {"Зарегистрировать выплату", "Показать количество записей",
               "Показать сумму оплаченных записей",
               "Сформировать отчёт по расходам"});

    switch (readChoice(0, 4)) {
    case 0:
      return;

    case 1: {
      const std::string studentId = readString("ID студента: ");

      const double amount = readDouble("Сумма выплаты: ");

      const std::string purpose = readString("Назначение выплаты: ");

      const std::string date = readString("Дата: ");

      PaymentRecord record(studentId, amount, purpose, date);

      const int paid = readChoice(0, 1);

      if (paid == 1) {
        record.markPaid();
      }

      accounting.recordPayment(record);

      std::cout << "Запись добавлена.\n";
      break;
    }

    case 2:
      std::cout << "Количество записей: " << accounting.paymentCount() << '\n';
      break;

    case 3:
      std::cout << "Сумма оплаченных записей: " << accounting.totalPaid()
                << '\n';
      break;

    case 4: {
      const std::string period = readString("Период отчёта: ");

      const FinancialReport report = accounting.makeReport(period);

      std::cout << "Отчёт за период: " << report.period()
                << "\nРасходы по учтённым оплаченным записям: "
                << -report.balance() << '\n';

      break;
    }
    }
  }
}

// ============================================================
// ГЛАВНОЕ МЕНЮ И ТОЧКА ВХОДА
// ============================================================

int main() {
  try {
    DeanOffice office("Деканат");

    AccountingDepartment accounting("Бухгалтерия");

    std::map<std::string, Schedule> schedules;

    while (true) {
      std::cout << "\n========== ИНФОРМАЦИОННАЯ СИСТЕМА ==========\n"
                << "1. Студенты и группы\n"
                << "2. Сотрудники\n"
                << "3. Учебный процесс\n"
                << "4. Бухгалтерия\n"
                << "0. Выход\n";

      switch (readChoice(0, 4)) {
      case 0:
        std::cout << "Завершение работы.\n";
        return 0;

      case 1:
        studentSection(office);
        break;

      case 2:
        workerSection(office);
        break;

      case 3:
        studySection(office, schedules);
        break;

      case 4:
        accountingSection(accounting);
        break;
      }
    }
  } catch (const std::exception &e) {
    std::cerr << "Критическая ошибка: " << e.what() << '\n';

    return 1;
  }
}
