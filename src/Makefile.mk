CC = g++

test : test.o simengine.o
	cc -o checkers consttypes.o functions.o main.o

simengine.o : simengine.cpp simengine.hpp types.hpp
	cc -c simengine.cpp

test.o : test.cpp simengine.hpp types.hpp

.PHONY : clean
clean :
	del test.exe test.o simengine.o
	