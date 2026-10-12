.PHONY: all run test grade list clean

CXX      = g++
CXXFLAGS = -Wall --std=c++17

# Default: build and run YOUR program (the example main in MainProgram.cpp).
all: run

main: MainProgram.cpp
	$(CXX) $(CXXFLAGS) MainProgram.cpp -o main

run: main
	./main

# The grader #includes your MainProgram.cpp (without your main()).
grader: autograder.cpp MainProgram.cpp
	$(CXX) $(CXXFLAGS) autograder.cpp -o grader

# Full grade report: every check with its points and a total out of 100.
grade: grader
	./grader

test: grade

# Names of all checks (run one with: ./grader --test removeAt.swap)
list: grader
	./grader --list

clean:
	rm -f main grader main.exe grader.exe *.o
