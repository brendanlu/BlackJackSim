CXX=g++
test : test.o simengine.o
	$(CXX) -o test test.o simengine.o

simengine.o : simengine.cpp simengine.hpp types.hpp
	$(CXX) -c simengine.cpp

test.o : test.cpp simengine.hpp types.hpp
	$(CXX) -c test.cpp

.PHONY : clean
clean :
	del test.exe test.o simengine.o
	
