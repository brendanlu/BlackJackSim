CXX=g++
OBJs = test.o \
shoe.o \
simenginebj.o \
agent.o 

test : $(OBJs)
	$(CXX) -o test $(OBJs)

test.o : test.cpp shoe.hpp types.hpp
	$(CXX) -c test.cpp

shoe.o : shoe.cpp shoe.hpp fisheryates.hpp types.hpp 
	$(CXX) -c shoe.cpp

simenginebj.o : simenginebj.cpp simenginebj.hpp shoe.hpp agent.hpp strategyinput.hpp types.hpp
	$(CXX) -c simenginebj.cpp

agent.o : agent.cpp agent.hpp types.hpp
	$(CXX) -c agent.cpp

# .PHONY : clean # run clean every time we make, might be redundant as we clunkily call it from Python already

# clean : \
	del test.exe $(OBJs)
	
