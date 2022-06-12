all: Assignment3Part1 Assignment3Part2

Assignment3Part1: Assignment3Part1.cpp
	g++ -o Assignment3Part1 -pthread Assignment3Part1.cpp

Assignment3Part2: Assignment3Part2.cpp
	g++ -o Assignment3Part2 -pthread Assignment3Part2.cpp