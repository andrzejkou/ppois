
main: main.o Vocabulary.o Set.o parser.o
	g++ main.o Vocabulary.o Set.o parser.o -o main
main.o: main.cpp classHead.h parser.h
	g++ -c main.cpp -o main.o
Vocabulary.o: Vocabulary.cpp classHead.h
	g++ -c Vocabulary.cpp -o Vocabulary.o
Set.o: Set.cpp classHead.h
	g++ -c Set.cpp -o Set.o
parser.o: parser.cpp parser.h classHead.h
	g++ -c parser.cpp -o parser.o
run: main
	./main
tests/test_set: tests/test_set.cpp Set.cpp parser.cpp classHead.h parser.h
	g++ -std=c++17 -Wall -Wextra -Wpedantic -I. tests/test_set.cpp Set.cpp parser.cpp -o tests/test_set
tests/test_vocabulary: tests/test_vocabulary.cpp Vocabulary.cpp classHead.h
	g++ -std=c++17 -Wall -Wextra -Wpedantic -I. tests/test_vocabulary.cpp Vocabulary.cpp -o tests/test_vocabulary
test: tests/test_set tests/test_vocabulary
	./tests/test_set
	./tests/test_vocabulary
coverage:
	rm -rf build/coverage
	mkdir -p build/coverage
	g++ --coverage -O0 -g -std=c++17 -I. -c Set.cpp -o build/coverage/Set.o
	g++ --coverage -O0 -g -std=c++17 -I. -c parser.cpp -o build/coverage/parser.o
	g++ --coverage -O0 -g -std=c++17 -I. -c Vocabulary.cpp -o build/coverage/Vocabulary.o
	g++ --coverage -O0 -g -std=c++17 -I. -c tests/test_set.cpp -o build/coverage/test_set.o
	g++ --coverage -O0 -g -std=c++17 -I. -c tests/test_vocabulary.cpp -o build/coverage/test_vocabulary.o
	g++ --coverage build/coverage/test_set.o build/coverage/Set.o build/coverage/parser.o -o build/coverage/test_set
	g++ --coverage build/coverage/test_vocabulary.o build/coverage/Vocabulary.o -o build/coverage/test_vocabulary
	./build/coverage/test_set
	./build/coverage/test_vocabulary
	LC_ALL=C gcov -b -c -r -o build/coverage Set.cpp parser.cpp Vocabulary.cpp | tee build/coverage/summary.txt
	awk '/Lines executed:/ { percent = $$2; sub("executed:", "", percent); sub("%", "", percent); if ((percent + 0) <= 90) { print "Coverage below 90%: " $$0; failed = 1 } count++ } END { if (count == 0) { print "Could not read gcov line coverage summary"; exit 2 } if (failed) exit 1; print "All source files exceed 90% line coverage." }' build/coverage/summary.txt
clean:
	rm -f main.o Vocabulary.o Set.o parser.o main
	rm -f tests/test_set tests/test_vocabulary
	rm -rf build
	rm -f *.gcov *.gcda *.gcno *.info
.PHONY: run test coverage clean
.PHONY: test coverage clean


