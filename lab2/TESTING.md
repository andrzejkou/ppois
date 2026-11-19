# Сборка и тестирование

## Зависимости (Arch Linux)

```bash
sudo pacman -S cmake gcc gtest gcovr
```

## Обычная сборка приложения и тестов

Из каталога `lab2`:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j$(nproc)
ctest --test-dir build --output-on-failure
```

## Сборка с покрытием

Для корректного измерения покрытия используйте отдельный каталог сборки:

```bash
cmake -S . -B build-coverage \
  -DCMAKE_BUILD_TYPE=Debug \
  -DENABLE_COVERAGE=ON
cmake --build build-coverage -j$(nproc)
ctest --test-dir build-coverage --output-on-failure
```

## Отчёт покрытия

```bash
cmake --build build-coverage --target coverage_report
```

Отчёты появятся в `build-coverage/coverage.html` и
`build-coverage/coverage.xml`.

## Проверка порога 90%

```bash
cmake --build build-coverage --target coverage_check
```

Цель `coverage_check` завершится ошибкой, если покрытие строк исходников
из `src/` будет ниже 90%. Это именно проверка результата, а не гарантия,
что текущий набор тестов уже достигает порога.

## Запуск отдельного теста

```bash
ctest --test-dir build-coverage -R StudentTest --output-on-failure
```

## Результат проверки на macOS

В `tests/test_uncovered_scenarios.cpp` добавлены 14 тестов GoogleTest.
После сборки Apple Clang все 112 тестов прошли через CTest.

Покрытие файлов `src/*.cpp` по `llvm-cov`:

| Метрика | До | После |
| --- | ---: | ---: |
| Строки | 99,00% | 100,00% |
| Функции | 99,21% | 100,00% |
| Ветви | 93,92% | 100,00% (672 из 672) |

В `DeanOffice::transferStudent` один вызов `findStudent` одновременно
проверяет наличие студента в исходной группе и возвращает его для перевода.
Тест проверяет отказ, когда студент числится в третьей группе, и сохранение
состава групп после отказа.

На macOS с установленным GoogleTest отчёт можно воспроизвести из каталога
`lab2`:

```bash
cmake -S . -B build-llvm -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="-fprofile-instr-generate -fcoverage-mapping"
cmake --build build-llvm --parallel
LLVM_PROFILE_FILE=build-llvm/dekanat.profraw ./build-llvm/dekanat_tests
xcrun llvm-profdata merge -sparse build-llvm/dekanat.profraw -o build-llvm/dekanat.profdata
xcrun llvm-cov report build-llvm/dekanat_tests \
  -instr-profile=build-llvm/dekanat.profdata "$PWD/src"
```

Цель `coverage_check` использует GCC и gcovr. Эту метрику после изменений
ещё предстоит измерить.
