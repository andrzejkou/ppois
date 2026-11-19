# PPOIS

Репозиторий лабораторных работ по дисциплине **ППОИС**.

## Содержание репозитория

- `lab1/` — первая лабораторная работа.
- `lab2/` — объектная модель предметной области «Деканат».

## Лабораторная работа № 2 — «Деканат»

### Назначение

Проект моделирует работу деканата: учёт студентов и учебных групп, управление сотрудниками и кураторами, специальностями и учебными планами, расписанием занятий, успеваемостью, заявлениями, стипендиями и финансовыми операциями.

Реализация выполнена на **C++17** с разделением объявлений и реализаций классов. Заголовочные файлы находятся в `lab2/include/`, реализации — в `lab2/src/`.

### Структура `lab2`

```text
lab2/
├── CMakeLists.txt
├── main.cpp
├── include/       # объявления классов и шаблонные функции
├── src/           # реализации классов
├── tests/         # модульные тесты GoogleTest
├── build/         # локальная директория сборки (не включается в Git)
├── METHODS_100.md # реестр 100 содержательных методов
├── HEADER_MAP.md  # карта заголовочных файлов
└── TESTING.md     # подробная инструкция по сборке и тестированию
```

### Модель предметной области

Формат таблицы: **Класс | число полей | число методов | связанные классы**.

В число методов включены объявленные публичные методы, конструкторы, операторы и методы доступа (геттеры/сеттеры). Унаследованные методы повторно не учитываются. Связанные классы указаны по типам полей, параметров и возвращаемых значений методов; повторяющиеся связи одного класса учитываются один раз.

| Класс | Поля | Методы | Ассоциации (связанные классы) |
| --- | ---: | ---: | --- |
| `AcademicOrder` | 6 | 11 | `Dean` |
| `AcademicWork` | 4 | 9 | — |
| `AccountingDepartment` | 3 | 20 | `FinancialReport`, `PaymentRecord`, `Student` |
| `Assignment` | 3 | 4 | — |
| `Benefit` | 2 | 3 | — |
| `Classroom` | 2 | 4 | — |
| `ClassroomUnavailableException` | 0 | 1 | `DeanOfficeException` |
| `Company` | 3 | 7 | — |
| `Consultation` | 1 | 3 | `Classroom`, `Exam`, `Subject` |
| `Contract` | 2 | 3 | `Company` |
| `CourseProjCheck` | 0 | 2 | `Lesson` |
| `CourseProject` | 2 | 5 | — |
| `Curator` | 4 | 21 | `Group` |
| `Curriculum` | 3 | 6 | — |
| `Day` | 3 | 10 | `Lesson` |
| `Dean` | 2 | 8 | `AcademicOrder` |
| `DeanOffice` | 5 | 33 | `Group`, `Specialty`, `Student`, `Worker` |
| `DeanOfficeException` | 0 | 1 | — |
| `DuplicateStudentException` | 0 | 1 | `DeanOfficeException` |
| `Evaluation` | 2 | 4 | — |
| `Exam` | 1 | 4 | `Classroom`, `Subject` |
| `ExpulsionOrder` | 2 | 3 | — |
| `FinancialOperationException` | 0 | 1 | `DeanOfficeException` |
| `FinancialReport` | 5 | 9 | — |
| `Group` | 7 | 26 | `Student` |
| `GroupCapacityException` | 0 | 1 | `DeanOfficeException` |
| `InvalidAcademicWorkException` | 0 | 1 | `DeanOfficeException` |
| `InvalidCurriculumException` | 0 | 1 | `DeanOfficeException` |
| `InvalidGradeException` | 0 | 1 | `DeanOfficeException` |
| `InvalidGroupException` | 0 | 1 | `DeanOfficeException` |
| `InvalidPersonException` | 0 | 1 | `DeanOfficeException` |
| `InvalidScheduleException` | 0 | 1 | `DeanOfficeException` |
| `InvalidScholarshipException` | 0 | 1 | `DeanOfficeException` |
| `InvalidStudentException` | 0 | 1 | `DeanOfficeException` |
| `LaboratoryWork` | 1 | 3 | — |
| `Lecture` | 0 | 2 | `Lesson` |
| `Lesson` | 6 | 8 | `Classroom`, `Subject` |
| `Mark` | 3 | 4 | `Day`, `Subject` |
| `PaymentRecord` | 8 | 15 | — |
| `People` | 4 | 15 | — |
| `Perezdacha` | 1 | 3 | `Classroom`, `Subject` |
| `Premy` | 2 | 5 | — |
| `Schedule` | 4 | 16 | `Week` |
| `Scholarship` | 0 | 2 | — |
| `ScholarshipApplication` | 5 | 8 | — |
| `ScientificWork` | 0 | 1 | `AcademicWork` |
| `Secretary` | 1 | 5 | — |
| `Semester` | 3 | 5 | `Subject` |
| `Seminar` | 0 | 2 | `Lesson` |
| `Specialty` | 5 | 6 | `Curriculum` |
| `SpecialtyMismatchException` | 0 | 1 | `DeanOfficeException` |
| `Student` | 16 | 47 | `Benefit`, `Mark`, `Scholarship`, `ScholarshipApplication`, `StudentApplication`, `Submission` |
| `StudentApplication` | 3 | 5 | — |
| `StudyProgram` | 3 | 5 | `Specialty` |
| `Subject` | 3 | 4 | — |
| `Submission` | 6 | 12 | — |
| `SubmissionDeadlineException` | 0 | 1 | `DeanOfficeException` |
| `Teacher` | 7 | 22 | `Subject` |
| `TransferOrder` | 3 | 4 | — |
| `Week` | 2 | 9 | `Day` |
| `Worker` | 2 | 6 | — |

