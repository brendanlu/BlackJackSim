# 
# Makefile for Windows to test just the C++ build
#
CXX = g++
CXXFLAGS = -O3 -march=native -Wall -Wextra -flto -fprefetch-loop-arrays -I../../e-socks/include -pg
LDFLAGS = -pg -lws2_32

OBJs = test.o shoe.o simenginebj.o agent.o dealer.o

TARGET = test

all: $(TARGET)

$(TARGET): $(OBJs)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJs) $(LDFLAGS)

test.o: test.cpp shoe.hpp card.hpp agent.hpp simenginebj.hpp
	$(CXX) $(CXXFLAGS) -c test.cpp

shoe.o: shoe.cpp shoe.hpp fisheryates.hpp card.hpp 
	$(CXX) $(CXXFLAGS) -c shoe.cpp

simenginebj.o: simenginebj.cpp simenginebj.hpp shoe.hpp agent.hpp strategyinput.hpp card.hpp
	$(CXX) $(CXXFLAGS) -c simenginebj.cpp

agent.o: agent.cpp agent.hpp card.hpp simenginebj.hpp strategyinput.hpp
	$(CXX) $(CXXFLAGS) -c agent.cpp

dealer.o: dealer.cpp dealer.hpp card.hpp
	$(CXX) $(CXXFLAGS) -c dealer.cpp

clean:
	del /q $(OBJs) $(TARGET)