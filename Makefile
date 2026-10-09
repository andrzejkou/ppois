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

clean:
	rm -f main.o Vocabulary.o Set.o parser.o main

.PHONY: run clean
