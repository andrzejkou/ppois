# Разделение заголовков lab2

Каждый класс вынесен в одноимённый заголовок. В заголовках оставлены только
необходимые стандартные подключения, прямые зависимости (наследование и
объекты-поля) и локальные предварительные объявления для ссылок/указателей.

Тематические заголовки (`people.h`, `schedule.h`, `works.h` и т. д.) сохранены
как совместимые агрегирующие заголовки: они только подключают новые файлы.

## Реализации
- `src/Worker.cpp` → `include/Worker.h`
- `src/Exam.cpp` → `include/Exam.h`
- `src/PaymentRecord.cpp` → `include/PaymentRecord.h`
- `src/FinancialReport.cpp` → `include/FinancialReport.h`
- `src/Consultation.cpp` → `include/Consultation.h`
- `src/Mark.cpp` → `include/Mark.h`
- `src/Perezdacha.cpp` → `include/Perezdacha.h`
- `src/Secretary.cpp` → `include/Secretary.h`
- `src/Lesson.cpp` → `include/Lesson.h`
- `src/Classroom.cpp` → `include/Classroom.h`
- `src/Company.cpp` → `include/Company.h`
- `src/Teacher.cpp` → `include/Teacher.h`
- `src/CourseProjCheck.cpp` → `include/CourseProjCheck.h`
- `src/Lecture.cpp` → `include/Lecture.h`
- `src/Subject.cpp` → `include/Subject.h`
- `src/AccountingDepartment.cpp` → `include/AccountingDepartment.h`
- `src/Group.cpp` → `include/Group.h`
- `src/Student.cpp` → `include/Student.h`
- `src/Scholarship.cpp` → `include/Scholarship.h`
- `src/People.cpp` → `include/People.h`
- `src/Seminar.cpp` → `include/Seminar.h`
- `src/ScholarshipApplication.cpp` → `include/ScholarshipApplication.h`
- `src/DeanOffice.cpp` → `include/DeanOffice.h`
- `src/Dean.cpp` → `include/Dean.h`
- `src/Benefit.cpp` → `include/Benefit.h`
- `src/Curator.cpp` → `include/Curator.h`
- `src/Premy.cpp` → `include/Premy.h`

`Box.h` оставлен отдельным заголовком шаблонных функций.
