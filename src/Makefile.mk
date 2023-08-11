CXX=g++
CXXFLAGS=-O3 -march=native -Wall -Wextra

OBJs = test.o \
shoe.o \
simenginebj.o \
agent.o \
dealer.o

test : $(OBJs)
	$(CXX) $(CXXFLAGS) -o test $(OBJs)

test.o : test.cpp shoe.hpp types.hpp agent.hpp simenginebj.hpp
	$(CXX) $(CXXFLAGS) -c test.cpp

shoe.o : shoe.cpp shoe.hpp fisheryates.hpp types.hpp 
	$(CXX) $(CXXFLAGS) -c shoe.cpp

simenginebj.o : simenginebj.cpp simenginebj.hpp shoe.hpp agent.hpp strategyinput.hpp types.hpp
	$(CXX) $(CXXFLAGS) -c simenginebj.cpp

agent.o : agent.cpp agent.hpp types.hpp simenginebj.hpp strategyinput.hpp
	$(CXX) $(CXXFLAGS) -c agent.cpp

dealer.o : dealer.cpp dealer.hpp types.hpp
	$(CXX) $(CXXFLAGS) -c dealer.cpp

# .PHONY : clean # run clean every time we make, might be redundant as we clunkily call it from Python already

# clean : \
	del test.exe $(OBJs)
	