### Пользовательские исключения (15)

Все специализированные исключения предметной области наследуются от `DeanOfficeException`.

- `ClassroomUnavailableException`
- `DeanOfficeException`
- `DuplicateStudentException`
- `FinancialOperationException`
- `GroupCapacityException`
- `InvalidAcademicWorkException`
- `InvalidCurriculumException`
- `InvalidGradeException`
- `InvalidGroupException`
- `InvalidPersonException`
- `InvalidScheduleException`
- `InvalidScholarshipException`
- `InvalidStudentException`
- `SpecialtyMismatchException`
- `SubmissionDeadlineException`

### Итоговая статистика

| Показатель | Значение |
| --- | ---: |
| Классы | 61 |
| Поля | 150 |
| Объявленные публичные методы | 424 |
| Содержательные поведения | 100 |
| Ассоциации между классами | 54 |
| Пользовательские исключения | 15 |

**Подсчёт содержательных поведений.** Для выполнения требования по количеству уникальных поведений ведётся отдельный реестр `lab2/METHODS_100.md`. В нём перечислены 100 предметных операций. Конструкторы, деструкторы, простые геттеры/сеттеры и форматированный вывод в эти 100 поведений не включаются. Сам реестр является консервативным перечнем методов-кандидатов; окончательная оценка соответствия критериям остаётся за преподавателем.

### Сборка и запуск

Зависимости для Arch Linux:

```bash
sudo pacman -S cmake gcc gtest gcovr
```

Сборка из корня репозитория:

```bash
cmake -S lab2 -B lab2/build -DCMAKE_BUILD_TYPE=Debug
cmake --build lab2/build --parallel
```

Запуск приложения:

```bash
./lab2/build/dekanat_app
```

Запуск модульных тестов:

```bash
ctest --test-dir lab2/build --output-on-failure
```

### Покрытие тестами

Для сборки с инструментированием покрытия GCC:

```bash
cmake -S lab2 -B lab2/build-coverage \
  -DCMAKE_BUILD_TYPE=Debug \
  -DBUILD_TESTING=ON \
  -DENABLE_COVERAGE=ON

cmake --build lab2/build-coverage --parallel
ctest --test-dir lab2/build-coverage --output-on-failure
cmake --build lab2/build-coverage --target coverage_report
cmake --build lab2/build-coverage --target coverage_check
```

HTML- и XML-отчёты создаются в `lab2/build-coverage/`.

Подробности, включая запуск отдельного теста и описание порога покрытия, приведены в [`lab2/TESTING.md`](lab2/TESTING.md).

### Статический анализ

Для локальной проверки Cppcheck (если он установлен):

```bash
cppcheck \
  --enable=warning,performance,portability \
  --language=c++ \
  --std=c++17 \
  --suppress=missingIncludeSystem \
  -I lab2/include \
  lab2/main.cpp lab2/src
```

### Дополнительная документация

- [`lab2/METHODS_100.md`](lab2/METHODS_100.md) — реестр содержательных методов.
- [`lab2/CHANGES_METHODS.md`](lab2/CHANGES_METHODS.md) — описание дополнений предметной логики.
- [`lab2/HEADER_MAP.md`](lab2/HEADER_MAP.md) — разделение заголовочных файлов.
- [`lab2/CPP_AUDIT_REPORT.md`](lab2/CPP_AUDIT_REPORT.md) — результаты аудита реализаций.
- [`lab2/TESTING.md`](lab2/TESTING.md) — сборка, тестирование и покрытие.

## Сборка и проверка

Сборка и тестирование лабораторных работ выполняются через CMake/CTest и GitHub Actions. Подробные команды для конкретной лабораторной работы находятся в её каталоге.
