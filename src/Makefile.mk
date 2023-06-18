CXX=g++
OBJs = test.o shoe.o fisheryates.o

test : $(OBJs)
	$(CXX) -o test $(OBJs)

shoe.o : shoe.cpp shoe.hpp fisheryates.hpp types.hpp 
	$(CXX) -c shoe.cpp

test.o : test.cpp shoe.hpp types.hpp
	$(CXX) -c test.cpp

fisheryates.o : fisheryates.cpp fisheryates.hpp types.hpp
	$(CXX) -c fisheryates.cpp

.PHONY : clean
clean :
	del test.exe $(OBJs)
	
