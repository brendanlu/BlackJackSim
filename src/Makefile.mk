test : test.o simengine.o
	g++ -o test test.o simengine.o

simengine.o : simengine.cpp simengine.hpp types.hpp
	g++ -c simengine.cpp

test.o : test.cpp simengine.hpp types.hpp
	g++ -c test.cpp

.PHONY : clean
clean :
	del test.exe test.o simengine.o
	
