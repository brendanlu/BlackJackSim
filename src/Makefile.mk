CXX=g++
OBJs = test.o simengine.o fisheryates.o

test : $(OBJs)
	$(CXX) -o test $(OBJs)

simengine.o : simengine.cpp simengine.hpp fisheryates.hpp types.hpp 
	$(CXX) -c simengine.cpp

test.o : test.cpp simengine.hpp types.hpp
	$(CXX) -c test.cpp

fisheryates.o : fisheryates.cpp fisheryates.hpp types.hpp
	$(CXX) -c fisheryates.cpp

.PHONY : clean
clean :
	del test.exe $(OBJs)
	
