
main: main.o vocabulary/Vocabulary.o set/Set.o set/parser.o
	g++ main.o vocabulary/Vocabulary.o set/Set.o set/parser.o -o main

main.o: main.cpp set/Set.h set/parser.h vocabulary/Vocabulary.h
	g++ -std=c++17 -Wall -Wextra -Wpedantic -c main.cpp -o main.o

vocabulary/Vocabulary.o: vocabulary/Vocabulary.cpp vocabulary/Vocabulary.h
	g++ -std=c++17 -Wall -Wextra -Wpedantic -c vocabulary/Vocabulary.cpp -o vocabulary/Vocabulary.o

set/Set.o: set/Set.cpp set/Set.h
	g++ -std=c++17 -Wall -Wextra -Wpedantic -c set/Set.cpp -o set/Set.o

set/parser.o: set/parser.cpp set/parser.h set/Set.h
	g++ -std=c++17 -Wall -Wextra -Wpedantic -c set/parser.cpp -o set/parser.o

run: main
	./main

tests/test_set: tests/test_set.cpp set/Set.cpp set/Set.h set/parser.cpp set/parser.h
	g++ -std=c++17 -Wall -Wextra -Wpedantic -I. tests/test_set.cpp set/Set.cpp set/parser.cpp -o tests/test_set

tests/test_vocabulary: tests/test_vocabulary.cpp vocabulary/Vocabulary.cpp vocabulary/Vocabulary.h
	g++ -std=c++17 -Wall -Wextra -Wpedantic -I. tests/test_vocabulary.cpp vocabulary/Vocabulary.cpp -o tests/test_vocabulary

test: tests/test_set tests/test_vocabulary
	./tests/test_set
	./tests/test_vocabulary

coverage:
	rm -rf build/coverage
	mkdir -p build/coverage
	g++ --coverage -O0 -g -std=c++17 -I. -c set/Set.cpp -o build/coverage/Set.o
	g++ --coverage -O0 -g -std=c++17 -I. -c set/parser.cpp -o build/coverage/parser.o
	g++ --coverage -O0 -g -std=c++17 -I. -c vocabulary/Vocabulary.cpp -o build/coverage/Vocabulary.o
	g++ --coverage -O0 -g -std=c++17 -I. -c tests/test_set.cpp -o build/coverage/test_set.o
	g++ --coverage -O0 -g -std=c++17 -I. -c tests/test_vocabulary.cpp -o build/coverage/test_vocabulary.o
	g++ --coverage build/coverage/test_set.o build/coverage/Set.o build/coverage/parser.o -o build/coverage/test_set
	g++ --coverage build/coverage/test_vocabulary.o build/coverage/Vocabulary.o -o build/coverage/test_vocabulary
	./build/coverage/test_set
	./build/coverage/test_vocabulary
	LC_ALL=C gcov -b -c -r -o build/coverage set/Set.cpp set/parser.cpp vocabulary/Vocabulary.cpp | tee build/coverage/summary.txt
	awk '/Lines executed:/ { percent = $$2; sub("executed:", "", percent); sub("%", "", percent); if ((percent + 0) <= 90) { print "Coverage below 90%: " $$0; failed = 1 } count++ } END { if (count == 0) { print "Could not read gcov line coverage summary"; exit 2 } if (failed) exit 1; print "All source files exceed 90% line coverage." }' build/coverage/summary.txt

clean:
	rm -f main.o Vocabulary.o Set.o parser.o main
	rm -f vocabulary/Vocabulary.o set/Set.o set/parser.o
	rm -f tests/test_set tests/test_vocabulary
	rm -rf build
	rm -f *.gcov set/*.gcov vocabulary/*.gcov *.gcda *.gcno *.info

.PHONY: run test coverage clean
