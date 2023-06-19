CXX=g++
OBJs = test.o \
simenginebj.o \
shoe.o fisheryates.o \
agent.o

test : $(OBJs)
	$(CXX) -o test $(OBJs)

test.o : test.cpp shoe.hpp types.hpp
	$(CXX) -c test.cpp

shoe.o : shoe.cpp shoe.hpp fisheryates.hpp types.hpp 
	$(CXX) -c shoe.cpp

fisheryates.o : fisheryates.cpp fisheryates.hpp types.hpp
	$(CXX) -c fisheryates.cpp

simenginebj.o : simenginebj.cpp simenginebj.hpp shoe.hpp agent.hpp strategyinput.hpp types.hpp
	$(CXX) -c simenginebj.cpp

agent.o : agent.cpp agent.hpp types.hpp
	$(CXX) -c agent.cpp

.PHONY : clean
clean :
	del test.exe $(OBJs)
	
